/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                                 *
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

#include "wineobject.h"

WineObject::WineObject(QObject *parent) : QObject(parent)
{
    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName("libq4wine-core");
#else
    libq4wine.setFileName(QString("%1/q4wine-lib/libq4wine-core").arg(APP_BUILD));
#endif

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->programNice = 0;
    this->prefixId = 0;
    this->useConsole=false;

    this->user = getenv("USER");

    return;
}

void WineObject::setPrefix(QString prefix){
    QHash<QString, QString> prefix_info = db_prefix.getByName(prefix);

    this->prefixId=prefix_info.value("id").toInt();
    this->prefixName=prefix_info.value("name");
    this->prefixBinary=CoreLib->getStrictEscapeString(prefix_info.value("bin"));
    this->prefixDllPath=CoreLib->getStrictEscapeString(prefix_info.value("libs"));
    this->prefixLoader=CoreLib->getStrictEscapeString(prefix_info.value("loader"));
    this->prefixPath=CoreLib->getStrictEscapeString(prefix_info.value("path"));
    this->prefixServer=CoreLib->getStrictEscapeString(prefix_info.value("server"));
    this->prefixArch=prefix_info.value("arch");
    return;
}

void WineObject::setProgramBinary(QString binary){
    this->programBinaryName=binary.split("/").last().split("\\").last();
    this->programBinary=CoreLib->getShellEscapeString(binary);
    return;
}

void WineObject::setProgramArgs(QString args){
        this->programArgs=args;
    return;
}

void WineObject::setProgramDisplay(QString dislpay){
    this->programDisplay=dislpay;
    return;
}

void WineObject::setProgramDebug(QString debug){
    this->programDebug=debug;
    return;
}

void WineObject::setProgramNice(int nice){
    this->programNice = nice;
    return;
}

void WineObject::setProgramDesktop(QString desktop){
    this->programDesktop=desktop;
    return;
}

void WineObject::setProgramOverride(QString override){
    this->overrideDllList = override;
}

void WineObject::setProgramWrkdir(QString wrkdir){
    this->programWrkDir = CoreLib->getShellEscapeString(wrkdir);
}

void WineObject::setProgramLang(QString lang){
    this->programLang=lang;
    return;
}

void WineObject::setUseConsole(int console){
    if (console==1){
        this->useConsole=true;
    } else {
        this->useConsole=false;
    }
    return;
}

QString WineObject::createEnvString(){
    QString env;
    if (this->prefixName.isEmpty())
        this->setPrefix("Default");

    env.append(QString(" WINEPREFIX='%1' ").arg(this->prefixPath));
    env.append(QString(" WINESERVER='%1' ").arg(this->prefixServer));
    env.append(QString(" WINELOADER='%1' ").arg(this->prefixLoader));
    env.append(QString(" WINEDLLPATH='%1' ").arg(this->prefixDllPath));
    if (!this->prefixArch.isEmpty())
        env.append(QString(" WINEARCH='%1' ").arg(this->prefixArch));

    if (!this->programDebug.isEmpty()){
        env.append(QString(" WINEDEBUG='%1' ").arg(this->programDebug));
    } else {
        if ((!this->useConsole) && (CoreLib->getSetting("logging", "enable", false, 0).toInt()==0)){
                env.append(QString(" WINEDEBUG='-all' "));
        }
    }

    if (!this->programDisplay.isEmpty())
        env.append(QString(" DISPLAY='%1' ").arg(this->programDisplay));

    if (!this->overrideDllList.isEmpty())
        env.append(QString(" WINEDLLOVERRIDES=%1 ").arg(this->overrideDllList));

    if (!this->programLang.isEmpty())
        env.append(QString(" LANG='%1' ").arg(this->programLang));

    return env;
}

int WineObject::runSys(){
    bool logEnabled = false;

    QString env = this->createEnvString();
    QString stdout, app_stdout;

    QTextCodec *codec = QTextCodec::codecForName(CoreLib->getLocale().toAscii());

    FILE *fp;
    int status;
    char path[PATH_MAX];

    QString run_string="";

    /*
     * We need to trim wrk dir path from binary path
     */
    this->programBinary = this->programBinary.replace(this->programWrkDir, "").replace("/", "");

    if (!env.isEmpty()){
        run_string.append(" env ");
        run_string.append(env);
    }

    if (this->useConsole){
        // If we gona use console output, so exec program is program specificed at CONSOLE global variable
        run_string.append(QString(" %1 ").arg(CoreLib->getSetting("console", "bin").toString()));

        if (!CoreLib->getSetting("console", "args", false).toString().isEmpty()){
            // If we have any conslope parametres, we gona preccess them one by one
            run_string.append(CoreLib->getSetting("console", "args", false).toString());
        }
    }

    run_string.append(" /bin/sh -c \"");

    if (!this->programWrkDir.isEmpty()){
        run_string.append(QString(" cd \'%1\' && ").arg(this->programWrkDir));
    }

    if (this->programNice != 0){
        run_string.append(QString(" %1 -n %2 ").arg(CoreLib->getSetting("system", "nice", false).toString()).arg(this->programNice));
    }


    if (this->programBinary=="wineserver"){
        run_string.append(QString(" '%1server' ").arg(this->prefixBinary));
    } else {
        run_string.append(QString(" '%1' ").arg(this->prefixBinary));
}

    if (!this->programDesktop.isEmpty()){
        QString deskname = this->programBinaryName;
        deskname.replace(" ", ".");
        deskname.replace("&", ".");
        deskname.replace("!", ".");
        deskname.replace("$", ".");
        deskname.replace("*", ".");
        deskname.replace("(", ".");
        deskname.replace(")", ".");
        deskname.replace("[", ".");
        deskname.replace("]", ".");
        deskname.replace(";", ".");
        deskname.replace("'", ".");
        deskname.replace("\"", ".");
        deskname.replace("|", ".");
        deskname.replace("`", ".");
        deskname.replace("\\", ".");
        deskname.replace("/", ".");
        deskname.replace(">", ".");
        deskname.replace("<", ".");
        run_string.append(QString(" explorer.exe /desktop=%1,%2 ").arg(deskname).arg(this->programDesktop));
    }

    if (this->programBinary=="wineserver"){
            run_string.append(QString(" %2 ").arg(programArgs));
    } else {
        run_string.append(QString(" \'%1\' %2 ").arg(this->programBinary).arg(programArgs));
    }
    run_string.append(" 2>&1 \"");

 /*   QStringList argss;
    argss<<"-c"<<run_string;

    QProcess *proc = new QProcess(0);
    proc->start("/bin/sh", argss);

    if (!proc->waitForStarted(90000))
             return -1;
    return 0;*/
    /*
    if (this->useConsole){
        // If we gona use console output, so exec program is program specificed at CONSOLE global variable
        run_string = QString(" %1 ").arg(CoreLib->getSetting("console", "bin").toString());

        if (!CoreLib->getSetting("console", "args", false).toString().isEmpty()){
            // If we have any conslope parametres, we gona preccess them one by one
            run_string.append(CoreLib->getSetting("console", "args", false).toString());
        }

        run_string.append(" /bin/sh -c \"cd \'");
        run_string.append(this->programWrkDir);
        run_string.append("\' && ");
    } else {
        if (chdir(codec->fromUnicode(this->programWrkDir).data()) != 0){
            qDebug()<<"[EE] chdir to:"<<codec->fromUnicode(this->programWrkDir).data()<<"fail";
            return -1;
        } else {
            qDebug()<<"wooot";
        }
    }

    //Setting enveropment variables
    if (!env.isEmpty()){
        run_string.append(" env ");
        run_string.append(env);
    }

    if (this->programNice != 0){
        run_string.append(QString(" %1 -n %2 ").arg(CoreLib->getSetting("system", "nice", false).toString()).arg(this->programNice));
    }

    run_string.append(QString(" %1 ").arg(this->prefixBinary));

    if (!this->programDesktop.isEmpty()){
        QString deskname = this->programBinaryName;
        deskname.replace(" ", ".");
        deskname.replace("&", ".");
        deskname.replace("!", ".");
        deskname.replace("$", ".");
        deskname.replace("*", ".");
        deskname.replace("(", ".");
        deskname.replace(")", ".");
        deskname.replace("[", ".");
        deskname.replace("]", ".");
        deskname.replace(";", ".");
        deskname.replace("'", ".");
        deskname.replace("\"", ".");
        deskname.replace("|", ".");
        deskname.replace("`", ".");
        deskname.replace("\\", ".");
        deskname.replace("/", ".");
        deskname.replace(">", ".");
        deskname.replace("<", ".");
        run_string.append(QString(" explorer.exe /desktop=%1,%2 ").arg(deskname).arg(this->programDesktop));
    }

    run_string.append(QString(" \"%1\" %2 ").arg(this->programBinary).arg(programArgs));
    run_string.append(" 2>&1 ");

     if (this->useConsole){
         run_string.append(" \"");
     }
    */

#ifdef DEBUG
    qDebug()<<run_string;
#endif
    stdout.append("Exec string:");
    stdout.append("\n");
    stdout.append(run_string.trimmed());
    stdout.append("\n");

    fp = popen(codec->fromUnicode(run_string).data(), "r");
    if (fp == NULL){
        this->sendMessage(QString("error/%1/%2").arg(this->programBinaryName).arg(this->prefixName));
    } else {
        if (this->useConsole){
            this->sendMessage(QString("console/%1/%2").arg(this->programBinaryName).arg(this->prefixName));
        } else {
            this->sendMessage(QString("start/%1/%2").arg(this->programBinaryName).arg(this->prefixName));
        }
    }
        /* Handle error */;

    while (fgets(path, PATH_MAX, fp) != NULL){
        app_stdout.append(codec->toUnicode(path));
    }

    status = pclose(fp);

    if (CoreLib->getSetting("logging", "enable", false, 0).toInt()==1)
        logEnabled = true;



    stdout.append("Exit code:");
    stdout.append("\n");
    stdout.append(QString("%1").arg(status));
    stdout.append("\n");
    stdout.append("App STDOUT and STDERR output:");
    stdout.append("\n");
    stdout.append(app_stdout);

    if (!this->useConsole){
        if (logEnabled){
            uint date = QDateTime::currentDateTime ().toTime_t();
            db_logging.addLogRecord(this->prefixId, this->programBinaryName, status, stdout, date);
        }
        this->sendMessage(QString("finish/%1/%2/%3").arg(this->programBinaryName).arg(this->prefixName).arg(status));
    } else {
        if (status!=0){
            if (logEnabled){
                uint date = QDateTime::currentDateTime ().toTime_t();
                db_logging.addLogRecord(this->prefixId, this->programBinaryName, status, stdout, date);
            }
            this->sendMessage(QString("console_error/%1/%2").arg(this->programBinaryName).arg(this->prefixName));
        }
    }

    if (status>0){
        QTextStream QErr(stderr);
        QErr<<stdout;

        status=-1;
    }

    return status;
}

void WineObject::sendMessage(QString message){
    QLocalSocket socket(this);
    socket.connectToServer( QString("/tmp/q4wine-%1.sock").arg(this->user) , QIODevice::WriteOnly );

    if (socket.waitForConnected()){
#ifdef DEBUG
        qDebug()<<"helper:sendMessage Connected!";
#endif

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << (quint16)0;
        out << message;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        socket.write(block);
        socket.flush();

        socket.disconnectFromServer();
    }
#ifdef DEBUG
    else {
        qDebug()<<"[ii] helper:sendMessage Not connected!";
    }
#endif
    return;
}

void WineObject::setOverrideDll(QString dll_list){
    this->overrideDllList = dll_list;
}
