/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ------+---------+---------+-------- + --------+---------+---------+---------*
 * Copyright (c) 2004  - Garance Alistair Drosehn <gad@FreeBSD.org>.
 * All rights reserved.
 *
 * Significan't modifications made to bring `ps' options somewhat closer
 * to the standard for `ps' as described in SingleUnixSpec-v3.
 * ------+---------+---------+-------- + --------+---------+---------+---------*
 */

#include <sys/user.h>
#include <unistd.h>

#include "ps.h"

static KINFO	*kinfo;
static char    *getenvinfo(KINFO *k);

process_list* getProcessList(void){
    process_list* p_list = malloc(sizeof(process_list));
    p_list->pnum = 0;
    p_list->procs = NULL;
    p_list->size = 0;
    p_list->block_size = 0;

    struct kinfo_proc *kp;
    KINFO *next_KINFO;
    int i;
    int nentries, nkept;
    struct kinfo_proc *kprocbuf;
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    size_t bufSize = 0;
    size_t orig_bufSize = 0;
    int local_error=0;
    int retry_count = 0;
    int dflag = 0;

    //select proc
    nentries = -1;

    if (sysctl(mib, 4, NULL, &bufSize, NULL, 0) < 0) {
        return p_list;
    }

    kprocbuf = kp = (struct kinfo_proc *)malloc(bufSize);

    retry_count = 0;
    orig_bufSize = bufSize;
    for(retry_count=0; ; retry_count++) {
        // retry for transient errors due to load in the system
        local_error = 0;
        bufSize = orig_bufSize;
        if ((local_error = sysctl(mib, 4, kp, &bufSize, NULL, 0)) < 0) {
            if (retry_count < 1000) {
                // 1 sec back off
                sleep(1);
                continue;
            }
            return p_list;
        } else if (local_error == 0) {
            break;
        }
        // 1 sec back off
        sleep(1);
    }

    /* This has to be after the second sysctl since the bufSize
       may have changed.  */
    nentries = bufSize / sizeof(struct kinfo_proc);

    nkept = 0;
    if (nentries > 0) {
        if ((kinfo = malloc(nentries * sizeof(*kinfo))) == NULL)
            return p_list;
        for (i = nentries; --i >= 0; ++kp) {
            if (kp->kp_proc.p_pid == 0) {
                continue;
            }
            if (dflag && (kp->kp_proc.p_pid == kp->kp_eproc.e_pgid))
                continue;

            char *name = kp->kp_proc.p_comm;
            if ((strstr(name, "wine") == NULL) && (strstr(name, ".exe") == NULL) && (strstr(name, ".bat") == NULL))
                continue;
            if (strstr(name, "q4wine"))
                continue;

            next_KINFO = &kinfo[nkept];
            next_KINFO->ki_p = kp;
            nkept++;
        }
    }

    if (nkept == 0)
        return p_list;

    //Allocate process_list array
    p_list->size = nkept;
    p_list->block_size = nkept;
    p_list->pnum = nkept;
    p_list->procs = (process_info**)malloc(sizeof(process_info*) * nkept);

    for (i = 0; i < nkept; i++) {
        process_info *p_info = malloc(sizeof(process_info));
        char *name = kinfo[i].ki_p->kp_proc.p_comm;
        p_info->name = (char*)malloc(strlen(name) + 1);
        strcpy(p_info->name, name);
        p_info->nice = kinfo[i].ki_p->kp_proc.p_nice;
        p_info->pid = kinfo[i].ki_p->kp_proc.p_pid;
        char *env = getenvinfo(&kinfo[i]);
        p_info->args = (char*)malloc(strlen(env) + 1);
        strcpy(p_info->args, env);
        p_list->procs[i] = p_info;
    }

    free(kprocbuf);
    free(kinfo);
    return p_list;
}

static char* getenvinfo(KINFO *k)
{
    int mib[3], argmax, nargs, c = 0;
    size_t size;
    char	 *procargs, *sp = NULL, *np, *cp;

    /* Get the maximum process arguments size. */
    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;

    size = sizeof(argmax);
    if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1) {
        return sp;
    }

    /* Allocate space for the arguments. */
    procargs = (char *)malloc(argmax);
    if (procargs == NULL) {
        return sp;
    }

    /*
         * Make a sysctl() call to get the raw argument space of the process.
         * The layout is documented in start.s, which is part of the Csu
         * project.  In summary, it looks like:
         *
         * /---------------\ 0x00000000
         * :               :
         * :               :
         * |---------------|
         * | argc          |
         * |---------------|
         * | arg[0]        |
         * |---------------|
         * :               :
         * :               :
         * |---------------|
         * | arg[argc - 1] |
         * |---------------|
         * | 0             |
         * |---------------|
         * | env[0]        |
         * |---------------|
         * :               :
         * :               :
         * |---------------|
         * | env[n]        |
         * |---------------|
         * | 0             |
         * |---------------| <-- Beginning of data returned by sysctl() is here.
         * | argc          |
         * |---------------|
         * | exec_path     |
         * |:::::::::::::::|
         * |               |
         * | String area.  |
         * |               |
         * |---------------| <-- Top of stack.
         * :               :
         * :               :
         * \---------------/ 0xffffffff
         */
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROCARGS2;
    mib[2] = KI_PROC(k)->p_pid;

    size = (size_t)argmax;
    if (sysctl(mib, 3, procargs, &size, NULL, 0) == -1) {
        free(procargs);
        return sp;
    }

    memcpy(&nargs, procargs, sizeof(nargs));
    cp = procargs + sizeof(nargs);

    /* Skip the saved exec_path. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            /* End of exec_path reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        free(procargs);
        return sp;
    }

    /* Skip trailing '\0' characters. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp != '\0') {
            /* Beginning of first argument reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        free(procargs);
        return sp;
    }
    /* Save where the argv[0] string starts. */
    sp = cp;

    /*
         * Iterate through the '\0'-terminated strings and convert '\0' to ' '
         * until a string is found that has a '=' character in it (or there are
         * no more strings in procargs).  There is no way to deterministically
         * know where the command arguments end and the environment strings
         * start, which is why the '=' character is searched for as a heuristic.
         */
    for (np = NULL; c < nargs && cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            c++;
            if (np != NULL) {
                /* Convert previous '\0'. */
                *np = ' ';
            } else {
                //*argv0len = cp - sp;
            }
            /* Note location of current '\0'. */
            np = cp;
        }
    }

    /*
         * If eflg is non-zero, continue converting '\0' characters to ' '
         * characters until no more strings that look like environment settings
         * follow.
         */
    if ( ( (getuid() == 0) || (KI_EPROC(k)->e_pcred.p_ruid == getuid()) ) ) {
        for (; cp < &procargs[size]; cp++) {
            if (*cp == '\0') {
                if (np != NULL) {
                    if (&np[1] == cp) {
                        /*
                                                 * Two '\0' characters in a row.
                                                 * This should normally only
                                                 * happen after all the strings
                                                 * have been seen, but in any
                                                 * case, stop parsing.
                                                 */
                        break;
                    }
                    /* Convert previous '\0'. */
                    *np = ' ';
                }
                /* Note location of current '\0'. */
                np = cp;
            }
        }
    }

    /* Clean up. */
    free(procargs);
    return sp;
}
