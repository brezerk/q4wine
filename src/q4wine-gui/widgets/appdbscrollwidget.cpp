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

#include "appdbscrollwidget.h"

AppDBScrollWidget::AppDBScrollWidget(AppDBHeaderWidget *appdbHeader, QWidget * parent) : QScrollArea(parent)
{
	this->setAutoFillBackground(true);
	this->setFrameShape(QFrame::NoFrame);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setWidgetResizable(true);

	contentWidget = new QWidget();
	contentLayout = new QVBoxLayout(contentWidget);
	contentLayout->setMargin(3);
	this->setWidget(contentWidget);
	this->appdbHeader=appdbHeader;

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	connect(appdbHeader, SIGNAL(linkTrigged(short int, QString, int)), this, SLOT(linkTrigged(short int, QString, int)));
	timer->stop();

	return;
}

void AppDBScrollWidget::addSearchWidget(WineAppDBInfo appinfo){
	if (contentLayout){
		AppDBSearchWidget *AppDBWidget;
		AppDBWidget = new AppDBSearchWidget(appinfo.name, appinfo.desc, appinfo.versions, appinfo.id);
		contentLayout->addWidget(AppDBWidget);
		connect(AppDBWidget, SIGNAL(versionTrigged(short int, int, int)), this, SLOT(versionTrigged(short int, int, int)));
	}
	return;
}

void AppDBScrollWidget::clear(void){
	if (contentWidget){
		QList<QObject*> list = contentWidget->children();
		for (int i=0; i<list.count(); i++){
			list.at(i)->disconnect();
			delete(list.at(i));
		}

		contentLayout = new QVBoxLayout(contentWidget);
		contentLayout->setMargin(3);
	}
	return;
}

void AppDBScrollWidget::insertStretch(void){
	if (contentLayout){
		contentLayout->insertStretch(-1);
	}
}

void AppDBScrollWidget::startSearch(short int action, QString search){
#ifdef DEBUG
	qDebug()<<"[ii] startSearch: "<<action<<search;
#endif
	this->_ACTION=action;
	this->_SEARCH=search;
	timer->start(3000);
}

void AppDBScrollWidget::linkTrigged(short int action, QString search, int value){
#ifdef DEBUG
	qDebug()<<"[ii] linkTrigged: "<<action<<search<<value;
#endif
	timer->start(3000);
	return;
}

void AppDBScrollWidget::versionTrigged(short int action, int appid, int verid){
#ifdef DEBUG
	qDebug()<<"[ii] verTrigged: "<<action<<appid<<verid;
#endif
	timer->start(3000);
	return;
}

void AppDBScrollWidget::update(void){
	//appdbHeader->clear();
	//this->clear();
	switch (this->_ACTION){
	case 1:
		this->clear();
		appdbHeader->clear();

		XmlParser *xmlparser = new XmlParser("/home/brezerk/develop/q4wine/templates/app-search.xml");
		appdbHeader->createPagesList(xmlparser->_PAGE_COUNT, xmlparser->_PAGE_CURRENT, this->_SEARCH);

		for (int i=0; i<xmlparser->_APPDB_SEARCH_INFO.count(); i++){
			this->addSearchWidget(xmlparser->_APPDB_SEARCH_INFO.at(i));
		}

		this->insertStretch();
		break;
	}
	timer->stop();
}
