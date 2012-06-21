/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                              *
 *   pzinin@gmail.com                                                     *
 *   Copyright (C) 2010 by Malakhov Alexey                           *
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

#include "winetricks.h"

winetricks::winetricks(QString prefixName)
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

    this->winetricks_bin.append(QDir::homePath());
    this->winetricks_bin.append("/.config/");
    this->winetricks_bin.append(APP_SHORT_NAME);
    this->winetricks_bin.append("/winetricks");

    this->prefix_name = prefixName;
    this->console_bin = CoreLib->getSetting("console", "bin").toString();
    this->console_args = CoreLib->getSetting("console", "args", FALSE).toString();
}


void winetricks::install_winetricks() {
    this->downloadwinetricks();
}

void winetricks::run_winetricks(QString item){

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

        args.append("env");

        if (!proxy_host.isEmpty()){
            args.append(QString("HTTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
            args.append(QString("FTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
        }

        QHash<QString, QString> prefix_info = db_prefix.getByName(this->prefix_name);

        args << QString("WINEPREFIX=%1").arg(prefix_info.value("path"));
        args << QString("WINEDLLPATH=%1").arg(prefix_info.value("libs"));
        args << QString("WINELOADER=%1").arg(prefix_info.value("loader"));
        args << QString("WINESERVER=%1").arg(prefix_info.value("server"));
        if (!prefix_info.value("arch").isEmpty())
            args << QString("WINEARCH=%1").arg(prefix_info.value("arch"));

        args.append(CoreLib->getWhichOut("sh"));
        args.append("-c");
        args.append(QString("%1 --no-isolate %2").arg(this->winetricks_bin).arg(item));

    QProcess proc;
    proc.startDetached(console_bin, args, QDir::homePath());

    return;
}

void winetricks::downloadwinetricks () {
        /*
         * Downloading winetricks and installing it
         */

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

    args.append(CoreLib->getWhichOut("sh"));
    args.append("-c");

    QString arg;
        arg.append(CoreLib->getWhichOut("wget"));
        arg.append(" http://kegel.com/wine/winetricks -O ");
        arg.append(this->winetricks_bin);
        arg.append(" && ");
        arg.append(CoreLib->getWhichOut("chmod"));
        arg.append(" +x ");
        arg.append(this->winetricks_bin);
    args.append(arg);

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
    QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());

    p.start(command);
    p.waitForFinished();
    QString stdout = codec->toUnicode(p.readAllStandardOutput());
    return stdout.split("\n");
}

bool winetricks::parse() {
    QProgressDialog *pd = new QProgressDialog("Refresh Winetricks application list.", QString(), 0, 100);
    pd->setModal(true);
    pd->setValue(5);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

    #ifdef DEBUG
        qDebug()<<"[plugin] parsing winetricks output";
    #endif



    QString pargs;
    if (!QFile(this->winetricks_bin).exists()){
            //QMessageBox::warning(Non, QString("Error"), QString("<p>q4wine cannot locate Winetricks at %1 path!</p><p>The script is maintained and hosted by DanKegel at http://www.kegel.com/wine/winetricks.  You can get it from the commandline with the command:</p><p>wget http://www.kegel.com/wine/winetricks</p><p>Or use \"Install winetricks\" button.</p>").arg(this->winetricks_bin));
        return false;
    }
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



