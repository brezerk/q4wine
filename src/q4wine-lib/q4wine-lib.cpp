/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                           *
 *   brezerk@gmail.com                                                     *
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

#include "q4wine-lib.h"

corelib::corelib(bool _GUI_MODE)
{
	// Setting gui mode, if false - cli mode else gui mode
	this->_GUI_MODE=_GUI_MODE;
	this->xdg_open="";
	this->mdconfig="";
	this->fuseiso="";
	this->fusermount="";
}

corelib* createCoreLib(bool _GUI_MODE){
	return new corelib(_GUI_MODE);
}

QList<QStringList> corelib::getWineProcessList(){
	QList<QStringList> proclist;
	QStringList procline;

	QString name, procstat, path, prefix, env_arg, nice;

#ifdef _OS_LINUX_
	QString message = "<p>Process is unable access to /proc file system.</p><p>Access is necessary for displaying wine process information.</p><p>You need to set CONFIG_PROC_FS=y option on linux kernel config file and mount proc file system by running: mount -t proc none /proc</p>";
#endif

#ifdef _OS_FREEBSD_
	QString message = "<p>Process is unable access to /proc file system.</p><p>Access is necessary for displaying wine process information.</p><p>You need to set PSEUDOFS and PROCFS option on FreeBSD kernel config file and mount proc file system by running: mount -t procfs proc /proc</p>";
#endif

	// Check for /proc directory exists
	QDir dir("/proc");
	if (!dir.exists()){
		if (this->showError(message, false) == QMessageBox::Ignore){
			procline << "-1";
			proclist << procline;
			return proclist;
		}
	}

	/* On Linux:
	   * This is new engine for getting process info from /proc directory
	   * its fully wrighted with QT and might work more stable =)
	   */
#ifdef _OS_LINUX_
	dir.setFilter(QDir::Dirs | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);

	QFileInfoList list = dir.entryInfoList();

	// Getting directoryes one by one
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		path = "/proc/";
		path.append(fileInfo.fileName());
		path.append("/stat");
		QFile file(path);
		path = "/proc/";
		path.append(fileInfo.fileName());
		path.append("/exe");
		QFileInfo exelink(path);
		// Try to read stat file
		if (exelink.isSymLink() && (exelink.symLinkTarget().contains("wineserver") || exelink.symLinkTarget().contains("wine-preloader"))){
			if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&file);
				QString line = in.readLine();
				if (!line.isNull()){
					// Getting nice and name of the process
					nice = line.section(' ', 18, 18);
					name = line.section(' ', 1, 1);
					name.remove(QChar('('));
					name.remove(QChar(')'));
					name = name.toLower();

					// If name contains wine or .exe and not contains q4wine,
					// then we try to get environ variables.
					//if ((name.contains("wine") || name.contains(".exe")) && !name.contains(APP_SHORT_NAME)){
					path = "/proc/";
					path.append(fileInfo.fileName());
					path.append("/environ");
					QFile e_file(path);

					// Getting WINEPREFIX variable.
					if (e_file.open(QIODevice::ReadOnly | QIODevice::Text)){
						QTextStream e_in(&e_file);
						QString e_line = e_in.readLine();
						int index = e_line.indexOf("WINEPREFIX=");
						prefix="";
						if (index!=-1)
							for (int i=index+11; i<=e_line.length(); i++){
							if (e_line.mid(i, 1).data()->isPrint()){
								prefix.append(e_line.mid(i, 1));
							} else {
								break;
							}
						}
					}

					// Puting all fields into QList<QStringList>
					procline.clear();
					procline << fileInfo.fileName() << name << nice << prefix;
					proclist << procline;

					//}
				}
				file.close();
			}
		}
	}
#endif

	/* On FreeBSD:
		 * This is new engine for getting process info from /proc directory and kmem interface
		 */
#ifdef _OS_FREEBSD_
	kvm_t *kd;
	int cntproc, i, ni, ipid;

	struct kinfo_proc *kp;
	char buf[_POSIX2_LINE_MAX];
	char **envs;

	kd = kvm_openfiles(_PATH_DEVNULL, _PATH_DEVNULL, NULL, O_RDONLY, buf);
	if (!kd){
		if (this->showError(QObject::tr("<p>It seems q4wine can not run kvm_openfiles.</p>"), false) == QMessageBox::Ignore){
			procline << "-1";
			proclist << procline;
			return proclist;
		}
		return proclist;
	}
	kp = kvm_getprocs(kd, KERN_PROC_PROC, 0, &cntproc);
	if (!kp){
		if (this->showError(QObject::tr("<p>It seems q4wine can not run kvm_getprocs.</p>"), false) == QMessageBox::Ignore){
			procline << "-1";
			proclist << procline;
			return proclist;
		}
		return proclist;
	}

	QStringList cur_pids;
	for (i=0; i<cntproc;i++){
		prefix="";
		ipid = kp[i].ki_pid;

		if (cur_pids.indexOf(QString("%1").arg(ipid))==-1){
			cur_pids << QString("%1").arg(ipid);
			name = kp[i].ki_comm;

			if ((name.contains("wine") || name.contains(".exe")) && !name.contains(APP_SHORT_NAME)){
				ni = kp[i].ki_nice;
				nice = QString("%1").arg(ni);

				envs = kvm_getargv(kd, (const struct kinfo_proc *) &(kp[i]), 0);
				if (envs){
					name = envs[0];
					if (name.isEmpty()){
						name = kp[i].ki_comm;
					} else {
						name = name.split('\\').last();
						name = name.split('/').last();
					}
				} else {
					qDebug()<< "kvm_getargv failed: " << kvm_geterr(kd);
				}

				envs = kvm_getenvv(kd, (const struct kinfo_proc *) &(kp[i]), 0);
				if (envs){
					int j=0;
					while (envs[j]){
						env_arg=envs[j];
						int index = env_arg.indexOf("WINEPREFIX=");
						if (index>=0){
							prefix=env_arg.mid(11);
							break;
						}
						j++;
					}
				} else {
					qDebug()<< "kvm_getenvv failed: " << kvm_geterr(kd);
				}

				// Puting all fields into QList<QStringList>
				procline.clear();
				procline << QString("%1").arg(ipid) << name << nice << prefix;
				proclist << procline;
			}
		}
	}

	kvm_close(kd);
#endif


	return proclist;
}

QVariant corelib::getSetting(const QString group, const QString key, const bool checkExist, const QVariant defaultVal) const{
	QVariant retVal;
	QSettings settings(APP_SHORT_NAME, "default");

	settings.beginGroup(group);
	retVal = settings.value(key, defaultVal);
	settings.endGroup();
	if (checkExist==true)
		if (!QFileInfo(retVal.toString()).exists()){
		this->showError(QObject::tr("<p>Error while loading application settings by key: '%1'. File or path not exists: \"%2\"</p><p>Please, go to %3 options dialog and set it.</p>").arg(key).arg(retVal.toString()).arg(APP_SHORT_NAME));
		retVal = QVariant();
	}
	return retVal;
}

void corelib::checkSettings(){
	/*
	 * Getting application settings
	 */

	this->getSetting("system", "tar");
	this->getSetting("system", "mount");
	this->getSetting("system", "umount");
	this->getSetting("system", "sudo");
	this->getSetting("system", "gui_sudo");
	this->getSetting("system", "nice");
	this->getSetting("system", "renice");
	this->getSetting("system", "sh");
	this->getSetting("console", "bin");
	this->getSetting("console", "args", false);

#ifdef WITH_ICOUTILS
	this->getSetting("icotool", "wrestool");
	this->getSetting("icotool", "icotool");
#endif

	if (this->getSetting("quickmount", "type", FALSE).toString().isEmpty()){
		QSettings settings(APP_SHORT_NAME, "default");
		settings.beginGroup("quickmount");

		if (this->getWhichOut("fuseiso", false).isEmpty()){
#ifdef WITH_EMBEDDED_FUSEISO
			settings.setValue("type", 3);
			settings.setValue("mount_drive_string", this->getMountString(3));
			settings.setValue("mount_image_string", this->getMountImageString(3));
			settings.setValue("umount_string", this->getUmountString(3));
#else
			settings.setValue("type", 0);
			settings.setValue("mount_drive_string", this->getMountString(0));
			settings.setValue("mount_image_string", this->getMountImageString(0));
			settings.setValue("umount_string", this->getUmountString(0));
#endif
		} else {
			settings.setValue("type", 2);
			settings.setValue("mount_drive_string", this->getMountString(2));
			settings.setValue("mount_image_string", this->getMountImageString(2));
			settings.setValue("umount_string", this->getUmountString(2));
		}
		settings.endGroup();
	}

	return;
}

QIcon corelib::loadIcon(QString iconName){
	// Function tryes to load icon image from theme dir
	// If it fails -> load default from rsource file

	QIcon icon;
	QString themeName = this->getSetting("app", "theme", false, "Default").toString();

	if ((!themeName.isEmpty()) and (themeName!="Default")){
		icon.addFile(QString("%1/%2").arg(themeName).arg(iconName));
		if (icon.isNull()){
			icon.addFile(QString(":/%1").arg(iconName));
		}
	} else {
		icon.addFile(QString(":/%1").arg(iconName));
	}

	return icon;
}

QPixmap corelib::loadPixmap(QString iconName){
	// Function tryes to load icon image from theme dir
	// If it fails -> load default from rsource file

	QPixmap pixmap;
	QString themeName = this->getSetting("app", "theme", false, "Default").toString();

	if ((!themeName.isEmpty()) and (themeName!="Default")){
		pixmap.load(QString("%1/%2").arg(themeName).arg(iconName));
		if (pixmap.isNull()){
			pixmap.load(QString(":/%1").arg(iconName));
		}
	} else {
		pixmap.load(QString(":/%1").arg(iconName));
	}

	return pixmap;
}

QString  corelib::getLang(){
	QString lang=this->getSetting("app", "lang", false).toString();

	if (!lang.isEmpty()){
#ifdef DEBUG
		qDebug()<<"[ii] Get lang from settings:"<<lang;
#endif
		return lang;
	}

	// This is hack for next QLocale bug:
	//  http://bugs.gentoo.org/150745

	lang = setlocale(LC_ALL, "");
#ifdef DEBUG
	qDebug()<<"[ii] LC_ALL: "<<lang;
#endif
	if (lang.isEmpty()){
		lang = setlocale(LC_MESSAGES, "");
#ifdef DEBUG
		qDebug()<<"[ii] LC_MESSAGES: "<<lang;
#endif
		if (lang.isEmpty()){
			lang = getenv("LANG");
#ifdef DEBUG
			qDebug()<<"[ii] Env LANG: "<<lang;
#endif
		}
	}

#ifdef DEBUG
	qDebug()<<"[ii] Lang before split: "<<lang;
#endif

	lang = lang.split(".").at(0).toLower();
	if (lang.contains("=")){
		lang = lang.split("=").last();
	}

#ifdef DEBUG
	qDebug()<<"[ii] Lang to load: "<<lang;
#endif
	return lang;
}

QString  corelib::getLocale(){
    QString lang;
        lang = setlocale(LC_ALL, "");
#ifdef DEBUG
        qDebug()<<"[ii] LC_ALL: "<<lang;
#endif
        if (lang.isEmpty()){
                lang = setlocale(LC_MESSAGES, "");
#ifdef DEBUG
                qDebug()<<"[ii] LC_MESSAGES: "<<lang;
#endif
                if (lang.isEmpty()){
                        lang = getenv("LANG");
#ifdef DEBUG
                        qDebug()<<"[ii] Env LANG: "<<lang;
#endif
                }
        }

#ifdef DEBUG
        qDebug()<<"[ii] Lang before split: "<<lang;
#endif

        QStringList loc = lang.split(".");
#ifdef DEBUG
        qDebug()<<loc.count();
#endif
        if (loc.count()==2){
            lang = loc.at(1).toLower();
        } else {
            lang = "utf8";
        }

#ifdef DEBUG
        qDebug()<<"[ii] Lang to load: "<<lang;
#endif
        return lang;
}

void corelib::getBuildFlags(){
    QTextStream Qcout(stdout);
    Qcout<<QObject::tr("Buildtime flags are:")<<endl<<endl;
    Qcout<<qSetFieldWidth(25)<<left<<" CMAKE_INSTALL_PREFIX"<<APP_PREF<<qSetFieldWidth(0)<<endl<<endl;
#ifdef DEBUG
    Qcout<<qSetFieldWidth(25)<<left<<" DEBUG"<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" DEBUG"<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
#ifdef WITH_ICOUTILS
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_ICOUTILS"<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_ICOUTILS"<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
#ifdef WITH_WINETRIKS
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_WINETRIKS"<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_WINETRIKS"<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
#ifdef WITH_EMBEDDED_FUSEISO
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_EMBEDDED_FUSEISO"<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_EMBEDDED_FUSEISO"<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
#ifdef WITH_WINEAPPDB
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_WINEAPPDB "<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_WINEAPPDB "<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
    Qcout<<endl;
}

QString corelib::getWhichOut(const QString fileName, bool showErr){
	/*
   * Getting 'which' output;
   */

	if (fileName=="xdg-open"){
		if (!xdg_open.isEmpty())
			return xdg_open;
	} else if  (fileName=="mdconfig"){
		if (!mdconfig.isEmpty())
			return mdconfig;
	} else if  (fuseiso=="fuseiso"){
		if (!fuseiso.isEmpty())
			return fuseiso;
	} else if  (fileName=="fusermount"){
		if (!fusermount.isEmpty())
			return fusermount;
	} else if  (fileName=="ln"){
		if (!ln.isEmpty())
			return ln;
	} else if  (fileName=="rm"){
		if (!rm.isEmpty())
			return rm;
	} else if  (fileName=="sh"){
		if (!sh.isEmpty())
			return sh;
	}

	QProcess proc;
	QStringList args;

	args<<fileName;

	proc.setWorkingDirectory (QDir::homePath());
	proc.start("which", args, QIODevice::ReadOnly);
	proc.waitForFinished();

	QString string = proc.readAllStandardOutput();

	if (!string.isEmpty()){
		if (fileName=="xdg-open"){
			xdg_open=string.trimmed();
		} else if (fileName=="mdconfig"){
			mdconfig=string.trimmed();
		} else if (fileName=="fuseiso"){
			fuseiso=string.trimmed();
		} else if (fileName=="fusermount"){
			fusermount=string.trimmed();
		} else if (fileName=="ln"){
			ln=string.trimmed();
		} else if (fileName=="rm"){
			rm=string.trimmed();
		} else if (fileName=="sh"){
			sh=string.trimmed();
		}

		return string.trimmed();
	} else {
		if (showErr)
			this->showError(QObject::tr("Can't find or execute '%1' binary. Make shure this binary is available by search PATH variable and see also INSTALL file for application depends.").arg(fileName));
	}

	return "";
}

QStringList corelib::getCdromDevices(void) const{
	QStringList retVal;

	QDir dir("/dev/");
	dir.setFilter(QDir::Files | QDir::System);
	dir.setSorting(QDir::Name);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);

#ifdef _OS_LINUX_
		if (fileInfo.fileName().contains(QRegExp("^cdrom")) or fileInfo.fileName().contains(QRegExp("^sr")) or fileInfo.fileName().contains(QRegExp("^dvd"))){
#endif
#ifdef _OS_FREEBSD_
			if (fileInfo.fileName().contains(QRegExp("^cdrom")) or fileInfo.fileName().contains(QRegExp("^cd")) or fileInfo.fileName().contains(QRegExp("^acd")) or fileInfo.fileName().contains(QRegExp("^dvd"))){
#endif
				if (fileInfo.isSymLink()){
					if (!retVal.contains(fileInfo.symLinkTarget()))
						retVal.append(fileInfo.symLinkTarget());
				} else {
					if (!retVal.contains(fileInfo.absoluteFilePath()))
						retVal.append(fileInfo.absoluteFilePath());
				}
			}
		}

		return retVal;
	}

	QStringList corelib::getWineDlls(QString prefix_lib_path) const{
		QStringList dllList;
		if (prefix_lib_path.isEmpty()){
			prefix_lib_path=this->getSetting("wine", "WineLibs").toString();
		}

		QDir dir(prefix_lib_path);
		dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			if (fileInfo.fileName().indexOf(".dll.so")>=0){
				dllList<<fileInfo.fileName().left(fileInfo.fileName().length()-3);
			}
		}

		return dllList;
	}

	QString corelib::getMountedImages(const QString cdrom_mount) const{
		QString image="";
		QStringList arguments;

#ifdef _OS_LINUX_
		arguments << "-c" << QString("%1 | grep %2").arg(this->getSetting("system", "mount").toString()).arg(cdrom_mount);
#endif
#ifdef _OS_FREEBSD_
		arguments << "-c" << QString("%1 | grep %2").arg(this->getSetting("system", "mount").toString()).arg(cdrom_mount);
#endif

		QProcess myProcess;
		myProcess.start(this->getSetting("system", "sh").toString(), arguments);
		if (!myProcess.waitForFinished()){
			qDebug() << "Make failed:" << myProcess.errorString();
			return QString();
		}

		image = myProcess.readAll();

		if (!image.isEmpty()){
			image = image.split(" ").first();
			if (!image.isEmpty()){
#ifdef _OS_LINUX_
				if (image.contains("loop")){
#endif
#ifdef _OS_FREEBSD_
					if (image.contains("md")){
#endif
						myProcess.close ();
						arguments.clear();
#ifdef _OS_LINUX_
						arguments << "losetup" << image;
#endif
#ifdef _OS_FREEBSD_
						arguments << "mdconfig" <<  "-l" << QString("-u%1").arg(image.mid(7));
#endif
						myProcess.start(this->getSetting("system", "sudo").toString(), arguments);
						if (!myProcess.waitForFinished()){
							qDebug() << "Make failed:" << myProcess.errorString();
							return QString();
						} else {
							image = myProcess.readAll();
#ifdef _OS_LINUX_
							image = image.split("/").last().mid(0, image.split("/").last().length()-2);
#endif
#ifdef _OS_FREEBSD_
							image = image.split("/").last().mid(0, image.split("/").last().length()-1);
#endif
						}
					} else if (image.contains("fuseiso") || image.contains("q4wine-mount")){
						QString filename;
						filename=QDir::homePath();
						filename.append("/.mtab.fuseiso");
						QFile file(filename);
						if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
							QTextStream in(&file);
							while (!in.atEnd()) {
								QString line = in.readLine();
								if (line.contains(cdrom_mount))
									image = line.split(" ").first().split("/").last();
							}
						} else {
							image = "fuseiso [cant read $HOME/.mtab.fuseiso]";
						}
					}
				} else {
					image = "none";
				}
			} else {
				image = "none";
			}
			return image;
		}

		bool corelib::runIcon(const QString prefix_name, const QString dir_name, const QString icon_name) const{
			QStringList result = db_icon.getByName(prefix_name, dir_name, icon_name);
			//  0   1     2     3          4       5         6          7           8        9        10    11       12    13         14
			//	id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id
			ExecObject execObj;
			execObj.wrkdir = result.at(4);
			execObj.override = result.at(5);
			execObj.winedebug = result.at(6);
			execObj.useconsole = result.at(7);
			execObj.display = result.at(8);
			execObj.cmdargs = result.at(9);
			execObj.execcmd = result.at(10);
			execObj.desktop = result.at(11);
			execObj.nice = result.at(12);
			execObj.prefixid = result.at(13);
			execObj.name = icon_name;
			return runWineBinary(execObj);
		}

		bool corelib::runWineBinary(const ExecObject execObj) const{
			QStringList prefixList;
			// 0   1     2             3            4            5          6            7
			// id, path, wine_dllpath, wine_loader, wine_server, wine_exec, cdrom_mount, cdrom_drive
			prefixList = db_prefix.getFieldsByPrefixId(execObj.prefixid);

			QString exec;
			QStringList args;
			QString envargs;

			if (execObj.useconsole == "1"){
				// If we gona use console output, so exec program is program specificed at CONSOLE global variable
				exec = this->getSetting("console", "bin").toString();

				if (!this->getSetting("console", "args", false).toString().isEmpty()){
					// If we have any conslope parametres, we gona preccess them one by one
					QStringList cons_args = this->getSetting("console", "args", false).toString().split(" ");
					for (int i=0; i<cons_args.count(); i++){
						if (!cons_args.at(i).isEmpty())
							args.append(cons_args.at(i));
					}
				}

				args.append(this->getSetting("system", "sh").toString());

			} else {
				exec = this->getSetting("system", "sh").toString();
			}

			args.append("-c");


			if ((execObj.useconsole == "1") && (!execObj.wrkdir.isNull())){
				envargs.append(" cd '");
				envargs.append(execObj.wrkdir);
				envargs.append("' && ");
			}

			if (!prefixList.at(1).isEmpty()){
				//If icon has prefix -- add to args
				envargs.append(QString(" WINEPREFIX=\"%1\" ").arg(prefixList.at(1)));
			} else {
				//Else use default prefix
				envargs.append(QString(" WINEPREFIX=\"%1/.wine\" ").arg(QDir::homePath()));
			}

			if (!prefixList.at(2).isEmpty()){
				envargs.append(QString(" WINEDLLPATH=\"%1\" ").arg(prefixList.at(2)));
			} else {
				envargs.append(QString(" WINEDLLPATH=\"%1\" ").arg(this->getSetting("wine", "WineLibs").toString()));
			}

			if (!prefixList.at(3).isEmpty()){
				envargs.append(QString(" WINELOADER=\"%1\" ").arg(prefixList.at(3)));
			} else {
				envargs.append(QString(" WINELOADER=\"%1\" ").arg(this->getSetting("wine", "LoaderBin").toString()));
			}

			if (!prefixList.at(4).isEmpty()){
				envargs.append(QString(" WINESERVER=\"%1\" ").arg(prefixList.at(4)));
			} else {
				envargs.append(QString(" WINESERVER=\"%1\" ").arg(this->getSetting("wine", "ServerBin").toString()));
			}

			if (!execObj.override.isEmpty()){
				envargs.append(QString(" WINEDLLOVERRIDES=\"%1\" ").arg(execObj.override));
			}

			if (!execObj.winedebug.isEmpty() && execObj.useconsole == "1"){
				envargs.append(QString(" WINEDEBUG=\"%1\" ").arg(execObj.winedebug));
			}

			if (!execObj.display.isEmpty()){
				envargs.append(QString(" DISPLAY=\"%1\" ").arg(execObj.display));
			}

			QString exec_string = "";

			exec_string.append(envargs);
			if(!execObj.nice.isEmpty()){
				exec_string.append(this->getSetting("system", "nice").toString());
				exec_string.append(" -n ");
				exec_string.append(execObj.nice);
			}

			exec_string.append(" ");

			if (!prefixList.at(5).isEmpty()){
				exec_string.append(prefixList.at(5));
			} else {
				exec_string.append(this->getSetting("wine", "WineBin").toString());
			}

			exec_string.append(" ");

			if (!execObj.desktop.isEmpty()){
				exec_string.append(" explorer.exe /desktop=");
				QString repl = execObj.name;
				exec_string.append(repl.replace(" ", ""));
				exec_string.append(",");
				exec_string.append(execObj.desktop);
			}

			exec_string.append(" \"");
			exec_string.append(execObj.execcmd);
			exec_string.append("\" ");
			exec_string.append(execObj.cmdargs);
			if (execObj.useconsole != "1")
				exec_string.append(" > /dev/null 2>&1");

			args.append(exec_string);

#ifdef DEBUG
			qDebug()<<"[ii] corelib::runWineBinary via ExecObj args: "<<exec<<args<<execObj.wrkdir;
#endif

			QProcess proc;
			return proc.startDetached( exec, args, execObj.wrkdir );
		}

		bool corelib::runWineBinary(const QString windows_binary, const QString cmdargs, const QString prefix_name, const QString wineAppendBin, const bool releaseProc) const{
			QStringList prefixList;
			// 0   1     2             3            4            5          6            7
			// id, path, wine_dllpath, wine_loader, wine_server, wine_exec, cdrom_mount, cdrom_drive
			prefixList = db_prefix.getFieldsByPrefixName(prefix_name);

			QString exec;
			QStringList args;
			QString envargs;

			exec = this->getSetting("system", "sh").toString();

			args.append("-c");

			if (!prefixList.at(1).isEmpty()){
				//If icon has prefix -- add to args
				envargs.append(QString(" WINEPREFIX=\"%1\" ").arg(prefixList.at(1)));
			} else {
				//Else use default prefix
				envargs.append(QString(" WINEPREFIX=\"%1/.wine\" ").arg(QDir::homePath()));
			}

			if (!prefixList.at(2).isEmpty()){
				envargs.append(QString(" WINEDLLPATH=\"%1\" ").arg(prefixList.at(2)));
			} else {
				envargs.append(QString(" WINEDLLPATH=\"%1\" ").arg(this->getSetting("wine", "WineLibs").toString()));
			}

			if (!prefixList.at(3).isEmpty()){
				envargs.append(QString(" WINELOADER=\"%1\" ").arg(prefixList.at(3)));
			} else {
				envargs.append(QString(" WINELOADER=\"%1\" ").arg(this->getSetting("wine", "LoaderBin").toString()));
			}

			if (!prefixList.at(4).isEmpty()){
				envargs.append(QString(" WINESERVER=\"%1\" ").arg(prefixList.at(4)));
			} else {
				envargs.append(QString(" WINESERVER=\"%1\" ").arg(this->getSetting("wine", "ServerBin").toString()));
			}

			QString exec_string = "";
			exec_string.append(envargs);
			exec_string.append(" ");

			if (!prefixList.at(5).isEmpty()){
				exec_string.append(prefixList.at(5));
			} else {
				exec_string.append(this->getSetting("wine", "WineBin").toString());
			}

			if (!wineAppendBin.isEmpty())
				exec_string.append(wineAppendBin);

			exec_string.append(" ");
			if (!windows_binary.isEmpty()){
				exec_string.append(" \"");
				exec_string.append(windows_binary);
				exec_string.append("\" ");
			}

			exec_string.append(cmdargs);
			args.append(exec_string);

#ifdef DEBUG
			qDebug()<<"[ii] corelib::runWineBinary via Requested args: "<<exec<<args<<QDir::homePath();
#endif

			if (releaseProc){
				QProcess proc;
				return proc.startDetached(exec, args, QDir::homePath());
			} else {
				return this->runProcess(args, QObject::tr("Running wine binary"), QObject::tr("Please wait..."));
			}
			return false;
		}

		QString corelib::createDesktopFile(const QString prefix_name, const QString dir_name, const QString icon_name) const{
			QStringList result = db_icon.getByName(prefix_name, dir_name, icon_name);

			QString fileName = QDir::homePath();
			fileName.append("/.config/");
			fileName.append(APP_SHORT_NAME);
			fileName.append("/tmp/");
			fileName.append(result.at(1));
			fileName.append(".desktop");

			QFile file(fileName);
			file.open(QIODevice::Truncate | QIODevice::WriteOnly);

			QTextStream out(&file);
			out<<"[Desktop Entry]"<<endl;
			out<<"Exec="<<APP_PREF<<"/bin/q4wine-cli -p \""<<prefix_name<<"\" ";
			if (!dir_name.isEmpty())
				out<<" -d \""<<dir_name<<"\" ";
			out<<" -i \""<<icon_name<<"\" "<<endl;

			if (result.at(3).isEmpty()){
				out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/exec_wine.png"<<endl;
			} else {
				if (result.at(3)=="winecfg"){
					out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/winecfg.png"<<endl;
				} else if (result.at(3)=="wineconsole"){
					out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/wineconsole.png"<<endl;
				} else if (result.at(3)=="uninstaller"){
					out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/uninstaller.png"<<endl;
				} else if (result.at(3)=="regedit"){
					out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/regedit.png"<<endl;
				} else if (result.at(3)=="explorer"){
					out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/explorer.png"<<endl;
				} else if (result.at(3)=="eject"){
					out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/eject.png"<<endl;
				} else if (result.at(3)=="wordpad"){
					out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/notepad.png"<<endl;
				} else {
					out<<"Icon="<<result.at(3)<<endl;
				}
			}
			out<<"Type=Application"<<endl;
			out<<"X-KDE-StartupNotify=true"<<endl;
			out<<"GenericName="<<result.at(2)<<endl;
			out<<"Name="<<result.at(2)<<endl;
			out<<"Path="<<result.at(4)<<endl;

			file.close();

			return fileName;
		}

		QString corelib::getEscapeString(const QString string) const{
			return QRegExp::escape(string).replace(" ", "\\ ");
		}

                bool corelib::mountImage(const QString image_name, const QString prefix_name){

                        this->umountImage(prefix_name);

			QString mount_point=db_prefix.getFieldsByPrefixName(prefix_name).at(6);
#ifdef DEBUG
			qDebug()<<"[ii] corelib::mountImage: mount point: "<<mount_point;
#endif

			if (mount_point.isEmpty()){
				this->showError(QObject::tr("It seems no mount point was set in prefix options.<br>You might need to set it manualy."));
				return false;
			}
			if (image_name.isEmpty())
				return false;

			QStringList args;
			QString mount_string;

#ifdef _OS_FREEBSD_
			if ((image_name.contains("/") && (!image_name.contains(".iso", Qt::CaseInsensitive)) && (!image_name.contains(".nrg", Qt::CaseInsensitive)))) {
				mount_string=this->getSetting("quickmount", "mount_drive_string", false).toString();
#ifdef DEBUG
				qDebug()<<"[ii] corelib::mountImage:FreeBSD drive mount base string: "<<mount_string;
#endif
				mount_string.replace("%MOUNT_DRIVE%", this->getEscapeString(image_name));
			} else {
				mount_string=this->getSetting("quickmount", "mount_image_string", false).toString();
#ifdef DEBUG
				qDebug()<<"[ii] corelib::mountImage:FreeBSD image mount base string: "<<mount_string;
#endif

				if (!QFile(image_name).exists()){
                                        mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(db_image.getPath(image_name)));
				} else {
					mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(image_name));
				}
                                mount_string.replace("%MDCONFIG_BIN%", this->getWhichOut("mdconfig"));
			}

			mount_string.replace("%GUI_SUDO%", getSetting("system", "gui_sudo").toString());
			mount_string.replace("%SUDO%", getSetting("system", "sudo").toString());
			mount_string.replace("%MOUNT_BIN%", getSetting("system", "mount").toString());
			mount_string.replace("%MOUNT_POINT%", this->getEscapeString(mount_point));
#endif

#ifdef _OS_LINUX_
			if ((image_name.contains("/") && (!image_name.contains(".iso", Qt::CaseInsensitive)) && (!image_name.contains(".nrg", Qt::CaseInsensitive)))) {
				mount_string=this->getSetting("quickmount", "mount_drive_string", false).toString();
#ifdef DEBUG
				qDebug()<<"[ii] corelib::mountImage:Linux drive mount base string: "<<mount_string;
#endif
				mount_string.replace("%MOUNT_DRIVE%", this->getEscapeString(image_name));
			} else {
				mount_string=this->getSetting("quickmount", "mount_image_string", false).toString();
#ifdef DEBUG
				qDebug()<<"[ii] corelib::mountImage:Linux image mount base string: "<<mount_string;
#endif

				if (!QFile(image_name).exists()){
					mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(this->db_image.getPath(image_name)));
				} else {
					mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(image_name));
				}

				if (image_name.right(3)=="nrg"){
					mount_string.replace("%MOUNT_OPTIONS%", "-o  loop,offset=307200");
				} else {
					mount_string.replace("%MOUNT_OPTIONS%", "-o  loop");
				}
			}

			mount_string.replace("%GUI_SUDO%", getSetting("system", "gui_sudo").toString());
			mount_string.replace("%SUDO%", getSetting("system", "sudo").toString());
			mount_string.replace("%MOUNT_BIN%", getSetting("system", "mount").toString());
			mount_string.replace("%MOUNT_POINT%", this->getEscapeString(mount_point));
#endif

			args.clear();
			args.append("-c");
			args.append(mount_string);

#ifdef DEBUG
			qDebug()<<"[ii] corelib::mountImage: mount args: "<<args;
#endif
			return this->runProcess(args, QObject::tr("Mounting..."),  QObject::tr("Mounting %1 into %2").arg(image_name).arg(mount_point));
		}

		bool corelib::umountImage(const QString prefix_name) const{
			QString mount_point=db_prefix.getFieldsByPrefixName(prefix_name).at(6);

                        if (this->getMountedImages(mount_point)=="none")
                            return true;

#ifdef DEBUG
			qDebug()<<"[ii] corelib::umountImage: mount point: "<<mount_point;
#endif
			if (mount_point.isEmpty()){
				this->showError(QObject::tr("It seems no mount point was set in prefix options.<br>You might need to set it manualy."));
				return false;
			}

			QStringList args;
			QString arg;

			QString mount_string;
			mount_string=this->getSetting("quickmount", "umount_string", false).toString();
#ifdef DEBUG
			qDebug()<<"[ii] corelib::umountImage: umount string: "<<mount_string;
#endif
			mount_string.replace("%GUI_SUDO%", getSetting("system", "gui_sudo").toString());
			mount_string.replace("%SUDO%", getSetting("system", "sudo").toString());
			mount_string.replace("%UMOUNT_BIN%", getSetting("system", "umount").toString());
			mount_string.replace("%MOUNT_POINT%", this->getEscapeString(mount_point));

			args.clear();
			args.append("-c");
			args.append(mount_string);
#ifdef DEBUG
			qDebug()<<"[ii] corelib::umountImage: umount args: "<<args;
#endif

			return this->runProcess(args, QObject::tr("Umounting..."), QObject::tr("Umounting point: %1").arg(mount_point));

			return true;
		}

		bool corelib::runProcess(const QStringList args, const QString caption, const QString message) const{

#ifdef DEBUG
			qDebug()<<"[ii] corelib::runProcess: args: "<<args;
#endif

			if (this->_GUI_MODE){
				Process proc(args, this->getSetting("system", "sh").toString(), QDir::homePath(), message, caption, false);
				return (proc.exec());
			} else {
				return (this->runProcess(this->getSetting("system", "sh").toString(), args));
			}
			return true;
		}

		bool corelib::openIconDirectry(const QString prefix_name, const QString dir_name, const QString icon_name){
			QStringList result = db_icon.getByName(prefix_name, dir_name, icon_name);
			QStringList args;
			args<<result.at(4);
			return this->runProcess(this->getWhichOut("xdg-open"), args, "", false);
		}

		bool corelib::openPrefixDirectry(const QString prefix_name){
			QStringList args;
			args<<db_prefix.getPath(prefix_name);
			return this->runProcess(this->getWhichOut("xdg-open"), args, "", false);
		}

		QString corelib::getWinePath(const QString path, const QString option) {
			QString output, exec;
			QStringList args;

			args.append(option);
			args.append(path);
			exec = this->getWhichOut("winepath");

			QProcess myProcess;
			myProcess.setEnvironment(QProcess::systemEnvironment());
			myProcess.setWorkingDirectory (QDir::homePath());
			myProcess.start(exec, args);

			if (myProcess.waitForFinished()){
				output = myProcess.readAllStandardOutput().trimmed();
			}
			return output;
		}

		bool corelib::runProcess(const QString exec, const QStringList args, QString dir, bool showLog) const{
			if (dir.isEmpty())
				dir=QDir::homePath();

			QProcess myProcess;
			myProcess.setEnvironment(QProcess::systemEnvironment());
			myProcess.setWorkingDirectory (dir);

#ifdef DEBUG
			qDebug()<<"[ii] corelib::runProcess"<<exec<<args;
#endif

			myProcess.start(exec, args);

			if (!myProcess.waitForFinished())
				return false;

			if (showLog){
				// Getting env LANG variable
				QString lang=getenv("LANG");
				lang=lang.split(".").at(1);

				// If in is empty -- set UTF8 locale
				if (lang.isEmpty())
					lang = "UTF8";

				// Read STDERR with locale support
				QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
				QString string = codec->toUnicode(myProcess.readAllStandardError());

				if (!string.isEmpty()){
					showError(QObject::tr("It seems the process crashed. STDERR log: %1").arg(string));
					delete (&codec);
					return false;
				}
				delete (&codec);
			}
			return true;
		}

		int corelib::showError(const QString message, const bool info) const{
			switch (this->_GUI_MODE){
   case true:
				switch (info){
	case true:
					QMessageBox::warning(0, QObject::tr("Error"), message);
					return 0;
					break;
	case false:
					return QMessageBox::warning(0, QObject::tr("Error"), message, QMessageBox::Retry, QMessageBox::Ignore);
					break;
				}
				break;
			case false:
				QTextStream stdErr(stderr);
				stdErr<<"[ee] "<<message<<endl; // message.toLatin1();
				break;
			}
			return 0;
		}

		void corelib::showError(const QString message) const{
			QTextStream Qcout(stdout);
			switch (this->_GUI_MODE){
   case true:
				QMessageBox::warning(0, QObject::tr("Error"), message);
				break;
   case false:
				Qcout<<QObject::tr("Error")<<endl<<message<<endl;
				break;
			}
			return;
		}

		bool corelib::killWineServer(const QString prefix_path) const{
			QString command;

			if (!prefix_path.isEmpty()){
				command=QString("env WINEPREFIX=\"%1\" wineserver -kill").arg(prefix_path);
			} else {
				command="wineserver -kill";
			}


			if (system(command.toAscii().data())==-1){
				this->showError(QObject::tr("Can't run: %1").arg(command.toAscii().data()));
				return false;
			}

			return true;
		}

		void corelib::openHelpUrl(const QString rawurl){

			QString lang = this->getSetting("", "", false).toString();
			if (lang.isEmpty()){
				lang = setlocale(LC_ALL, "");
				if (lang.isEmpty()){
					lang = setlocale(LC_MESSAGES, "");
					if (lang.isEmpty()){
						lang = getenv("LANG");
					}
				}
				lang = lang.split(".").at(0).toLower();
			}

			if ((lang!="en_us") and (lang!="ru_ru") and (lang!="uk_ua"))
				lang = "en_us";

			QString url="http://";
			url.append(APP_WEBSITTE);
			url.append("/documentation/");
			url.append(lang);
			url.append("/");
			url.append(rawurl);

			QStringList args;
			args<<url;
			this->runProcess(this->getWhichOut("xdg-open"), args, "", false);
			return;
		}

		void corelib::openHomeUrl(const QString rawurl){
			QString url="http://";
			url.append(APP_WEBSITTE);
			url.append("/");
			url.append(rawurl);

			QStringList args;
			args<<url;
			this->runProcess(this->getWhichOut("xdg-open"), args, "", false);
			return;
		}

		void corelib::openUrl(const QString rawurl){
			QStringList args;
			args<<rawurl;
			this->runProcess(this->getWhichOut("xdg-open"), args, "", false);
			return;
		}

		QString corelib::getMountString(const int profile){
			QString string;
			switch (profile){
   case 0:
#ifdef _OS_LINUX_
				string="%SUDO% %MOUNT_BIN% %MOUNT_DRIVE% %MOUNT_POINT%";
#endif
#ifdef _OS_FREEBSD_
				string="%SUDO% %MOUNT_BIN% -t cd9660 %MOUNT_DRIVE% %MOUNT_POINT%";
#endif
				break;
   case 1:
#ifdef _OS_LINUX_
				string="%GUI_SUDO% \"%MOUNT_BIN% %MOUNT_DRIVE% %MOUNT_POINT%\"";
#endif
#ifdef _OS_FREEBSD_
				string="%GUI_SUDO% \"%MOUNT_BIN% -t cd9660 %MOUNT_DRIVE% %MOUNT_POINT%\"";
#endif
				break;
   case 2:
				string=getWhichOut("fuseiso");
				string.append(" %MOUNT_DRIVE% %MOUNT_POINT%");
				break;
   case 3:
				string=APP_PREF;
				string.append("/bin/q4wine-mount %MOUNT_DRIVE% %MOUNT_POINT%");
				break;
			}

			return string;
		}

		void corelib::updateRecentImagesList(const QString media) const{
			QSettings settings(APP_SHORT_NAME, "default");
			QStringList files = settings.value("recent_images").toStringList();
			files.removeAll(media);
			files.prepend(media);
			while (files.size() > 8)
				files.removeLast();

			settings.setValue("recent_images", files);
			return;
		}

		QString corelib::getMountImageString(const int profile){
			QString string;
			switch (profile){
   case 0:
#ifdef _OS_LINUX_
				string = "%SUDO% %MOUNT_BIN% %MOUNT_OPTIONS% %MOUNT_IMAGE% %MOUNT_POINT%";
#endif
#ifdef _OS_FREEBSD_
				string="";
				string.append(APP_PREF);
				string.append("/share/q4wine/scripts/mount_image.sh");
				string.append(" %MOUNT_IMAGE% %MOUNT_POINT%");
#endif
				break;
   case 1:
#ifdef _OS_LINUX_
				string = "%GUI_SUDO% \"%MOUNT_BIN% %MOUNT_OPTIONS% %MOUNT_IMAGE% %MOUNT_POINT%\"";
#endif
#ifdef _OS_FREEBSD_
				string="";
				string.append(APP_PREF);
				string.append("/share/q4wine/scripts/mount_image.sh");
				string.append(" %MOUNT_IMAGE% %MOUNT_POINT%");
#endif
				break;
   case 2:
				string=getWhichOut("fuseiso");
				string.append(" %MOUNT_IMAGE% %MOUNT_POINT%");
				break;
   case 3:
				string=APP_PREF;
				string.append("/bin/q4wine-mount %MOUNT_IMAGE% %MOUNT_POINT%");
				break;
			}
			return string;
		}

		QString corelib::getUmountString(const int profile){
			QString string;
			switch (profile){
   case 0:
#ifdef _OS_LINUX_
				string = "%SUDO% %UMOUNT_BIN% %MOUNT_POINT%";
#endif
#ifdef _OS_FREEBSD_
				string="";
				string.append(APP_PREF);
				string.append("/share/q4wine/scripts/umount.sh");
				string.append(" %MOUNT_POINT%");
#endif
				break;
   case 1:
#ifdef _OS_LINUX_
				string="%GUI_SUDO% %UMOUNT_BIN% %MOUNT_POINT%";
#endif
#ifdef _OS_FREEBSD_
				string = "";
				string.append(APP_PREF);
				string.append("/share/q4wine/scripts/umount.sh");
				string.append(" %MOUNT_POINT%");
#endif
				break;
   case 2:
				string=getWhichOut("fusermount");
				string.append(" -u %MOUNT_POINT%");
				break;
   case 3:
				string=getWhichOut("fusermount");
				string.append(" -u %MOUNT_POINT%");
				break;
			}
			return string;
		}

		bool corelib::reniceProcess(const int pid, const int priority) const{
			QStringList args;
			args << this->getSetting("system", "renice").toString();
			args.append(QString("%1").arg(priority));
			args.append(QString("%1").arg(pid));

			QString sudobin = this->getSetting("system", "gui_sudo").toString();

			if (!sudobin.contains(QRegExp("/sudo$"))){
				QString arg =args.join(" ");
				args.clear();
				args.append(arg);
			}

			if (this->runProcess(this->getSetting("system", "gui_sudo").toString(), args, QDir::homePath(), false)){
				return true;
			} else {
				return false;
			}
		}

		void corelib::runAutostart(void){
			QList<QStringList> iconsList, prefixList;

			prefixList = db_prefix.getFields();
			for (int i = 0; i < prefixList.size(); ++i) {
				iconsList = db_icon.getByPrefixAndDirName(prefixList.at(i).at(1), "autostart");
				for (int j = 0; j < iconsList.size(); ++j) {
					this->runIcon(prefixList.at(i).at(1), "autostart", iconsList.at(j).at(1));
				}
			}
			return;
		}
