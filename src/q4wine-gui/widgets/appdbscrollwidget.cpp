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
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->setWidgetResizable(true);

	xmlparser.reset(new XmlParser());
	httpcore.reset(new HttpCore());
	timer.reset(new QTimer(this));

	contentWidget.reset(new QWidget());
	contentLayout.reset(new QVBoxLayout(contentWidget.get()));
	contentLayout->setMargin(3);
	this->setWidget(contentWidget.get());

	//FIXME: See no 8 draft page 133-253
	this->appdbHeader=appdbHeader;
	this->appdbHeader->addLabel("Status: Ready");


	connect(timer.get(), SIGNAL(timeout()), this, SLOT(update()));
	connect(appdbHeader, SIGNAL(linkTrigged(short int, QString, int, int)), this, SLOT(linkTrigged(short int, QString, int, int)));
	connect(httpcore.get(), SIGNAL(pageReaded()), this, SLOT(pageReaded()));
	timer->stop();

	return;
}

void AppDBScrollWidget::addSearchWidget(const WineAppDBInfo appinfo){
	if (contentLayout.get()){
		std::auto_ptr<AppDBSearchWidget> AppDBWidget(new AppDBSearchWidget(appinfo.name, appinfo.desc, appinfo.id, appinfo.versions));
		connect(AppDBWidget.get(), SIGNAL(versionTrigged(short int, int, int, int)), this, SLOT(versionTrigged(short int, int, int, int)));
		contentLayout->addWidget(AppDBWidget.release());
	}
	return;
}

void AppDBScrollWidget::addTestWidget(const WineAppDBInfo *appinfo){
	if (contentLayout.get()){
		std::auto_ptr<AppDBTestViewWidget> AppDBTestWidget (new AppDBTestViewWidget(appinfo));
		AppDBTestWidget->setObjectName("appViewTestWidget");
		connect(AppDBTestWidget.get(), SIGNAL(linkTrigged(short int, QString, int, int)), this, SLOT(linkTrigged(short int, QString, int, int)));
		connect(AppDBTestWidget.get(), SIGNAL(versionTrigged(short int, int, int, int)), this, SLOT(versionTrigged(short int, int, int, int)));
		contentLayout->addWidget(AppDBTestWidget.release());
	}
	return;
}

void AppDBScrollWidget::gotoCommentId(int id){
	//FIXME: Use a signal\slot MODEL!!!!!!!!!!!!!!!!!!!
	//int y_pos = AppDBTestWidget->selectParentCommentById(id);
	//if (y_pos>0)
	//	this->ensureVisible(0, y_pos, 0, this->height()/3);
}

void AppDBScrollWidget::clear(void){
	if (contentWidget.get()){
		QList<QObject*> list = contentWidget->children();
		// Start from 1 becouse of 0 -- is VBoxLayout
		for (int i=1; i<list.count(); i++){
#ifdef DEBUG
			qDebug()<<"[ii] Shedule QObject for deletetion. object type is:"<<list.at(i)->metaObject()->className();
#endif
			list.at(i)->setProperty("visible", false);
			list.at(i)->disconnect();
			list.at(i)->deleteLater();
		}
	}
	return;
}

void AppDBScrollWidget::insertStretch(void){
	if (contentLayout.get()){
		std::auto_ptr<QWidget> visibleStrech(new QWidget());
		visibleStrech->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		contentLayout->addWidget(visibleStrech.release());
	}
}

void AppDBScrollWidget::startSearch(QString search){
	this->appdbHeader->clear();
	this->appdbHeader->addLabel("Status: Connecting to appqb.winehq.org...");
	this->clear();

#ifdef DEBUG
	qDebug()<<"[ii] startSearch: "<<1<<search;
#endif
	this->_ACTION=1;
	this->_SEARCH=search;
	timer->start(1000);
}

void AppDBScrollWidget::linkTrigged(short int action, QString search, int val1, int val2){
#ifdef DEBUG
	qDebug()<<"[ii] linkTrigged: "<<action<<search<<val1<<val2;
#endif
	switch (action){
 case 7:
		//Get TestWidget....
		gotoCommentId(val1);
		break;
 default:
	if (action==2){
			this->page=val1;
	} else if (action==5){
		this->catid=val1;
}
	this->appdbHeader->clear();
	this->clear();
	this->appdbHeader->addLabel("Status: Connecting to appqb.winehq.org...");
	this->_ACTION=action;
	timer->start(1000);
break;
}
	return;

}

void AppDBScrollWidget::versionTrigged(short int action, int appid, int verid, int testid){
#ifdef DEBUG
	qDebug()<<"[ii] verTrigged: "<<action<<appid<<verid<<testid;
#endif
	switch (action){
 case 7:
		//Get TestWidget....
		break;
 default:
		if (action==5){
			this->catid=appid;
		} else {
			this->appid = appid;
		}

		this->verid=verid;
		this->testid=testid;

		this->appdbHeader->clear();
		this->appdbHeader->addLabel(tr("Status: Connecting to %1...").arg(APPDB_HOSTNAME));
		this->clear();
		this->_ACTION=action;
		timer->start(1000);
		break;
	}


	return;
}

void AppDBScrollWidget::update(void){
	switch (this->_ACTION){
 case 1:
		httpcore->getWineAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=1&search=%1").arg(this->_SEARCH));
		break;
 case 2:
		httpcore->getWineAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=1&search=%1&page=%2").arg(this->_SEARCH).arg(page));
		break;
 case 3:
		httpcore->getWineAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=3&appid=%1").arg(this->appid));
		break;
 case 4:
		httpcore->getWineAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=4&appid=%1&verid=%2&testid=%3").arg(this->appid).arg(this->verid).arg(this->testid));
		break;
 case 5:
		httpcore->getWineAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=5&catid=%1").arg(this->catid));
		break;
	}
	timer->stop();
}


void AppDBScrollWidget::pageReaded(void){
	int ret=0;
	QList<WineAppDBInfo> applist;
	appdbHeader->clear();

	ret = xmlparser->parseIOSream(httpcore->getXMLReply());
	if (ret>0){
		this->showXmlError(ret);
		return;
	}

	switch (this->_ACTION){
	case 1:
		appdbHeader->createPagesList(xmlparser->getPageCount(), xmlparser->getPageCurrent(), this->_SEARCH);
		applist = xmlparser->getAppSearchInfoList();

		for (int i=0; i<applist.count(); i++){
			this->addSearchWidget(applist.at(i));
		}
		this->insertStretch();
		break;
	case 2:
		appdbHeader->createPagesList(xmlparser->getPageCount(), xmlparser->getPageCurrent(), this->_SEARCH);

		applist = xmlparser->getAppSearchInfoList();
		for (int i=0; i<xmlparser->getAppSearchInfoList().count(); i++){
			this->addSearchWidget(applist.at(i));
		}
		this->insertStretch();
		break;
	case 3:
		appdbHeader->createCategoryList(&xmlparser->getAppSearchInfo().categorys);
		this->addSearchWidget(xmlparser->getAppSearchInfo());
		appdbHeader->insertStretch();
		this->insertStretch();
		break;
	case 4:
		this->addTestWidget(&xmlparser->getAppSearchInfo());
		appdbHeader->createCategoryList(&xmlparser->getAppSearchInfo().categorys);
		appdbHeader->addLabel(">");
		appdbHeader->addLink(xmlparser->getAppSearchInfo().name, true, 3, "", xmlparser->getAppSearchInfo().id);
		appdbHeader->addLabel(QString("- %1").arg(xmlparser->getAppSearchInfo().appver));
		appdbHeader->insertStretch();
		break;
	case 5:
		appdbHeader->createCategoryList(&xmlparser->getCategorysList());
		this->addVersionFrame(xmlparser->getSubCategorysList(), tr("Sub categoryes"), 5);
		this->addVersionFrame(xmlparser->getAppsList(), tr("Applications"), 3);
		appdbHeader->insertStretch();
		this->insertStretch();
		break;
	}
	return;
}

void AppDBScrollWidget::addVersionFrame(QList<WineAppDBCategory> list, QString frame_caption, short int action){
	if (list.count()<=0)
		return;

	std::auto_ptr<QGroupBox> frame(new QGroupBox(QString("%1:").arg(frame_caption)));
	std::auto_ptr<QVBoxLayout> layout(new QVBoxLayout());


	for (int i=0; i<list.count(); i++){
		std::auto_ptr<AppDBAppVersionWidget> version(new AppDBAppVersionWidget(action));
		version->setAppId(list.at(i).id);
		version->addLabel(list.at(i).name, 240, 3);
		version->addLabel(list.at(i).desc, -1, 3, true);
		connect(version.get(), SIGNAL(versionTrigged(short int, int, int, int)), this, SLOT(versionTrigged(short int, int, int, int)));
		layout->addWidget(version.release());
	}

	frame->setLayout(layout.release());
	contentLayout->addWidget(frame.release());
	return;
}

void AppDBScrollWidget::showXmlError(int id){
	switch (id){
		case 1:
		this->appdbHeader->addLabel(tr("Error: can't read data from appqb.winehq.org."));
		return;
		break;
		case 2:
		this->appdbHeader->addLabel(tr("Error: wrong or broken xml data. Try again later."));
		return;
		break;
		case 3:
		this->appdbHeader->addLabel(tr("Error: wrong or broken appdb xml version. Application needs to be updated?"));
		return;
		case 4:
		this->appdbHeader->addLabel(tr("Error: xml parse error."));
		return;
		break;
	}
}
