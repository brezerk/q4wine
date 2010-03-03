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

WineObject::WineObject(QString prefixName, QObject *parent) : QObject(parent)
{
    QStringList prefix;

    if (!prefixName.isEmpty()){
        prefix = db_prefix.getFieldsByPrefixName(prefixName);

        if (prefix.count() > 0){
            this->prefixName = prefixName;
            this->prefixPath = prefix.at(1);
            this->prefixDllPath = prefix.at(2);
            this->prefixLoader = prefix.at(3);
            this->prefixServer = prefix.at(4);
            this->prefixBinary = prefix.at(5);
        } else {
            this->prefixName.clear();
            this->prefixPath.clear();
            this->prefixDllPath.clear();
            this->prefixLoader.clear();
            this->prefixServer.clear();
            this->prefixBinary.clear();
        }

    }
    return;
}

void WineObject::setProgramBinary(QString binary){
    this->prefixBinary=binary;
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
    this->programDisplay=display;
    return;
}

void WineObject::setProgramNice(int nice){
    this->programNice = nice;
    return;
}

void WineObject::setDesktop(QString desktop){
    this->programDesktop=desktop;
    return;
}

void WineObject::setUseConsole(bool console){
    this->useConsole=console;
    return;
}
