/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                                 *
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
    return;
}

void WineObject::setProgramBinary(QString binary){
    this->programBinary=binary;
    return;
}

void WineObject::setProgramArgs(QString args){
    this->programArgs=args;
    return;
}

void WineObject::setProgramDir(QString dir){
    this->programDir=dir;
    return;
}

void WineObject::setProgramDisplay(QString dislpay){
    this->programDisplay=dislpay;
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

void WineObject::setUseConsole(bool console){
    this->useConsole=console;
    return;
}

void WineObject::runSys(){
    qDebug()<<this->programBinary;
    if (this->programBinary.isEmpty())
        return;

    QString stdout;
    QString message;

    FILE *fp;
    int status;
    char path[PATH_MAX];


    fp = popen(QString("%1 2>&1").arg(this->programBinary).toAscii().data(), "r");
    if (fp == NULL)
        /* Handle error */;

    while (fgets(path, PATH_MAX, fp) != NULL){
        stdout.append(path);
    }

    qDebug()<<"out"<<stdout;

    status = pclose(fp);


    QLocalSocket socket(this);
    socket.connectToServer( "/tmp/q4wine-brezerk.sock" , QIODevice::WriteOnly );

    if (socket.waitForConnected()){
        qDebug()<<"Connected!";

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << (quint16)0;
        out << QString("%1\n%2\n%3").arg(this->programBinary).arg(status).arg(stdout);
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        socket.write(block);
        socket.flush();
        socket.disconnectFromServer();

    } else {
        qDebug()<<"Not connected!";
    }

    qDebug()<<"Exit status:"<<status;


    /*
    FILE *f = popen("VAR1=bla VAR2=foo ./child", "r");

    char buffer[MAX];
*/
    /*
    QProcess proc(this);

    QStringList env = proc.systemEnvironment();
    env << "WINEPREFIX=/home/brezerk/ddddd";
    proc.setEnvironment(env);

    qDebug()<<proc.environment();

    proc.start("winecfg");
    proc.waitForFinished();

    qDebug()<<proc.readAll();
*/
    //qDebug()<<proc.environment();


}

void WineObject::runQt(){

    QProcess proc(this);

    //connect(proc, SIGNAL(readyReadStandardError ()), this, printOut());
    //connect(proc, SIGNAL(readyReadStandardOutput ()), this, printOut());

    /*QStringList env = proc.systemEnvironment();
    env << "WINEPREFIX=/home/brezerk/ddddd";
    proc.setEnvironment(env);*/

    //qDebug()<<proc.environment();

    proc.execute("winecfg");
    //proc.waitForFinished();
    qDebug()<<proc.readAll();
    qDebug()<<proc.exitCode();
    qDebug()<<proc.exitStatus();

    //qDebug()<<proc.environment();

    return;
}

void WineObject::printOut(){

    return;
}

void WineObject::setOverrideDll(QString dll_list){
    this->overrideDllList = dll_list;
}
