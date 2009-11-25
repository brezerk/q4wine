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

#include "appdbsearchwidget.h"

AppDBSearchWidget::AppDBSearchWidget(QString name, QString desc, QList<WineAppDBVersionInfo> &versions, const int appid, QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	this->setAppName(name);
	this->setAppDesc(desc);
	this->_APPID=appid;

	AppDBAppVersionWidget *version;
	for (int i=0; i<versions.count(); i++){
		version = new AppDBAppVersionWidget(appid, versions.at(i));
		AppVersionListerLayout->addWidget(version);
		connect(version, SIGNAL(versionTrigged(short int, int, int)), this, SIGNAL(versionTrigged(short int, int, int)));
	}

	lblAppName->installEventFilter(this);
	lblAppName->setCursor(Qt::PointingHandCursor);
	return;
}

AppDBSearchWidget::~AppDBSearchWidget(){
	//nothig but...
}

void AppDBSearchWidget::setAppName(QString name){
	//FIXME: url might pint to web xml engine
	lblAppName->setText(name);
	return;
}

void AppDBSearchWidget::setAppDesc(QString desc){
	if (desc.length()>=255){
		lblAppDesc->setText(QString("%1...").arg(desc.left(100)));
	} else {
		lblAppDesc->setText(desc);
	}
	return;
}


bool AppDBSearchWidget::eventFilter(QObject *obj, QEvent *event){
	// qDebug()<<obj->objectName();

	if (event->type()==QEvent::MouseButtonRelease){
		emit(versionTrigged(3, this->_APPID, 0));
	}

	if (event->type()==QEvent::Enter){
		QPalette p(palette());
		// Set colour
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::Highlight));
		this->lblAppName->setPalette(p);
	} else if (event->type()==QEvent::Leave){
		// Restore default color
		this->lblAppName->setPalette(QPalette());
	}
	return false;
}
