/***************************************************************************
 *   Copyright (C) 2005, 2006 by Dmitry Morozhnikov                        *
 *			   dmiceman@mail.ru                                            *
 *             (C) 2008, 2009 by Malakhov Alexey                           *
 *             brezerk@gmail.com                                           *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <linux/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <mntent.h>
#include <sys/param.h>
#include <pwd.h>

#include <linux/iso_fs.h>

#define FUSE_USE_VERSION 22
#define VERSION "based on fuseiso v.20070708 + RedHat path"
#include <fuse.h>

#include <zlib.h>
#include <locale.h>
#include <langinfo.h>

#include "isofs.h"

#ifdef __GNUC__
# define UNUSED(x) x __attribute__((unused))
#else
# define UNUSED(x) x
#endif

static char *imagefile = NULL;
static char *mount_point = NULL;
static int image_fd = -1;

int maintain_mount_point;
int maintain_mtab;
char* iocharset;

char* normalize_name(const char* fname) {
	char* abs_fname = (char *) malloc(PATH_MAX);
	realpath(fname, abs_fname);
	// ignore errors from realpath()
	return abs_fname;
};

int check_mount_point() {
	struct stat st;
	int rc = lstat(mount_point, &st);
	if(rc == -1 && errno == ENOENT) {
		// directory does not exists, createcontext
		rc = mkdir(mount_point, 0777); // let`s underlying filesystem manage permissions
		if(rc != 0) {
			perror("Can`t create mount point");
			return -EIO;
		};
	} else if(rc == -1) {
		perror("Can`t check mount point");
		return -1;
	};
	return 0;
};

void del_mount_point() {
	int rc = rmdir(mount_point);
	if(rc != 0) {
		perror("Can`t delete mount point");
	};
};

char* get_mtab_path() {
	char* mtab_path = (char*) malloc(PATH_MAX);
	uid_t uid = getuid();
	struct passwd* passwd = getpwuid(uid);
	if(!passwd) {
		fprintf(stderr, "Can`t get home directory for user %d: %s\n", uid, strerror(errno));
		return NULL;
	};
	mtab_path[0] = 0;
	if(passwd->pw_dir) { // may be NULL, who know..
		strncpy(mtab_path, passwd->pw_dir, PATH_MAX - 16);
		mtab_path[PATH_MAX - 1] = 0;
	};
	strcat(mtab_path, "/.mtab.fuseiso");
	return mtab_path;
};

int add_mtab_record() {
	int rc;
	char* mtab_path = get_mtab_path();
	if(!mtab_path) {
		return -EIO;
	};
	int fd = open(mtab_path, O_RDWR | O_CREAT, 0644);
	if(fd < 0) {
		perror("Can`t open mtab");
		return -EIO;
	};
	rc = lockf(fd, F_LOCK, 0);
	if(rc != 0) {
		perror("Can`t lock mtab");
		return -EIO;
	};
	FILE* mtab = setmntent(mtab_path, "a");
	if(!mtab) {
		perror("Can`t open mtab");
		return -EIO;
	};
	struct mntent ent;
	ent.mnt_fsname = imagefile;
	ent.mnt_dir = mount_point;
	ent.mnt_type = "fuseiso";
	ent.mnt_opts = "defaults";
	ent.mnt_freq = 0;
	ent.mnt_passno = 0;
	rc = addmntent(mtab, &ent);
	if(rc != 0) {
		perror("Can`t add mtab entry");
		return -EIO;
	};
	endmntent(mtab);
	rc = lockf(fd, F_ULOCK, 0);
	if(rc != 0) {
		perror("Can`t unlock mtab");
		return -EIO;
	};
	close(fd);
	free(mtab_path);
	return 0;
};

int del_mtab_record() {
	int rc;
	char* mtab_path = get_mtab_path();
	char new_mtab_path[PATH_MAX];
	if(!mtab_path) {
		return -EIO;
	};
	int fd = open(mtab_path, O_RDWR | O_CREAT, 0644);
	if(fd < 0) {
		perror("Can`t open mtab");
		return -EIO;
	};
	rc = lockf(fd, F_LOCK, 0);
	if(rc != 0) {
		perror("Can`t lock mtab");
		return -EIO;
	};
	strncpy(new_mtab_path, mtab_path, PATH_MAX - 16);
	new_mtab_path[PATH_MAX - 1] = 0;
	strcat(new_mtab_path, ".new");
	FILE* mtab = setmntent(mtab_path, "r");
	if(!mtab) {
		perror("Can`t open mtab");
		return -EIO;
	};
	FILE* new_mtab = setmntent(new_mtab_path, "a+");
	if(!new_mtab) {
		perror("Can`t open new mtab");
		return -EIO;
	};
	struct mntent* ent;
	while((ent = getmntent(mtab)) != NULL) {
		if(strcmp(ent->mnt_fsname, imagefile) == 0 &&
			strcmp(ent->mnt_dir, mount_point) == 0 &&
			strcmp(ent->mnt_type, "fuseiso") == 0) {
			// skip
		} else {
			rc = addmntent(new_mtab, ent);
			if(rc != 0) {
				perror("Can`t add mtab entry");
				return -EIO;
			};
		};
	};
	endmntent(mtab);
	endmntent(new_mtab);
	rc = rename(new_mtab_path, mtab_path);
	if(rc != 0) {
		perror("Can`t rewrite mtab");
		return -EIO;
	};
	rc = lockf(fd, F_ULOCK, 0);
	if(rc != 0) {
		perror("Can`t unlock mtab");
		return -EIO;
	};
	close(fd);
	free(mtab_path);
	return 0;
};

static int isofs_getattr(const char *path, struct stat *stbuf)
{
	return isofs_real_getattr(path, stbuf);
}

static int isofs_readlink(const char *path, char *target, size_t size) {
	return isofs_real_readlink(path, target, size);
};

static int isofs_open(const char *path, struct fuse_file_info *UNUSED(fi))
{
	return isofs_real_open(path);
}

static int isofs_read(const char *path, char *buf, size_t size,
					 off_t offset, struct fuse_file_info *UNUSED(fi))
{
	return isofs_real_read(path, buf, size, offset);
}

static int isofs_flush(const char *UNUSED(path), struct fuse_file_info *UNUSED(fi)) {
	return 0;
};

static void* isofs_init() {
	int rc;
	if(maintain_mtab) {
		rc = add_mtab_record();
		if(rc != 0) {
			exit(EXIT_FAILURE);
		};
	};
	return isofs_real_init();
};

static void isofs_destroy(void* param) {
	if(maintain_mtab) {
		del_mtab_record();
	};
	return;
};

static int isofs_opendir(const char *path, struct fuse_file_info *UNUSED(fi)) {
	return isofs_real_opendir(path);
};

static int isofs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t UNUSED(offset),
	struct fuse_file_info *UNUSED(fi)) {
	return isofs_real_readdir(path, buf, filler);
};

static int isofs_statfs(const char *UNUSED(path), struct statfs *stbuf)
{
	return isofs_real_statfs(stbuf);
}

static struct fuse_operations isofs_oper = {
	.getattr    = isofs_getattr,
	.readlink   = isofs_readlink,
	.open       = isofs_open,
	.read       = isofs_read,
	.flush      = isofs_flush,
	.init       = isofs_init,
	.destroy    = isofs_destroy,
	.opendir    = isofs_opendir,
	.readdir    = isofs_readdir,
	.statfs     = isofs_statfs,
};

void usage(const char* prog) {
	printf("Version: %s\nUsage: %s [-n] [-p] [-c <iocharset>] [-h] <isofs_image_file> <mount_point> [<FUSE library options>]\n"
		"Where options are:\n"
		"    -n                 -- do NOT maintain file $HOME/.mtab.fuseiso\n"
		"    -p                 -- maintain mount point; \n"
		"                          create it if it does not exists and delete it on exit\n"
		"    -c <iocharset>     -- specify iocharset for Joliet filesystem\n"
		"    -h                 -- print this screen\n"
		"\nCommon FUSE library options are:\n"
		"    -f                 -- run in foreground, do not daemonize\n"
		"    -d                 -- run in foreground and print debug information\n"
		"    -s                 -- run single-threaded\n"
		"\nPlease consult with FUSE ducumentation for more information\n",
		VERSION,
		prog);
};

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, ""); // set current locale for proper iocharset

	// defaults
	maintain_mount_point = 0;
	maintain_mtab = 1;
	iocharset = NULL;

	char c;
	while((c = getopt(argc, argv, "+npc:h")) > 0) {
		switch(c) {
			case 'n':
				maintain_mtab = 0;
				break;
			case 'p':
				maintain_mount_point = 1;
				break;
			case 'c':
				if(optarg) {
					iocharset = optarg;
				};
				break;
			case 'h':
				usage(argv[0]);
				exit(0);
				break;
			case '?':
			case ':':
				usage(argv[0]);
				exit(EXIT_FAILURE);
				break;
		};
	};

	if((argc - optind) < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	};

	imagefile = normalize_name(argv[optind]);

	image_fd = open(imagefile, O_RDONLY);
	if(image_fd == -1) {
		fprintf(stderr, "Supplied image file name: \"%s\"\n", imagefile);
		perror("Can`t open image file");
		exit(EXIT_FAILURE);
	};

	mount_point = normalize_name(argv[optind + 1]);

	// with space for possible -o use_ino arguments
	char **nargv = (char **) malloc((argc + 2) * sizeof(char *));
	int nargc = argc - optind;

	nargv[0] = argv[0];

	int i;
	int next_opt = 0;
	int use_ino_found = 0;
	for(i = 0; i < nargc - 1; ++i) {
		if(next_opt && !use_ino_found) {
			if(strstr(argv[i + optind + 1], "use_ino")) { // ok, already there
				use_ino_found = 1;
				nargv[i + 1] = argv[i + optind + 1];
			} else { // add it
				char* str = (char*) malloc(strlen(argv[i + optind + 1]) + 10);
				strcpy(str, argv[i + optind + 1]);
				strcat(str, ",use_ino");
				nargv[i + 1] = str;
				use_ino_found = 1;
			};
		} else {
			nargv[i + 1] = argv[i + optind + 1];
		};
		// check if this is -o string mean that next argument should be options string
		if(i > 1 && nargv[i + 1][0] == '-' && nargv[i + 1][1] == 'o') {
			next_opt = 1;
		};
	};
	if(!use_ino_found) {
		nargv[nargc] = "-o";
		nargc++;
		nargv[nargc] = "use_ino";
		nargc++;
	};

	if(!iocharset) {
		char *nlcharset = nl_langinfo(CODESET);
		if(nlcharset) {
			iocharset = (char *) malloc(strlen(nlcharset) + 9);
			strcpy(iocharset, nlcharset);
			strcat(iocharset, "//IGNORE");
		};
		if(!iocharset) {
			iocharset = "UTF-8//IGNORE";
		};
	};

	int rc;
	if(maintain_mount_point) {
		rc = check_mount_point();
		if(rc != 0) {
			exit(EXIT_FAILURE);
		};
	};
	if(maintain_mount_point) {
		rc = atexit(del_mount_point);
		if(rc != 0) {
			fprintf(stderr, "Can`t set exit function\n");
			exit(EXIT_FAILURE);
		}
	};

	// will exit in case of failure
	rc = isofs_real_preinit(imagefile, image_fd);

	return fuse_main(nargc, nargv, &isofs_oper);
};
