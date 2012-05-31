/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                           *
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
    this->mdconfig="";
    this->fuseiso="";
    this->fusermount="";
}

corelib* createCoreLib(bool _GUI_MODE){
    return new corelib(_GUI_MODE);
}

QList<QStringList> corelib::getWineProcessList(const QString prefix_name){
    QList<QStringList> proclist;
    QStringList procline;


    QString prefix_path;

    if (!prefix_name.isEmpty())
        prefix_path = db_prefix.getPath(prefix_name);

    QString name, procstat, path, prefix, env_arg, nice, message;

#ifdef _OS_LINUX_
    message = "<p>Process is unable access to /proc file system.</p><p>Access is necessary for displaying wine process information.</p><p>You need to set CONFIG_PROC_FS=y option on linux kernel config file and mount proc file system by running: mount -t proc none /proc</p>";
#endif

#ifdef _OS_FREEBSD_
    message = "<p>Process is unable access to /proc file system.</p><p>Access is necessary for displaying wine process information.</p><p>You need to set PSEUDOFS and PROCFS option on FreeBSD kernel config file and mount proc file system by running: mount -t procfs proc /proc</p>";
#endif

#ifndef _OS_DARWIN_
    // Check for /proc directory exists
    QDir dir("/proc");
    if (!dir.exists()){
        if (this->showError(message, false) == QMessageBox::Ignore){
        //    procline << "-1";
        //    proclist << procline;
            proclist.clear();
            return proclist;
        }
    }
#endif
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
        if (exelink.isSymLink() && (exelink.symLinkTarget().contains("wineserver") || exelink.symLinkTarget().contains("wine-preloader") || exelink.symLinkTarget().contains("wine64-preloader"))){
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


                    if (!prefix_path.isNull()){
                        if (prefix_path == prefix){
                            procline << fileInfo.fileName() << name << nice << prefix;
                            proclist << procline;
                        }
                    } else {
                        procline << fileInfo.fileName() << name << nice << prefix;
                        proclist << procline;
                    }

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
#ifdef DEBUG
                    qDebug()<< "kvm_getargv failed: " << kvm_geterr(kd);
#endif
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
                                     prefix = QObject::tr("Can't read process info from /proc or API is changed.");
#ifdef DEBUG
                                     qDebug()<< "kvm_getenvv failed: " << kvm_geterr(kd);
#endif
                }

                procline.clear();
                // Puting all fields into QList<QStringList>
                if (!prefix_path.isNull()){
                    if (prefix_path == prefix){
                        procline << QString("%1").arg(ipid) << name << nice << prefix;
                        proclist << procline;
                    }
                } else {
                    procline << QString("%1").arg(ipid) << name << nice << prefix;
                    proclist << procline;
                }
            }
        }
    }

    kvm_close(kd);
#endif

#ifdef _OS_DARWIN_
    process_list *p_list = getProcessList();

    if (p_list->pnum > 0){
        for (unsigned int i=0; i<p_list->pnum; i++){
            QString pid = QString("%1").arg(p_list->procs[i]->pid);
            QString nice = QString("%1").arg(p_list->procs[i]->nice);
            QString name = QString("%1").arg(p_list->procs[i]->name);
            QStringList args = QString("%1").arg(p_list->procs[i]->args).split(" ");

            free(p_list->procs[i]->name);
            free(p_list->procs[i]->args);

            QString prefix = "";
            foreach (const QString &str, args) {
                int index = str.indexOf("WINEPREFIX=");
                if (index>=0){
                    prefix=str.mid(11);
                    break;
                }
            }

            if (name == "wine.bin"){
                foreach (const QString &str, args) {
                    int index = str.indexOf(".exe");
                    if (index>=0){
                        name=str.split("/").last().split("\\").last();
                        break;
                    }
                }
            }

            procline.clear();
            if (!prefix_path.isNull()){
                if (prefix_path == prefix){
                    procline << pid << name << nice << prefix;
                    proclist << procline;
                }
            } else {
                procline << pid << name << nice << prefix;
                proclist << procline;
            }
        }
        free(p_list->procs);
        free(p_list);
    }
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
    QString lib32 = this->getSetting("wine", "WineLibs32", false, "").toString();
    QString lib64 = this->getSetting("wine", "WineLibs64", false, "").toString();
    if (lib32.isEmpty() and lib64.isEmpty()){
        QStringList libs_path = this->getWineLibsPath();
        QSettings settings(APP_SHORT_NAME, "default");
        settings.beginGroup("wine");
        settings.setValue("WineLibs32", libs_path.at(0));
        settings.setValue("WineLibs64", libs_path.at(1));
        settings.endGroup();
    }

    lib32 = this->getSetting("wine", "WineLibs32", false, "").toString();
    lib64 = this->getSetting("wine", "WineLibs64", false, "").toString();
    if (lib32.isEmpty() and lib64.isEmpty()){
        this->showError(QObject::tr("<p>Error while loading application settings. wine's library path for 32 and 64 bit is not set.</p><p>Please, go to %1 options dialog and set at least one path.</p>").arg(APP_SHORT_NAME));

    }

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
            settings.setValue("type", 0);
            settings.setValue("mount_drive_string", this->getMountString(0));
            settings.setValue("mount_image_string", this->getMountImageString(0));
            settings.setValue("umount_string", this->getUmountString(0));
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

QString corelib::getTranslationLang(){
    QTranslator qtt;

#ifdef RELEASE
    #ifdef _OS_DARWIN_
        QString i18nPath = QString("%1/%2.app/Contents/i18n").arg(QDir::currentPath()).arg(APP_SHORT_NAME);
    #else
        QString i18nPath = QString("%1/share/%2/i18n").arg(APP_PREF).arg(APP_SHORT_NAME);
    #endif
#else
    QString i18nPath = QString("%1/i18n").arg(APP_BUILD);
#endif



#ifdef DEBUG
    qDebug()<<"[ii] i18n path: "<<i18nPath;
#endif

    QString lang = "q4wine_";
    lang.append(this->getLang());

    if (!lang.isNull()){
        if (qtt.load(lang, i18nPath)){
            qDebug()<<"[ii] loaded:"<<lang;
            return lang;
        } else {
            qDebug()<<"[EE] Can't open user selected translation";
            if (qtt.load("en_us", i18nPath)){
                return "en_us";
            } else {
                qDebug()<<"[EE] Can't open default translation, fall back to native translation ;[";
            }
        }
    } else {
        qDebug()<<"[EE] Can't get LANG variable, fall back to native translation ;[";
    }
    return "";
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
        qDebug()<<"[ii] loc.count(): "<<loc.count();
#endif
        if (loc.count()==2){
            lang = loc.at(1).toLower();
        } else {
            lang = "utf8";
        }

        if (lang.contains(";"))
            lang = lang.split(";").first();

#ifdef DEBUG
        qDebug()<<"[ii] Locale to use: "<<lang;
#endif
        return lang;
}

bool corelib::isConfigured(){
    if (this->getSetting("", "configure", false, "").toString()=="yes")
        return true;
    return false;
}

bool corelib::checkDirs(){
    QStringList subDirs;
    subDirs << "" << "db" << "icons" << "prefixes" << "tmp" << "theme" << "tmp/cache" << "scripts";

    QTextStream QErr(stderr);
    QDir dir;
    QString rootConfPath = QString("%1/.config/%2").arg(QDir::homePath()).arg(APP_SHORT_NAME);

    for (int i=0; i<subDirs.size(); ++i){
        QString subDir=rootConfPath;
        subDir.append("/");
        subDir.append(subDirs.at(i).toLocal8Bit().constData());

#ifdef DEBUG
        qDebug()<<"[ii] Check for directory: "<<subDir;
#endif

        if (!dir.exists(subDir)){
            if (!dir.mkdir(subDir)){
                QErr<<"[EE] "<<"Unable to create directory "<<subDir;
                return false;
            }
        }
    }

    return true;
}

void corelib::getBuildFlags(){
    QTextStream Qcout(stdout);
    Qcout<<QObject::tr("Buildtime flags are:")<<endl<<endl;
    Qcout<<qSetFieldWidth(25)<<left<<" CMAKE_INSTALL_PREFIX"<<APP_PREF<<qSetFieldWidth(0)<<endl<<endl;
#ifdef RELEASE
    Qcout<<qSetFieldWidth(25)<<left<<" RELEASE"<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" RELEASE"<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
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
#ifdef WITH_SYSTEM_SINGLEAPP
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_SYSTEM_SINGLEAPP"<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_SYSTEM_SINGLEAPP"<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
#ifdef WITH_WINEAPPDB
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_WINEAPPDB "<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_WINEAPPDB "<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
#ifdef WITH_DBUS
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_DBUS"<<"ON"<<qSetFieldWidth(0)<<endl;
#else
    Qcout<<qSetFieldWidth(25)<<left<<" WITH_DBUS"<<"OFF"<<qSetFieldWidth(0)<<endl;
#endif
    Qcout<<endl;
}

QString corelib::getWhichOut(const QString fileName, bool showErr){
    /*
   * Getting 'which' output;
   */

    if  (fileName=="mdconfig"){
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
        if (fileName=="mdconfig"){
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
            this->showError(QObject::tr("Can't find or execute the '%1' binary. Make sure that this binary is available by search PATH variable and see also INSTALL file for application depends.").arg(fileName));
    }

    return "";
}

QStringList corelib::getWineLibsPath(void) {
    QProcess proc;
    QStringList args;
    QStringList libs_path;
    libs_path << "" << ""; // 32, 64

#ifndef _OS_MACOSX_
    QString sh = this->getWhichOut("sh");
    if (sh.isEmpty())
        return libs_path;

    QString ldconfig = this->getWhichOut("ldconfig", false);
    if (ldconfig.isEmpty())
        ldconfig = "/sbin/ldconfig";

#ifdef _OS_LINUX_
    ldconfig.append(" -p ");
#else
    ldconfig.append(" -r ");
#endif
    ldconfig.append(" | grep libwine.so");

    args<<"-c"<<ldconfig;

    proc.setWorkingDirectory (QDir::homePath());
    proc.start(sh, args, QIODevice::ReadOnly);
    proc.waitForFinished();

    if (proc.exitCode() != 0){
        qDebug()<<"[EE] Can't get ldconfig data";
        return libs_path;
    }

    QString string = proc.readAllStandardOutput();
#ifdef DEBUG
    qDebug()<<"[ii]"<<string;
#endif
    if (!string.isEmpty()){
        QStringList libs_list = string.split("\n");
#ifdef _OS_LINUX_
        QRegExp rx_info(".*\\((.*)\\) => (.*\\.so$)");
#else
        QRegExp rx_info("(.*) => (.*\\.so.*)");
#endif
        foreach (QString lib_info, libs_list){
            int pos = rx_info.indexIn(lib_info);
            QString arch, path;
            if (pos > -1) {
                arch = rx_info.cap(1).split(",").last();
                path = rx_info.cap(2);
            } else {
                continue;
            }
            if (path.isEmpty()){
                qDebug()<<"[EE] No winelib.so found in: " << lib_info;
                continue;
            }
            QFileInfo file(path);
            if (!file.exists()){
                qDebug()<<"[EE] File not exists: " << path;
                continue;
            }
            path = file.absolutePath().append("/wine/");
            file.setFile(path);
            if (!file.isDir()){
                qDebug()<<"[EE] Directory not exists:"<<path;
                continue;
            } else {
                if (arch == "x86-64"){
                    libs_path.replace(1, path);
                } else {
                    libs_path.replace(0, path);
                }
            }
        }
    }
#else
    //FIXME: Design search for brew packages: /usr/local/Cellar/wine/1.2.3/lib
    QStringList libs_loc, libs_arch;
    libs_loc << "/usr/lib" << "/usr/local/lib" << "/local/usr/lib" << "/opt/local/lib";
    libs_arch << "64" << "32" << "";
    foreach (QString loc, libs_loc){
        foreach (QString arch, libs_arch){
            loc.append(arch);
            QString libwine_path = loc;
            libwine_path.append("/libwine.dylib");
            if (QFile(libwine_path).exists()){
                loc.append("/wine");
                libs_path.replace(0, loc);
                libs_path.replace(1, loc);
                break;
            }
        }
    }
#endif
    return libs_path;
}

QStringList corelib::getCdromDevices(void) const{
    QStringList retVal;
#ifdef _OS_DARWIN_
    return retVal;
#endif

    QDir dir("/dev/");
    dir.setFilter(QDir::Files | QDir::System);
    dir.setSorting(QDir::Name);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

#if defined(_OS_LINUX_)
        if (fileInfo.fileName().contains(QRegExp("^cdrom")) or
            fileInfo.fileName().contains(QRegExp("^sr")) or
            fileInfo.fileName().contains(QRegExp("^dvd"))){
#elif defined(_OS_FREEBSD_)
        if (fileInfo.fileName().contains(QRegExp("^cdrom")) or
            fileInfo.fileName().contains(QRegExp("^cd")) or
            fileInfo.fileName().contains(QRegExp("^acd")) or
            fileInfo.fileName().contains(QRegExp("^dvd"))){
#elif defined(_OS_DARWIN_)
        if (fileInfo.fileName().contains(QRegExp("^cdrom")) or
            fileInfo.fileName().contains(QRegExp("^cd")) or
            fileInfo.fileName().contains(QRegExp("^acd")) or
            fileInfo.fileName().contains(QRegExp("^dvd"))){
#else
        if (fileInfo.fileName().contains(QRegExp("^cdrom")) or
            fileInfo.fileName().contains(QRegExp("^sr")) or
            fileInfo.fileName().contains(QRegExp("^dvd"))){
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

    QChar corelib::getCdromWineDrive(QString prefix_path, QString mount_point){
        QDir wineDriveDir;
        wineDriveDir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot  );

        prefix_path.append("/dosdevices/");
        if (!wineDriveDir.cd(prefix_path)){
            qDebug()<<"[EE] Cannot cd to prefix directory: "<<prefix_path;
        } else {
            QFileInfoList drivelist = wineDriveDir.entryInfoList();
            for (int i = 0; i < drivelist.size(); ++i) {
                QFileInfo fileInfo = drivelist.at(i);
                if (fileInfo.symLinkTarget() == mount_point){
                    return fileInfo.fileName()[0];
                }
            }
        }

        return QChar();
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

    QString corelib::getMountedImages(QString cdrom_mount){

        if (cdrom_mount.right(1)=="/"){
            cdrom_mount=cdrom_mount.left(cdrom_mount.length()-1);
        }

        QString image="none";
        QStringList arguments;

#ifdef DEBUG
        qDebug()<<"corelib::getMountedImages("<<cdrom_mount<<")";
#endif


#ifdef _OS_LINUX_
        QString filename="/etc/mtab";
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
#ifdef DEBUG
                qDebug()<<"corelib::/etc/mtab:line"<<line;
#endif
                if (line.contains(cdrom_mount)){
                    image = line.split(" ").first();
                    if (image.contains("fuseiso") || image.contains("q4wine-mount")){
#ifdef DEBUG
                        qDebug()<<"corelib::fuseiso image found: "<<image;
#endif
                        filename=QDir::homePath();
                        filename.append("/.mtab.fuseiso");
                        QFile file(filename);
                        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                            QTextStream in(&file);
                            while (!in.atEnd()) {
                                QString line = in.readLine();
#ifdef DEBUG
                                qDebug()<<"corelib::getMountedImages:line"<<line;
#endif
                                if (line.contains(cdrom_mount))
                                    return line.split(" ").first().split("/").last();
                            }
                        } else {
#ifdef DEBUG
                            qDebug()<<"corelib::fuseiso cant read mtab.fuseiso"<<image;
#endif
                            return QString("cant read %1").arg(filename);
                        }
                    } else if (image.contains("loop")){
                        //FIXME: find a better solution
                        // I knew it is a horrible dirty hack, but I can't find any other way to get information from loop device.
                        // If some one knew how to fix it, feel free to mail solution to me :]
                        if (!this->getSetting("system", "sudo").toString().isEmpty()){
                            arguments << "losetup" << image;
                            QProcess myProcess;
                            myProcess.start(this->getSetting("system", "sudo").toString(), arguments);
                            if (!myProcess.waitForFinished()){
                                qDebug() << "Make failed:" << myProcess.errorString();
                                return QString("can't run %1").arg(arguments.at(0));
                            } else {
                                image = myProcess.readAll();
                                qDebug()<<"[ii] loop: "<<arguments;
                                return image.split("/").last().mid(0, image.split("/").last().length()-2);
                            }
                        }
                    } else {
                        return image;
                    }
                }
            }
        } else {
            return "cant read /etc/mtab";
        }

        return "none";
#endif
#ifdef _OS_FREEBSD_
        arguments << "-c" << QString("%1 | grep \"%2\"").arg(this->getSetting("system", "mount").toString()).arg(cdrom_mount);

        QProcess myProcess;
        myProcess.start(this->getSetting("system", "sh").toString(), arguments);
        if (!myProcess.waitForFinished()){
            qDebug() << "Make failed:" << myProcess.errorString();
            return QString();
        }

        image = myProcess.readAll();

#ifdef DEBUG
        qDebug()<<"corelib::getMountedImages:image"<<image;
#endif

        if (!image.isEmpty()){
            image = image.split(" ").first();
            if (!image.isEmpty()){
                    if (image.contains("md")){
                        myProcess.close ();
                        arguments.clear();
                        arguments << "mdconfig" <<  "-l" << QString("-u%1").arg(image.mid(7));
                        myProcess.start(this->getSetting("system", "sudo").toString(), arguments);
                        if (!myProcess.waitForFinished()){
                            qDebug() << "Make failed:" << myProcess.errorString();
                            return QString();
                        } else {
                            image = myProcess.readAll();
                            image = image.split("/").last().mid(0, image.split("/").last().length()-1);
                        }
                    } else if (image.contains("fuseiso") || image.contains("q4wine-mount")){
#ifdef DEBUG
        qDebug()<<"corelib::getMountedImages fuseiso sub"<<image;
#endif
                        QString filename;
                        filename=QDir::homePath();
                        filename.append("/.mtab.fuseiso");
                        QFile file(filename);
                        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                            QTextStream in(&file);
                            while (!in.atEnd()) {
                                QString line = in.readLine();
#ifdef DEBUG
        qDebug()<<"corelib::getMountedImages:line"<<line;
#endif
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
#endif

#ifdef _OS_DARWIN_

        arguments << "-c" << "hdiutil info";

        QProcess myProcess;
        myProcess.start(this->getSetting("system", "sh").toString(), arguments);
        if (!myProcess.waitForFinished()){
            qDebug() << "Make failed:" << myProcess.errorString();
            return QString();
        }

        QString buffer;
        buffer = myProcess.readAll();

#ifdef DEBUG
        //qDebug()<<"corelib::getMountedImages info"<<image;
#endif

        QStringList lines = buffer.split("\n");

        QRegExp rx_image("^image-path      : (.*)$");
        QRegExp rx_mount("^/dev/disk.*(/.*)$");



        for (int i=0; i < lines.size(); i++){
            if (rx_image.indexIn(lines.at(i)) != -1) {
                buffer = rx_image.cap(1);
            }
            if (rx_mount.indexIn(lines.at(i)) != -1) {
                if (rx_mount.cap(1) == cdrom_mount){
                    image = buffer.split("/").last();
                    break;
                }
            }
        }


        /* plist parse info
        QSettings settings("/Users/brezerk/test.plist", QSettings::NativeFormat);
        QVariantList lol = settings.value("images").toList();

        qDebug()<<lol.size();
        for (int i = 0; i < lol.size(); i++) {
            QString point = lol.at(i).toMap()["system-entities"].toMap()["mount-point"].toString();
            if (point == cdrom_mount){
                image = lol.at(i).toMap()["image-path"].toString().split(" ").first().split("/").last();
                break;
            }
        }
        */

#endif

            return image;
        }

        bool corelib::runIcon(const QString prefix_name, const QString dir_name, const QString icon_name){
            QHash<QString, QString> result = db_icon.getByName(prefix_name, dir_name, icon_name);
            ExecObject execObj;
            execObj.wrkdir = result.value("wrkdir");
            execObj.override = result.value("override");
            execObj.winedebug = result.value("winedebug");
            execObj.useconsole = result.value("useconsole");
            execObj.display = result.value("display");
            execObj.cmdargs = result.value("cmdargs");
            execObj.execcmd = result.value("exec");
            execObj.desktop = result.value("desktop");
            execObj.nice = result.value("nice");
            execObj.name = icon_name;
            execObj.lang = result.value("lang");
            execObj.prerun = result.value("prerun");
            execObj.postrun = result.value("postrun");

            return runWineBinary(execObj, prefix_name);
        }

        bool corelib::checkFileExists(QString path){
            QString u_path;

            if (path.length()<=0){
                /* if (this->_GUI_MODE){
                    qDebug()<<"[EE] No binary passed sSsssssssssssSSSSSSSSSSSs ============"<<path;
                    QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("No binary passed to function"));
                } else {
                    qDebug()<<"[EE] No binary passed";
                } */
                return true;
            }

            if (path.mid(0,1)=="/"){
                if (!QFile(path).exists()){
                    if (this->_GUI_MODE){
                        QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("Binary file \"%1\" do not exists.").arg(path));
                    } else {
                        qDebug()<<"[EE] Binary \""<<path<<"\" do not exists. Abort.";
                    }
                    return false;
                }
            } else if (path.mid(1,2)==":\\"){
                u_path = this->getWinePath(path, "-u");
                if (u_path.isEmpty()){
                    if (this->_GUI_MODE){
                        QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("Can't get unix path for \"%1\".").arg(path));
                    } else {
                        qDebug()<<"[EE] Binary \""<<path<<"\" do not exists. Abort.";
                    }
                    return false;
                } else {
                    if (!QFile(u_path).exists()){
                        if (this->_GUI_MODE){
                            QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("Binary file \"%1\" do not exists.").arg(u_path));
                        } else {
                            qDebug()<<"[EE] Binary \""<<u_path<<"\" do not exists. Abort.";
                        }
                        return false;
                    }
                }
            }
            return true;
        }

        bool corelib::runWineBinary(const ExecObject execObj, QString prefix_name, bool detach){
#ifdef RELEASE
    #ifdef _OS_DARWIN_
            QString binary = QString("%1/%2.app/Contents/MacOS/q4wine-helper").arg(QDir::currentPath(), APP_SHORT_NAME);
    #else
            QString binary = QString("%1/bin/q4wine-helper").arg(APP_PREF);
    #endif
#else
    #ifdef _OS_DARWIN_
            QString binary = QString("%1/q4wine-helper/q4wine-helper.app/Contents/MacOS/q4wine-helper").arg(APP_BUILD);
    #else
            QString binary = QString("%1/q4wine-helper/q4wine-helper").arg(APP_BUILD);
    #endif
#endif
            QStringList args;

            args.append("--prefix");
            args.append(prefix_name);

            if (execObj.nice>0){
                args.append("--nice");
                args.append(execObj.nice);
            }

            if (!execObj.desktop.isEmpty()){
                args.append("--desktop");
                args.append(execObj.desktop);
            }

            if (!execObj.override.isEmpty()){
                args.append("--override");
                args.append(QString("\"%1\"").arg(execObj.override));
            }

            if (execObj.useconsole == "1"){
                args.append("--console");
                args.append(execObj.useconsole);
            }

            if (!execObj.winedebug.isEmpty()){
                args.append("--wine-debug");
                args.append(execObj.winedebug);
            }

            if (!execObj.display.isEmpty()){
                args.append("--display");
                args.append(execObj.display);
            }

            if (!execObj.cmdargs.isEmpty()){
                args.append("--program-args");
                args.append(execObj.cmdargs);
            }

            if (!execObj.execcmd.isEmpty()){
                args.append("--program-bin");
                args.append(execObj.execcmd);
            }

            if (!execObj.lang.isEmpty()){
                args.append("--program-lang");
                args.append(execObj.lang);
            }

            if (!execObj.prerun.isEmpty()){
                args.append("--prerun");
                args.append(QString("\"%1\"").arg(execObj.prerun));
            }

            if (!execObj.postrun.isEmpty()){
                args.append("--postrun");
                args.append(QString("\"%1\"").arg(execObj.postrun));
            }

            QString wrkdir = execObj.wrkdir;

            if (wrkdir.isEmpty())
                wrkdir = QDir::currentPath();

            args.append("--wrkdir");
            args.append(wrkdir);

#ifdef DEBUG
            qDebug()<<"[ii] corelib::runWineBinary: "<<binary<<args<<" at: "<<wrkdir;
#endif

            if (detach){
                QProcess proc(0);
                return proc.startDetached(binary, args, QDir::currentPath());
            } else {
                Process proc(args, binary, QDir::currentPath(), QObject::tr("Running binary: \"%1\"").arg(execObj.execcmd), QObject::tr("Running binary..."), false);
                return proc.exec();
            }

                  return false;
        }

        QString corelib::createDesktopFile(const QString prefix_name, const QString dir_name, const QString icon_name) const{
            QHash<QString, QString> result = db_icon.getByName(prefix_name, dir_name, icon_name);

            QString fileName = QDir::homePath();
            fileName.append("/.config/");
            fileName.append(APP_SHORT_NAME);
            fileName.append("/tmp/");
            fileName.append(result.value("name"));
            fileName.append(".desktop");

            QFile file(fileName);
            file.open(QIODevice::Truncate | QIODevice::WriteOnly);

            QTextStream out(&file);
            out<<"[Desktop Entry]"<<endl;
            out<<"Exec="<<APP_PREF<<"/bin/q4wine-cli -p \""<<prefix_name<<"\" ";
            if (!dir_name.isEmpty())
                out<<" -d \""<<dir_name<<"\" ";
            out<<" -i \""<<icon_name<<"\" "<<endl;

            QString icon_path = result.value("icon_path");

            if (icon_path.isEmpty()){
                out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/exec_wine.png"<<endl;
            } else {
                if (QFile(icon_path).exists()){
                    out<<"Icon="<<icon_path<<endl;
                } else {
                    out<<"Icon="<<APP_PREF<<"/share/q4wine/icons/"<<icon_name<<".png"<<endl;
                }
            }
            out<<"Type=Application"<<endl;
            out<<"X-KDE-StartupNotify=true"<<endl;
            out<<"GenericName="<<result.value("name")<<endl;
            out<<"Name="<<result.value("name")<<endl;
            out<<"Path="<<result.value("wrkdir")<<endl;

            file.close();

            return fileName;
        }

        QString corelib::getEscapeString(const QString string, const bool spaces) const{
            if (spaces){
                return QRegExp::escape(string).replace(" ", "\\ ").replace("'", "\\'").replace("\"", "\\\"");
            } else {
                return QRegExp::escape(string).replace("'", "\\'").replace("\"", "\\\"");
            }
        }

                QString corelib::getShellEscapeString(QString string){
                    string.replace("\"", "\\\"");
                    string.replace("`", "\\`");
                    string.replace("$", "\\$");
                    string.replace("'", "'\\''");
                    return string;
                }

                QString corelib::getStrictEscapeString(QString string){
                    string.replace("\"", "\\\"");
                    string.replace("'", "'\\''");
                    return string;
                }

        bool corelib::mountImage(const QString image_name, const QString prefix_name){
            if (!this->umountImage(prefix_name)){
                this->showError(QObject::tr("Failed to unmount previously mounted image."));
                //TODO: allow ignoring this
                //return false;
            }

            QString mount_point=db_prefix.getMountPoint(prefix_name);
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
#endif
            QFile imageFile(image_name);

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
                if (!imageFile.exists()){
                    QString imagePath = this->db_image.getPath(image_name);
                    mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(imagePath));
                    imageFile.setFileName(imagePath);
                } else {
                    mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(image_name));
                }

                if (image_name.right(3)=="nrg"){
                    mount_string.replace("%MOUNT_OPTIONS%", "-o ro,loop,offset=307200");
                } else {
                    mount_string.replace("%MOUNT_OPTIONS%", "-o ro,loop");
                }
            }

            mount_string.replace("%GUI_SUDO%", getSetting("system", "gui_sudo").toString());
            mount_string.replace("%SUDO%", getSetting("system", "sudo").toString());
            mount_string.replace("%MOUNT_BIN%", getSetting("system", "mount").toString());
#endif

#ifdef _OS_DARWIN_
            mount_string=this->getSetting("quickmount", "mount_image_string", false).toString();
#ifdef DEBUG
            qDebug()<<"[ii] corelib::mountImage:Linux image mount base string: "<<mount_string;
#endif
            if (!imageFile.exists()){
                QString imagePath = this->db_image.getPath(image_name);
                mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(imagePath));
                imageFile.setFileName(imagePath);
            } else {
                mount_string.replace("%MOUNT_IMAGE%", this->getEscapeString(image_name));
            }
#endif
            mount_string.replace("%MOUNT_POINT%", this->getEscapeString(mount_point));
            args.clear();
            args.append("-c");
            args.append(mount_string);

#ifdef DEBUG
            qDebug()<<"[ii] corelib::mountImage: mount args: "<<args;
#endif
            bool success = this->runProcess(args, QObject::tr("Mounting..."),  QObject::tr("Mounting %1 into %2").arg(image_name).arg(mount_point));
            if (success){
                //create the symlink to the iso so that wine recognises the mountpoint as a CD-ROM
                QString prefixPath = db_prefix.getPath(prefix_name);
                QChar winDrive = db_prefix.getMountPointWindrive(prefix_name);
                if (winDrive.isNull()){
                    winDrive = this->getCdromWineDrive(db_prefix.getPath(prefix_name), mount_point);
                    if (winDrive.isNull()){
#ifdef DEBUG
                        qDebug()<<"[ii] Prefix '" << prefix_name << "' does not have a Windows drive set for the mount operation";
#endif
                        return success; //don't create the link, return true
                    }
                }
                //drive letter plus two colons links to the actual physical device (in this case the image)
                QFile physicalDriveLink(prefixPath + "/dosdevices/" + winDrive.toLower() + "::");
                if (physicalDriveLink.exists()){
                    if (!physicalDriveLink.remove()) {
                        //failed to delete, this is an error
                        qDebug()<<"[EE] failed to remove drive symlink";
                        return false;
                    }
                }
                //drive link has been removed or didn't exist -> we can create the link now
                imageFile.link(physicalDriveLink.fileName());

                //make sure the drive points to the mountpoint
                QFile mountPointLink(prefixPath + "/dosdevices/" + winDrive.toLower() + ":");
                if (mountPointLink.exists() && mountPointLink.symLinkTarget() != mount_point){
                    if (!mountPointLink.remove()) {
                        //failed to delete, this is an error
                        qDebug()<<"[EE] failed to remove mountpoint drive symlink";
                        return false;
                    }
                }
                //drive link has been removed or didn't exist -> we can create the link now
                QFile::link(mount_point, mountPointLink.fileName());
            }
            return success;
        }

        bool corelib::umountImage(const QString prefix_name){
            QString mount_point=db_prefix.getMountPoint(prefix_name);

            if (this->getMountedImages(mount_point)=="none"){
#ifdef DEBUG
                qDebug()<<"[ii] corelib::umountImage: no mounted images found in mount point: "<<mount_point;
#endif
                return true;
            }

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
        }

        bool corelib::runProcess(const QStringList args, const QString caption, const QString message){

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

        bool corelib::runProcess(const QString exec, const QStringList args, QString dir, bool showLog){
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
            int exitcode = myProcess.exitCode();
            QProcess::ExitStatus exitStatus = myProcess.exitStatus();
            if (showLog && (exitcode != 0 || exitStatus == QProcess::CrashExit)){
                // Getting env LANG variable
                QString lang=this->getLocale();

                // Read STDERR with locale support
                QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
                QString string = codec->toUnicode(myProcess.readAllStandardError());

                if (!string.isEmpty()){
                    showError(QObject::tr("It seems the process crashed. STDERR log: %1").arg(string));
                    delete (&codec);
                }
                delete (&codec);
                return false;
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

        bool corelib::killWineServer(const QString prefix_path){

            ExecObject execObj;
            execObj.cmdargs = "-kill";
            execObj.execcmd = "wineserver";

            if (!this->runWineBinary(execObj, db_prefix.getName(prefix_path), false)){
                return false;
            }

            return true;
        }

        void corelib::openHelpUrl(const QString rawurl){
            QString url="http://";
            url.append(APP_WEBSITTE);
            url.append("/documentation/");
            url.append(this->getLang());
            url.append("/");
            url.append(rawurl);

            this->openUrl(url);
            return;
        }

        void corelib::openHomeUrl(const QString rawurl){
            QString url="http://";
            url.append(APP_WEBSITTE);
            url.append("/");
            url.append(rawurl);

            this->openUrl(url);
            return;
        }

        void corelib::openUrl(const QString url){
            QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
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

                void corelib::openConsole(const QString path, const QString prefix_name){
                    QString console = this->getSetting("console", "bin").toString();
                    QStringList args;

                    if (!console.isEmpty()){
                        args = this->getSetting("console", "args", false).toString().split(" ");
                    } else {
                #ifdef DEBUG
                        qDebug()<<"[EE] No console binary set";
                #endif
                        return;
                    }



                    QString shell = getenv("SHELL");
                    if (shell.isEmpty()){
                #ifdef DEBUG
                        qDebug()<<"[EE] Can't get user shell";
                #endif
                        return;
                    }

                    QHash<QString, QString> prefix_info = db_prefix.getByName(prefix_name);

                    args << "env";
                    args << QString("WINEPREFIX=%1").arg(prefix_info.value("path"));
                    args << QString("WINEDLLPATH=%1").arg(prefix_info.value("libs"));
                    args << QString("WINELOADER=%1").arg(prefix_info.value("loader"));
                    args << QString("WINESERVER=%1").arg(prefix_info.value("server"));
                    if (!prefix_info.value("arch").isEmpty())
                        args << QString("WINEARCH=%1").arg(prefix_info.value("arch"));

                    QString prefix_path=path;
                    prefix_path.replace("'", "'\\''");

                    args << "/bin/sh" << "-c" << QString("cd \'%1\' && echo \'\' && echo \' [ii] wine environment variables are set to \"%2\" prefix settings.\' && echo \'\' && %3 ").arg(prefix_path).arg(prefix_name).arg(shell);

#ifdef DEBUG
                    qDebug()<<"[ii] Open console args:"<<args;
#endif

                    QProcess proc;
                    proc.startDetached(console, args, QDir::homePath());
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
#ifdef _OS_DARWIN_
                string=this->getWhichOut("hdiutil", false);
                string.append(" mount %MOUNT_IMAGE% -mountpoint %MOUNT_POINT%");
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
#ifdef _OS_DARWIN_
                string=this->getWhichOut("hdiutil", false);
                string.append(" unmount %MOUNT_POINT%");
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

        bool corelib::reniceProcess(const int pid, const int priority){
            QStringList args;
#ifdef _OS_DARWIN_
            args << "-e";
            args << QString("do shell script \"%1 %2 %3\" with administrator privileges").arg(this->getSetting("system", "renice").toString()).arg(priority).arg(pid);
#else
            args << this->getSetting("system", "renice").toString();
            args.append(QString("%1").arg(priority));
            args.append(QString("%1").arg(pid));

            QString sudobin = this->getSetting("system", "gui_sudo").toString();

            if (!sudobin.contains(QRegExp("/sudo$"))){
                QString arg =args.join(" ");
                args.clear();
                args.append(arg);
            }
#endif
            if (this->runProcess(this->getSetting("system", "gui_sudo").toString(), args, QDir::homePath(), false)){
                return true;
            } else {
                return false;
            }

        }

        void corelib::runAutostart(void){
            QStringList iconsList, prefixList;

            prefixList = db_prefix.getPrefixList();
            for (int i = 0; i < prefixList.size(); ++i) {
                iconsList = db_icon.getIconsList(prefixList.at(i), "autostart", "");
                for (int j = 0; j < iconsList.size(); ++j) {
                    qDebug()<<iconsList.at(j);
                    this->runIcon(prefixList.at(i), "autostart", iconsList.at(j));
                }
            }
            return;
        }

        void corelib::createPrefixDBStructure(QString prefixName){
#ifdef DEBUG
            qDebug()<<"[ii] Wizard::creating icons";
#endif


            //Is settings directory exists?
            if (!db_dir.isExistsByName(prefixName, "system")){
                db_dir.addDir(prefixName, "system");
                //Adding icons
                db_icon.addIcon("", "winecfg.exe", "winecfg", "Configure the general settings for Wine", prefixName, "system", "winecfg");
                db_icon.addIcon("--backend=user cmd", "wineconsole", "wineconsole", "Wineconsole is similar to wine command wcmd", prefixName, "system", "wineconsole");
                db_icon.addIcon("", "uninstaller.exe", "uninstaller", "Uninstall Windows programs under Wine properly", prefixName, "system", "uninstaller");
                db_icon.addIcon("", "regedit.exe", "regedit", "Wine registry editor", prefixName, "system", "regedit");
                db_icon.addIcon("", "explorer.exe", "explorer", "Browse the files in the virtual Wine Drive", prefixName, "system", "explorer");
                db_icon.addIcon("", "eject.exe", "eject", "Wine CD eject tool", prefixName, "system", "eject");
                db_icon.addIcon("", "wordpad.exe", "wordpad", "Wine wordpad text editor", prefixName, "system", "wordpad");
                db_icon.addIcon("", "taskmgr.exe", "taskmgr", "Wine task manager", prefixName, "system", "taskmgr");
                db_icon.addIcon("", "winemine.exe", "winemine", "Wine saper game", prefixName, "system", "winemine");
                db_icon.addIcon("", "oleview.exe", "wordpad", "Wine OLE/COM object viewer", prefixName, "system", "oleview");
                db_icon.addIcon("", "notepad.exe", "notepad", "Wine notepad text editor", prefixName, "system", "notepad");
                db_icon.addIcon("", "iexplore.exe", "iexplore", "Wine internet expolrer", prefixName, "system", "iexplore");
                db_icon.addIcon("", "control.exe", "control", "Wine control panel", prefixName, "system", "control");

            }

            if (!db_dir.isExistsByName(prefixName, "autostart"))
                db_dir.addDir(prefixName, "autostart");

            if (!db_dir.isExistsByName(prefixName, "import"))
                db_dir.addDir(prefixName, "import");

#ifdef DEBUG
            qDebug()<<"[ii] Wizard::done";
#endif
        }

        QString corelib::decodeRegString(QString string){
            QTextCodec *codec = QTextCodec::codecForName("UTF-16BE");
            QString ret;
            QStringList parts = string.split("\\");
            if (parts.count()>1){
                for (int j=0; j<parts.count(); j++){
                    if (parts.at(j).left(1)=="x"){
                        QString test = QString("0%1").arg(parts.at(j).left(4));
                        QByteArray temp = QByteArray::fromHex(test.toAscii().data());
                        ret.append(codec->toUnicode(temp));
                    }

                    if (parts.at(j).length()>4)
                        ret.append(parts.at(j).right(parts.at(j).length()-4));

                }
            } else {
                ret.append(string);
            }

            return ret;
        }



        bool corelib::exportPrefixesConfiguration(void){
            QStringList list = db_prefix.getPrefixList();
            QDir dir;
            QFile file;
            QString home_path = dir.homePath();
            for (int i = 0; i < list.size(); ++i){
                QString path = home_path;
                QString prefix_name = list.at(i);
                QHash<QString,QString> result = db_prefix.getByName(prefix_name);
                QString prefix_path=result.value("path");

                path.append("/.local/share/wineprefixes/");

                if (!dir.mkpath(path))
                    return false;

                path.append(prefix_name);

                if (!file.exists(path)){
                    if (!file.link(prefix_path, path))
                        return false;

                    path.append("/wrapper.cfg");
                    file.setFileName(path);
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                        return false;

                    QTextStream out(&file);
                    out << "ww_name=\"" << prefix_name << "\"\n";
                    out << "ww_winedllpath=\"" << result.value("libs") << "\"\n";
                    out << "ww_wineserver=\"" << result.value("server") << "\"\n";
                    out << "ww_wine=\"" << result.value("bin") << "\"\n";

                    file.close();
                }
            }
            return true;
        }

        QStringList corelib::importPrefixesConfiguration(void){
            QStringList prefixes;
            QStringList list = db_prefix.getPrefixList();
            QDir dir;
            QFile file;
            QString path = dir.homePath();
            path.append("/.local/share/wineprefixes/");


            dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
            dir.setPath(path);

            QFileInfoList p_list = dir.entryInfoList();

            for (int i = 0; i < p_list.size(); ++i) {
                QFileInfo fileInfo = p_list.at(i);
                QString prefix_name = fileInfo.fileName();
                QString prefix_path = fileInfo.absoluteFilePath();

                if (!db_prefix.isExistsByName(prefix_name)){
                    QFile file(QString("%1/wrapper.cfg").arg(prefix_path));
                    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                        return prefixes;

                    QString wine_bin, wine_serv, wine_loader, wine_lib;

                    while (!file.atEnd()) {
                        QByteArray line = file.readLine().trimmed();
                        if (line.startsWith("ww_wine=")){
                            wine_bin=line.mid(9, line.length()-10);
                        } else if (line.startsWith("ww_winedllpath=")){
                            wine_lib=line.mid(16, line.length()-17);
                        } else if (line.startsWith("ww_wineserver=")){
                            wine_serv=line.mid(15, line.length()-16);
                        } else if (line.startsWith("ww_wineloader=")){
                            wine_loader=line.mid(15, line.length()-16);
                        }
                    }

                    if (wine_serv.isEmpty())
                        wine_serv=wine_bin;

                    if (wine_loader.isEmpty())
                        wine_loader=wine_bin;

                    if (!db_prefix.addPrefix(prefix_name, prefix_path, wine_bin, wine_serv, wine_loader, wine_lib)){
                        qDebug()<<"[EE] Can't add prefi:x" << prefix_name <<  " into database..";
                    } else {
                        this->createPrefixDBStructure(prefix_name);
                        prefixes.append(prefix_path);
                    }
                }
            }

            return prefixes;
        }
