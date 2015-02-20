/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                              *
 *   pzinin@gmail.com                                                     *
 *   Copyright (C) 2008-2013 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include "winetricks.h"

winetricks::winetricks(QString prefixName) : QWidget()
{
    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->winetricks_bin = QDir::homePath();
    this->winetricks_bin.append("/.config/");
    this->winetricks_bin.append(APP_SHORT_NAME);
    this->winetricks_bin.append("/winetricks");

    if (!check_script(false)){
        this->winetricks_bin = CoreLib->getWhichOut("winetricks", false);
    }

    this->prefix_name = prefixName;
    this->console_bin = CoreLib->getSetting("console", "bin").toString();
    this->console_args = CoreLib->getSetting("console", "args", FALSE).toString();
}

void winetricks::install_winetricks() {
    this->downloadwinetricks();
}

bool winetricks::check_script(bool warn){
    if (!QFile(this->winetricks_bin).exists()){
        if (warn)
            QMessageBox::warning(this, QString(tr("Error")), QString(tr("Cannot locate Winetricks script. Install Winetricks script first.")));
        return false;
    }
    return true;
}

void winetricks::run_winetricks(QString item){
    if (!check_script())
        return;

    if (item.isEmpty())
        return;

#ifdef DEBUG
    qDebug()<<"[plugin] run item";
#endif

   QStringList args;
        if (!console_args.isEmpty()){
                // If we have any conslope parametres, we gona preccess them one by one
                QStringList cons_args = console_args.split(" ");
                for (int i=0; i<cons_args.count(); i++){
                        if (!cons_args.at(i).isEmpty())
                                args.append(cons_args.at(i));
                }
        }

        QString proxy_host = CoreLib->getSetting("network", "host", false).toString();

        QStringList sh_args;
        sh_args.append("env");

        if (!proxy_host.isEmpty()){
            sh_args.append(QString("HTTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
            sh_args.append(QString("FTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
        }

        QHash<QString, QString> prefix_info = db_prefix.getByName(this->prefix_name);

        sh_args << QString("WINEPREFIX='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("path")));

        if (!prefix_info.value("server").isEmpty()){
            sh_args << QString("WINEDLLPATH='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("libs")));
            sh_args << QString("WINELOADER='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("loader")));
            sh_args << QString("WINESERVER='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("server")));
        } else {
            QString prefixDllPath;
            Version vers;
            vers.id_ = prefix_info.value("version_id");
            if (vers.load()){
                if (prefix_info.value("arch") == "win32"){
                    prefixDllPath = vers.wine_dllpath32_;
                } else if (prefix_info.value("arch") == "win64"){
                    prefixDllPath = vers.wine_dllpath64_;
                } else {
                    if (vers.wine_dllpath64_.isEmpty()){
                        prefixDllPath = vers.wine_dllpath32_;
                    } else {
                        prefixDllPath = vers.wine_dllpath64_;
                    }
                }
                sh_args << QString("WINEDLLPATH='%1'").arg(CoreLib->getStrictEscapeString(prefixDllPath));
                sh_args << QString("WINELOADER='%1'").arg(CoreLib->getStrictEscapeString(vers.wine_loader_));
                sh_args << QString("WINESERVER='%1'").arg(CoreLib->getStrictEscapeString(vers.wine_server_));
            }
        }


        if (!prefix_info.value("arch").isEmpty())
            sh_args << QString("WINEARCH='%1'").arg(prefix_info.value("arch"));


        sh_args.append(CoreLib->getWhichOut("sh"));
        sh_args.append("-c");
        sh_args.append(QString("\"%1 --no-isolate %2\"").arg(this->winetricks_bin).arg(item));

        if (console_bin.split("/").last() == "konsole"){
            args.append("/bin/sh");
            args.append("-c");
        }

        args.append(sh_args.join(" "));

#ifdef DEBUG
        qDebug()<<"[DD] winetricks args: "<<args;
#endif

    QProcess proc;
    proc.startDetached(console_bin, args, QDir::homePath());

    return;
}

void winetricks::downloadwinetricks () {
        /*
         * Downloading winetricks and installing it
         */
    this->winetricks_bin = QDir::homePath();
    this->winetricks_bin.append("/.config/");
    this->winetricks_bin.append(APP_SHORT_NAME);
    this->winetricks_bin.append("/winetricks");

    QStringList args;
    if (!console_args.isEmpty()){
        // If we have any conslope parametres, we gona preccess them one by one
        QStringList cons_args = console_args.split(" ");
        for (int i=0; i<cons_args.count(); i++){
            if (!cons_args.at(i).isEmpty())
                args.append(cons_args.at(i));
        }
    }

    QString proxy_host = CoreLib->getSetting("network", "host", false).toString();
    if (!proxy_host.isEmpty()){
        args.append("env");
        args.append(QString("HTTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
        args.append(QString("FTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
    }

    QString arg;
    arg.append(CoreLib->getWhichOut("sh"));
    arg.append(" -c \"");
    arg.append(CoreLib->getWhichOut("wget"));
    arg.append(" http://kegel.com/wine/winetricks -O ");
    arg.append(this->winetricks_bin);
    arg.append(" && ");
    arg.append(CoreLib->getWhichOut("chmod"));
    arg.append(" +x ");
    arg.append(this->winetricks_bin);
    arg.append("\"");

    if (console_bin.split("/").last() == "konsole"){
        args.append("/bin/sh");
        args.append("-c");
    }

    args.append(arg);

#ifdef DEBUG
        qDebug()<<"[DD] winetricks exec: "<<console_bin<<args;
#endif
    QProcess proc(this);
    proc.startDetached(console_bin, args);
    #ifdef DEBUG
    qDebug()<<"[plugin] download done";
    #endif
    return;
}

QStringList winetricks::get_stdout_lines(QString command){
    QProcess p(this);
    QString lang = CoreLib->getLocale();
    #ifdef DEBUG
        qDebug()<<lang;
    #endif
    QTextCodec *codec = QTextCodec::codecForName(lang.toLatin1());

    p.start(command);
    p.waitForFinished();
    QString stdout = codec->toUnicode(p.readAllStandardOutput());
    return stdout.split("\n");
}

bool winetricks::parse() {
    if (!check_script())
        return false;

    QProgressDialog *pd = new QProgressDialog(tr("Refreshing winetricks application list."), QString(), 0, 100);
    pd->setModal(true);
    pd->show();
    pd->setValue(5);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

    #ifdef DEBUG
        qDebug()<<"[plugin] parsing winetricks output";
    #endif

    QString pargs;
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

    db_sysconfig.drop_items(D_PROVIDER_WINETRICKS);

    pargs.append(winetricks_bin);
    pargs.append(" list");

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

    QStringList subtypes = this->get_stdout_lines(pargs);

    pd->setValue(10);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

    int step = 90 / subtypes.length() + 1;

    foreach (QString subtype, subtypes){
        pd->setValue(pd->value() + step);
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);
        if (!subtype.isEmpty()){
            subtype = subtype.trimmed();
            pargs.clear();
            pargs.append(winetricks_bin);
            pargs.append(" ");
            pargs.append(subtype);
            pargs.append(" list");
            foreach (QString item, this->get_stdout_lines(pargs)){
                if (item.isEmpty())
                    continue;

                db_sysconfig.add_item(item.left(24).trimmed(), "exec.png", item.mid(24).trimmed(), subtype, D_PROVIDER_WINETRICKS);
            }
        }
    }

    #ifdef DEBUG
        qDebug()<<"[plugin] parsing winetricks output done";
    #endif

    pd->setValue(100);

    return true;
}



