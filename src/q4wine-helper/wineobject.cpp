/***************************************************************************
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

#include "wineobject.h"

WineObject::WineObject(QObject *parent) : QObject(parent)
{
    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName(QString("%1/q4wine-lib/libq4wine-core").arg(QString::fromUtf8(APP_BUILD)));
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
    this->logEnabled = CoreLib->getSetting("logging", "enable", false, 1).toBool();
    return;
}

bool WineObject::setPrefix(QString prefix){
    QHash<QString, QString> prefix_info = db_prefix.getByName(prefix);

    this->prefixId=prefix_info.value("id").toInt();
    this->prefixName=prefix_info.value("name");
    this->prefixArch=prefix_info.value("arch");
    this->prefixRunString=prefix_info.value("run_string");
    this->prefixPath=CoreLib->getStrictEscapeString(prefix_info.value("path"));
    if (!prefix_info.value("bin").isEmpty()){
        this->prefixBinary=CoreLib->getStrictEscapeString(prefix_info.value("bin"));
        this->prefixDllPath=CoreLib->getStrictEscapeString(prefix_info.value("libs"));
        this->prefixLoader=CoreLib->getStrictEscapeString(prefix_info.value("loader"));
        this->prefixServer=CoreLib->getStrictEscapeString(prefix_info.value("server"));
    } else {
        this->versionId = prefix_info.value("version_id");
    }
    return true;
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

void WineObject::setPreRun(QString path){
    //FIXME add check
    this->prerun_script = path;
    return;
}

void WineObject::setPostRun(QString path){
    //FIXME add check
    this->postrun_script = path;
    return;
}

QString WineObject::createEnvString(){
    QString env;
    if (this->prefixName.isEmpty())
        this->setPrefix("Default");

    if (!this->versionId.isEmpty()){
        this->prefixBinary.clear();
        this->prefixDllPath.clear();
        this->prefixLoader.clear();
        this->prefixServer.clear();
        Version vers;
        vers.id_ = this->versionId;
        if (vers.load()){
            this->prefixBinary = vers.wine_exec_;
            this->prefixLoader = vers.wine_loader_;
            this->prefixServer = vers.wine_server_;
            if (this->prefixArch == "win32"){
                this->prefixDllPath = vers.wine_dllpath32_;
            } else if (this->prefixArch == "win64"){
                this->prefixDllPath = vers.wine_dllpath64_;
            } else {
                if (vers.wine_dllpath64_.isEmpty()){
                    this->prefixDllPath = vers.wine_dllpath32_;
                } else {
                    this->prefixDllPath = vers.wine_dllpath64_;
                }
            }
        }
    }

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

    if (!this->useConsole){
        int ret = this->runScript(this->prerun_script);
        if ( ret != 0){
            return 255;
        }
    }

    QString env = this->createEnvString();
    QString app_stdout;

    QTextCodec *codec = QTextCodec::codecForName(CoreLib->getLocale().toAscii());

    FILE *fp;
    int status;
    char path[PATH_MAX];

    QString run_string=this->prefixRunString;
    /*
     * We need to trim wrk dir path from binary path
     */
    this->programBinary = this->programBinary.replace(this->programWrkDir, "").replace("/", "");

    if (this->useConsole){
        // Owerride " with \" in case of using console app.
        run_string.replace("\"", "\\\"");
        // If we gona use console output, so exec program is program specificed at CONSOLE global variable
        QString console_bin = CoreLib->getSetting("console", "bin").toString();
        QString console_args = CoreLib->getSetting("console", "args", false).toString();

        if (console_bin.split("/").last() == "konsole"){
            console_args.append(" /bin/sh -c ");
        }

        run_string.replace("%CONSOLE_BIN%", console_bin);
        run_string.replace("%CONSOLE_ARGS%", QString("%1 \"").arg(console_args));
    } else {
        run_string.replace("%CONSOLE_BIN%", "");
        run_string.replace("%CONSOLE_ARGS%", "");
    }

    if (!env.isEmpty()){
        run_string.replace("%ENV_BIN%", "env");
        run_string.replace("%ENV_ARGS%", env);
    } else {
        run_string.replace("%ENV_BIN%", "");
        run_string.replace("%ENV_ARGS%", "");
    }

    if (!this->programWrkDir.isEmpty()){
        run_string.replace("%WORK_DIR%", QString("cd \'%1\' &&").arg(this->programWrkDir));
    } else {
        run_string.replace("%WORK_DIR%", "");
    }

    if (this->programNice != 0){
        run_string.replace("%SET_NICE%", QString("%1 -n %2").arg(CoreLib->getSetting("system", "nice", false).toString()).arg(this->programNice));
    } else {
        run_string.replace("%SET_NICE%", "");
    }

    if (this->programBinary=="wineserver"){
        run_string.replace("%WINE_BIN%", QString(" '%1server' ").arg(this->prefixBinary));
    } else {
        run_string.replace("%WINE_BIN%", QString(" '%1' ").arg(this->prefixBinary));
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
        run_string.replace("%VIRTUAL_DESKTOP%", QString(" explorer.exe /desktop=%1,%2 ").arg(deskname).arg(this->programDesktop));
    } else {
        run_string.replace("%VIRTUAL_DESKTOP%", "");
    }

    if (this->programBinary=="wineserver"){
        run_string.replace("%PROGRAM_BIN%", "");
        run_string.replace("%PROGRAM_ARGS%", programArgs);
    } else {
        run_string.replace("%PROGRAM_BIN%", QString("\'%1\'").arg(this->programBinary));
        run_string.replace("%PROGRAM_ARGS%", programArgs);
    }

    if (this->useConsole){
        run_string.append("\"");
    }

#ifdef DEBUG
    qDebug()<<"Template string: "<<this->prefixRunString;
    qDebug()<<"Exec string    : "<<run_string;
#endif

    stdout.append("Exec string:\n");
    stdout.append(run_string.trimmed());
    stdout.append("\n");

    if (this->prefixBinary.isEmpty() or this->prefixDllPath.isEmpty() or \
        this->prefixLoader.isEmpty() or this->prefixServer.isEmpty()){
        stdout.append("Exit code:");
        stdout.append("\n");
        stdout.append(QString("%1").arg(10));
        stdout.append("\n");
        stdout.append("App STDOUT and STDERR output:\n");
        stdout.append("Can't load version information. Check prefix and/or version settings.");
        return 10;
    }

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

    if (!this->useConsole){
        int ret = this->runScript(this->postrun_script);
        if (ret != 0)
            return ret;
    }

    stdout.append("Exit code:");
    stdout.append("\n");
    stdout.append(QString("%1").arg(status));
    stdout.append("\n");
    stdout.append("App STDOUT and STDERR output:");
    stdout.append("\n");
    stdout.append(app_stdout);

    if (status>0){
        QTextStream QErr(stderr);
        QErr<<stdout;
        status=-1;
    }

    return status;
}

int WineObject::run(){
    int status;
    status = runSys();
    logStdout(status);
    return status;
}

int WineObject::runScript(QString script_path){
    int status;
    FILE *fp;
    char path[PATH_MAX];
    QString stdout, app_stdout;

    QString env = this->createEnvString();
    QTextCodec *codec = QTextCodec::codecForName(CoreLib->getLocale().toAscii());

    QString run_string = "";
    if (!env.isEmpty()){
        run_string.append(" env ");
        run_string.append(env);
    }

    run_string.append(" /bin/sh -c \"");
    run_string.append(script_path);
    run_string.append("\" 2>&1");

    fp = popen(codec->fromUnicode(run_string).data(), "r");
    if (fp == NULL)
        return 255;

    while (fgets(path, PATH_MAX, fp) != NULL){
        app_stdout.append(codec->toUnicode(path));
    }

    status = pclose(fp);
    if (status != 0){
        if (this->logEnabled){
            stdout.append("Exit code:");
            stdout.append("\n");
            stdout.append(QString("%1").arg(status));
            stdout.append("\n");
            stdout.append("App STDOUT and STDERR output:");
            stdout.append("\n");
            stdout.append(app_stdout);

            uint date = QDateTime::currentDateTime().toTime_t();
            db_logging.addLogRecord(this->prefixId, this->programBinaryName, status, stdout, date);
        }
        this->sendMessage(QString("finish/%1/%2/%3").arg(this->programBinaryName).arg(this->prefixName).arg(status));
    }
    return status;
}

void WineObject::logStdout(int status){
    if (!this->useConsole){
        if (logEnabled){
            uint date = QDateTime::currentDateTime ().toTime_t();
            db_logging.addLogRecord(this->prefixId, this->programBinaryName, status, stdout, date);
        }
        this->sendMessage(QString("finish/%1/%2/%3").arg(this->programBinaryName).arg(this->prefixName).arg(status));
    } else {
        if (status!=0){
            if (this->logEnabled){
                uint date = QDateTime::currentDateTime ().toTime_t();
                db_logging.addLogRecord(this->prefixId, this->programBinaryName, status, stdout, date);
            }
            this->sendMessage(QString("console_error/%1/%2").arg(this->programBinaryName).arg(this->prefixName));
        }
    }
}

void WineObject::sendMessage(QString message){
    QLocalSocket socket(this);
    socket.connectToServer( QString("/tmp/q4wine-%1.sock").arg(getuid()) , QIODevice::WriteOnly );

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

