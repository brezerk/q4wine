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

#include "appdbwidget.h"

AppDBWidget::AppDBWidget(QWidget *parent) : QWidget(parent)
{
	//Init AppDB Core Classes
	xmlparser.reset(new XmlParser());
	httpcore.reset(new HttpCore());

	//Init delay timer
	timer.reset(new QTimer(this));

	//Init custom widgets
	appdbHeader.reset(new AppDBHeaderWidget());
	connect(appdbHeader.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SLOT(itemTrigged(short int, QString, int, int, int)));
	appdbScrollArea.reset(new AppDBScrollWidget());
	connect(appdbScrollArea.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SLOT(itemTrigged(short int, QString, int, int, int)));

	//Add custom widgets to mail layout
	std::auto_ptr<QVBoxLayout> contentLayout(new QVBoxLayout(this));
	contentLayout->setMargin(0);
	contentLayout->setSpacing(0);
	contentLayout->addWidget(appdbHeader.get());
	contentLayout->addWidget(appdbScrollArea.get());
	this->setLayout(contentLayout.release());

	//Connect slots and signals
	connect(timer.get(), SIGNAL(timeout()), this, SLOT(timer_timeout()));
	connect(httpcore.get(), SIGNAL(pageReaded()), this, SLOT(httpcore_pageDownloaded()));
	connect(httpcore.get(), SIGNAL(requestError(QString)), this, SLOT(requestError(QString)));
	connect(httpcore.get(), SIGNAL(updateDataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));

	this->appdbHeader->addLabel(tr("Status: Ready"));
	timer->stop();

	return;
}

void AppDBWidget::startSearch(short int action, QString search){
	itemTrigged(action, search);
	return;
}

void AppDBWidget::itemTrigged(short int action, QString search, int val1, int val2, int val3){
#ifdef DEBUG
	qDebug()<<"[ii] itemTrigged"<<action<<search<<val1<<val2<<val3;
#endif

	//Set state variables
	this->action=action;

	switch (action){
 case 1:
		this->appdbHeader->clear();
		this->appdbHeader->addLabel(tr("Status: Connectiong to %1").arg(APPDB_HOSTNAME));
		this->search=search;
		this->page=0;
		this->appid=0;
		this->verid=0;
		this->testid=0;
		this->catid=0;
		this->appdbScrollArea->clear();
		break;
 case 2:
		this->appdbHeader->clear();
		this->appdbHeader->addLabel(tr("Status: Connectiong to %1").arg(APPDB_HOSTNAME));
		this->search=search;
		this->page=val1;
		this->appid=0;
		this->verid=0;
		this->testid=0;
		this->catid=0;
		this->appdbScrollArea->clear();
		break;
 case 3:
		this->appdbHeader->clear();
		this->appdbHeader->addLabel(tr("Status: Connectiong to %1").arg(APPDB_HOSTNAME));
		this->search="";
		this->page=0;
		this->appid=val1;
		this->verid=0;
		this->testid=0;
		this->catid=0;
		this->appdbScrollArea->clear();
		break;
 case 4:
		this->appdbHeader->clear();
		this->appdbHeader->addLabel(tr("Status: Connectiong to %1").arg(APPDB_HOSTNAME));
		this->search="";
		this->page=0;
		this->appid=val1;
		this->verid=val2;
		this->testid=val3;
		this->catid=0;
		this->appdbScrollArea->clear();
		break;
 case 5:
		this->appdbHeader->clear();
		this->appdbHeader->addLabel(tr("Status: Connectiong to %1").arg(APPDB_HOSTNAME));
		this->search="";
		this->page=0;
		this->appid=0;
		this->verid=0;
		this->testid=0;
		this->catid=val1;
		this->appdbScrollArea->clear();
		break;
 case 6:
		//FIXME: openurl!
		emit(xdgOpenUrl(search));
		break;
	}

	//run delay timer
	timer->start(1000);
	return;
}

void AppDBWidget::timer_timeout(void){
	switch (this->action){
 case 1:
		httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=1&search=%1").arg(this->search));
		break;
 case 2:
		httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=1&search=%1&page=%2").arg(this->search).arg(page));
		break;
 case 3:
		httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=3&appid=%1").arg(this->appid));
		break;
 case 4:
		httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=4&appid=%1&verid=%2&testid=%3").arg(this->appid).arg(this->verid).arg(this->testid));
		break;
 case 5:
		httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, "/xmlexport/index.php", QString("action=5&catid=%1").arg(this->catid));
		break;
	}
	timer->stop();
}

void AppDBWidget::httpcore_pageDownloaded(void){
	int ret=0;
	QList<WineAppDBInfo> applist;
	WineAppDBInfo appinfo;
	this->appdbHeader->clear();

	ret = xmlparser->parseIOSream(httpcore->getXMLReply());
	if (ret>0){
		this->showXmlError(ret);
		return;
	}

	switch (this->action){
	case 1:
		appdbHeader->createPagesList(xmlparser->getPageCount(), xmlparser->getPageCurrent(), this->search);
		applist = xmlparser->getAppSearchInfoList();

		for (int i=0; i<applist.count(); i++){
			this->appdbScrollArea->addSearchWidget(applist.at(i));
		}
		this->appdbScrollArea->insertStretch();
		break;
	case 2:
		appdbHeader->createPagesList(xmlparser->getPageCount(), xmlparser->getPageCurrent(), this->search);
		applist = xmlparser->getAppSearchInfoList();

		for (int i=0; i<applist.count(); i++){
			this->appdbScrollArea->addSearchWidget(applist.at(i));
		}
		this->appdbScrollArea->insertStretch();
		break;
	case 3:
		appinfo = xmlparser->getAppSearchInfo();
		appdbHeader->createCategoryList(appinfo.categorys);
		appdbHeader->insertStretch();
		this->appdbScrollArea->addSearchWidget(appinfo);
		this->appdbScrollArea->insertStretch();
		break;
	case 4:
		this->appdbHeader->createCategoryList(xmlparser->getAppSearchInfo().categorys);
		this->appdbHeader->addLabel(">");
		this->appdbHeader->addLink(xmlparser->getAppSearchInfo().name, true, 3, "", xmlparser->getAppSearchInfo().id);
		this->appdbHeader->addLabel(QString("- %1").arg(xmlparser->getAppSearchInfo().appver));
		this->appdbHeader->insertStretch();
		this->appdbScrollArea->addTestWidget(xmlparser->getAppSearchInfo());
		break;
	case 5:
		this->appdbHeader->createCategoryList(xmlparser->getCategorysList());
		this->appdbHeader->insertStretch();
		this->appdbScrollArea->addVersionFrame(xmlparser->getSubCategorysList(), tr("Sub categoryes"), 5);
		this->appdbScrollArea->addVersionFrame(xmlparser->getAppsList(), tr("Applications"), 3);
		this->appdbScrollArea->insertStretch();
		break;

	}
	return;
}

void AppDBWidget::showXmlError(int id){
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
 case 6:
		this->appdbHeader->addLabel(tr("No matches found"));
		return;
		break;
	}
}

void AppDBWidget::requestError(QString error){
	this->appdbHeader->clear();
	this->appdbHeader->addLabel(tr("Error: %1").arg(error));
}

void AppDBWidget::updateDataReadProgress(int bytesRead, int totalBytes){
	this->appdbHeader->updateFirstLabelText(QString("Downloaded: %1 of %2 bytes").arg(bytesRead).arg(totalBytes));
	return;
}
