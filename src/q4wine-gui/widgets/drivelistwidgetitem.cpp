/***************************************************************************
 *   Copyright (C) 2008-2010 by Malakhov Alexey                            *
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

#include "drivelistwidgetitem.h"

DriveListWidgetItem::DriveListWidgetItem(QString themeName, QListWidget *parent, int type): QObject( parent), QListWidgetItem(parent, type)
{
	this->themeName=themeName;
}


void DriveListWidgetItem::setDrive(QString letter, QString path, QString type){
	this->letter=letter;
	this->path=path;
	if (type.isEmpty()){
		this->type="auto";
	} else {
		this->type=type;
	}
	this->setText(QString("%1 %2\nType: %3").arg(letter).arg(path).arg(this->type));
	this->setIcon(loadIcon(this->getDrivePic(type)));
	return;
}

QString DriveListWidgetItem::getLetter(){
	return this->letter;
}

QString DriveListWidgetItem::getPath(){
	return this->path;
}

QString DriveListWidgetItem::getType(){
	return this->type;
}

QString DriveListWidgetItem::getDrivePic(QString driveType){
	QString pic;
	if (driveType.isEmpty()){
		pic = "data/drive_menu.png";
	} else {
		if (driveType=="hd"){
			pic = "data/drive_menu.png";
		} else if (driveType=="network"){
			pic = "data/drive_menu.png";
		} else if (driveType=="floppy"){
			pic = "data/drive_menu.png";
		} else if (driveType=="cdrom"){
			pic = "data/cdrom_menu.png";
		} else {
			pic = "data/drive_menu.png";
		}
	}
	return pic;
}

QIcon DriveListWidgetItem::loadIcon(QString iconName){
	  // Function tryes to load icon image from theme dir
	  // If it fails -> load default from rsource file
	  QIcon icon;
	  if ((!this->themeName.isEmpty()) and (this->themeName!="Default")){
			icon.addFile(QString("%1/%2").arg(this->themeName).arg(iconName));
			if (icon.isNull()){
				  icon.addFile(QString(":/%1").arg(iconName));
			}
	  } else {
			icon.addFile(QString(":/%1").arg(iconName));
	  }
	  return icon;
}

