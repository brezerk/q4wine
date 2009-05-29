/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#include "main.h"

corelib::corelib(bool _GUI_MODE)
{
    // Setting gui mode, if false - cli mode else gui mode
    this->_GUI_MODE=_GUI_MODE;
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
            // Try to read stat file
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
                    if ((name.contains("wine") || name.contains(".exe")) && !name.contains(APP_SHORT_NAME)){
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

                    }
                }
            file.close();
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
            kp = kvm_getprocs(kd, KERN_PROC_ALL, 0, &cntproc);
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

                if (cur_pids.indexOf(QObject::tr("%1").arg(ipid))==-1){
                    cur_pids << QObject::tr("%1").arg(ipid);
                    name = kp[i].ki_comm;

                    if ((name.contains("wine") || name.contains(".exe")) && !name.contains(APP_SHORT_NAME)){
                         ni = kp[i].ki_nice;
                         nice = QObject::tr("%1").arg(ni);

                         if (name.contains("pthread")){
                              envs = kvm_getargv(kd, (const struct kinfo_proc *) &(kp[i]), 0);
                              if (envs){
                                    name = envs[0];
                                    if (name.isEmpty()){
                                         name = kp[i].ki_comm;
                                    } else {
                                         name = name.split('\\').last();
                                    }
                               }
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
                                 prefix="";
                          }

                          // Puting all fields into QList<QStringList>
                          procline.clear();
                          procline << QObject::tr("%1").arg(ipid) << name << nice << prefix;
                          proclist << procline;
                     }
                }
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
	    cout << "test"; // message.toLatin1();
	break;
    }
    return 0;
}
