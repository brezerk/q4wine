/***************************************************************************
 *   Copyright (C) 2008-2017 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

    //Init AppDB Core Classes
    xmlparser.reset(new XmlParser());
    httpcore.reset(new HttpCore());

    //Init delay timer
    timer.reset(new QTimer(this));

    this->createActions();

    std::unique_ptr<QToolBar> toolbar (new QToolBar());
    toolbar->setIconSize(QSize(24, 24));
    toolbar->addAction(appdbOpen.get());
    toolbar->addAction(appdbAppPage.get());
    toolbar->addSeparator();
    toolbar->addAction(appdbCat.get());
    toolbar->addSeparator();
    toolbar->addAction(appdbClear.get());
    toolbar->addAction(appdbClearSearch.get());
    searchField.reset (new QLineEdit(this));
    connect(searchField.get(), SIGNAL(returnPressed()), this, SLOT(appdbSearch_Click()));

    toolbar->addWidget(searchField.get());
    toolbar->addAction(appdbSearch.get());

    //Init custom widgets
    appdbHeader.reset(new AppDBHeaderWidget());
    connect(appdbHeader.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SLOT(itemTrigged(short int, QString, int, int, int)));
    appdbScrollArea.reset(new AppDBScrollWidget());
    connect(appdbScrollArea.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SLOT(itemTrigged(short int, QString, int, int, int)));

    //Add custom widgets to mail layout
    std::unique_ptr<QVBoxLayout> contentLayout(new QVBoxLayout(this));
    contentLayout->setMargin(0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(toolbar.release());
    contentLayout->addWidget(appdbHeader.get());
    contentLayout->addWidget(appdbScrollArea.get());
    this->setLayout(contentLayout.release());

    //Connect slots and signals
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(timer_timeout()));
    connect(httpcore.get(), SIGNAL(pageReaded()), this, SLOT(httpcore_pageDownloaded()));
    connect(httpcore.get(), SIGNAL(requestError(QString)), this, SLOT(requestError(QString)));
    connect(httpcore.get(), SIGNAL(updateDataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));
    connect(httpcore.get(), SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));

    this->appdbHeader->addLabel(tr("Status: Ready"));
    timer->stop();

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
        this->setCursor(QCursor(Qt::WaitCursor));
        appdbClear->setEnabled(false);
        appdbAppPage->setEnabled(false);
        this->appdbHeader->clear();
        this->appdbHeader->addLabel(tr("Status: Connecting to %1").arg(APPDB_HOSTNAME));
        this->search=search;
        this->page=0;
        this->appid=0;
        this->verid=0;
        this->testid=0;
        this->catid=0;
        this->appdbScrollArea->clear();

        if (searchField->text()!=search)
            searchField->setText(search);

        timer->start(1000);
        break;
 case 2:
        this->setCursor(QCursor(Qt::WaitCursor));
        appdbClear->setEnabled(false);
        appdbAppPage->setEnabled(false);
        this->appdbHeader->clear();
        this->appdbHeader->addLabel(tr("Status: Connecting to %1").arg(APPDB_HOSTNAME));
        this->search=search;
        this->page=val1;
        this->appid=0;
        this->verid=0;
        this->testid=0;
        this->catid=0;
        this->appdbScrollArea->clear();
        timer->start(1000);
        break;
 case 3:
        this->setCursor(QCursor(Qt::WaitCursor));
        appdbClear->setEnabled(false);
        appdbAppPage->setEnabled(false);
        this->appdbHeader->clear();
        this->appdbHeader->addLabel(tr("Status: Connecting to %1").arg(APPDB_HOSTNAME));
        this->search="";
        this->page=0;
        this->appid=val1;
        this->verid=0;
        this->testid=0;
        this->catid=0;
        this->appdbScrollArea->clear();
        timer->start(1000);
        break;
 case 4:
        if (CoreLib->getSetting("AppDB", "useSystemBrowser", false, 1)==1){
            QDesktopServices::openUrl(QUrl(QString("http://%1/objectManager.php?sClass=version&iId=%2").arg("appdb.winehq.org").arg(val2), QUrl::TolerantMode));
        } else {
            this->setCursor(QCursor(Qt::WaitCursor));
            appdbClear->setEnabled(false);
            appdbAppPage->setEnabled(false);
            this->appdbHeader->clear();
            this->appdbHeader->addLabel(tr("Status: Connecting to %1").arg(APPDB_HOSTNAME));
            this->search="";
            this->page=0;
            this->appid=val1;
            this->verid=val2;
            this->testid=val3;
            this->catid=0;
            this->appdbScrollArea->clear();
            timer->start(1000);
        }
        break;
 case 5:
        this->setCursor(QCursor(Qt::WaitCursor));
        appdbClear->setEnabled(false);
        appdbAppPage->setEnabled(false);
        this->appdbHeader->clear();
        this->appdbHeader->addLabel(tr("Status: Connecting to %1").arg(APPDB_HOSTNAME));
        this->search="";
        this->page=0;
        this->appid=0;
        this->verid=0;
        this->testid=0;
        this->catid=val1;
        this->appdbScrollArea->clear();
        timer->start(1000);
        break;
 case 6:
        if (search.contains(QRegExp("^http"))){
            QDesktopServices::openUrl(QUrl(search, QUrl::TolerantMode));
        } else {
            QDesktopServices::openUrl(QUrl(QString("http://%1").arg(search), QUrl::TolerantMode));
        }
        break;
 case 7:
        QDesktopServices::openUrl(QUrl(QString("http://%1/show_bug.cgi?id=%2").arg(BUGS_HOSTNAME).arg(val1), QUrl::TolerantMode));
        break;
    }

    return;
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
        appdbClear->setEnabled(true);
        break;
    case 2:
        appdbHeader->createPagesList(xmlparser->getPageCount(), xmlparser->getPageCurrent(), this->search);
        applist = xmlparser->getAppSearchInfoList();

        for (int i=0; i<applist.count(); i++){
            this->appdbScrollArea->addSearchWidget(applist.at(i));
        }
        this->appdbScrollArea->insertStretch();
        appdbClear->setEnabled(true);
        break;
    case 3:
        appinfo = xmlparser->getAppSearchInfo();
        appdbHeader->createCategoryList(appinfo.categorys);
        appdbHeader->insertStretch();
        this->appdbScrollArea->addSearchWidget(appinfo);
        this->appdbScrollArea->insertStretch();
        appdbClear->setEnabled(true);
        appdbAppPage->setEnabled(true);
        break;
    case 4:
        this->appdbHeader->createCategoryList(xmlparser->getAppSearchInfo().categorys);
        this->appdbHeader->addLabel(">");
        this->appdbHeader->addLink(xmlparser->getAppSearchInfo().name, true, 3, "", xmlparser->getAppSearchInfo().id);
        this->appdbHeader->addLabel(QString("- %1").arg(xmlparser->getAppSearchInfo().appver));
        this->appdbHeader->insertStretch();
        this->appdbScrollArea->addTestWidget(xmlparser->getAppSearchInfo());
        appdbClear->setEnabled(true);
        appdbAppPage->setEnabled(true);
        break;
    case 5:
        this->appdbHeader->createCategoryList(xmlparser->getCategorysList());
        this->appdbHeader->insertStretch();
        this->appdbScrollArea->addVersionFrame(xmlparser->getSubCategorysList(), tr("Subcategories"), 5);
        this->appdbScrollArea->addVersionFrame(xmlparser->getAppsList(), tr("Applications"), 3);
        this->appdbScrollArea->insertStretch();
        appdbClear->setEnabled(true);
        break;

    }
    this->setCursor(QCursor(Qt::ArrowCursor));
    return;
}

void AppDBWidget::showXmlError(int id){
    switch (id){
    case 1:
        this->appdbHeader->addLabel(tr("Error: cannot read data from %1.").arg(APP_WEBSITE));
        break;
    case 2:
        this->appdbHeader->addLabel(tr("Error: wrong or broken XML data. Try again later."));
        break;
    case 3:
        this->appdbHeader->addLabel(tr("Error: wrong or broken AppDB XML version. This might be caused by an outdated application version."));
        break;
    case 4:
        this->appdbHeader->addLabel(tr("Error: XML parse error."));
        break;
    case 6:
        this->appdbHeader->addLabel(tr("Error: unexpected error."));
        break;
    case 404:
        this->appdbHeader->addLabel(tr("Search: No matches found."));
        break;
    }
    this->setCursor(QCursor(Qt::ArrowCursor));
    return;
}

void AppDBWidget::createActions(void){
    appdbOpen.reset(new QAction(CoreLib->loadIcon("wine"), tr("Open AppDB"), this));
    appdbOpen->setStatusTip(tr("Open Wine AppDB web site"));
    connect(appdbOpen.get(), SIGNAL(triggered()), this, SLOT(appdbOpen_Click()));

    appdbAppPage.reset(new QAction(CoreLib->loadIcon("document-preview"), tr("Open App page"), this));
    appdbAppPage->setStatusTip(tr("Open current application page at Wine AppDB web site"));
    appdbAppPage->setEnabled(false);
    connect(appdbAppPage.get(), SIGNAL(triggered()), this, SLOT(appdbAppPage_Click()));

    appdbClear.reset(new QAction(CoreLib->loadIcon("edit-clear-list"), tr("Clear results"),this));
    appdbClear->setStatusTip(tr("Clear results"));
    appdbClear->setEnabled(false);
    connect(appdbClear.get(), SIGNAL(triggered()), this, SLOT(appdbClear_Click()));

    appdbClearSearch.reset(new QAction(CoreLib->loadIcon("edit-clear"), tr("Clear search field"),this));
    appdbClearSearch->setStatusTip(tr("Clear search field"));
    connect(appdbClearSearch.get(), SIGNAL(triggered()), this, SLOT(appdbClearSearch_Click()));

    appdbSearch.reset(new QAction(CoreLib->loadIcon("edit-find"), tr("Search in AppDB"),this));
    appdbSearch->setStatusTip(tr("Search in Wine AppDB"));
    connect(appdbSearch.get(), SIGNAL(triggered()), this, SLOT(appdbSearch_Click()));

    appdbCat.reset(new QAction(CoreLib->loadIcon("view-catalog"), tr("AppDB categories list"),this));
    appdbCat->setStatusTip(tr("View Wine AppDB categories list"));
    connect(appdbCat.get(), SIGNAL(triggered()), this, SLOT(appdbCat_Click()));

    return;
}

void AppDBWidget::requestError(QString error){
    this->appdbHeader->clear();
    this->appdbHeader->addLabel(tr("Error: %1").arg(error));
    return;
}

void AppDBWidget::updateDataReadProgress(int bytesRead, int totalBytes){
    this->appdbHeader->updateFirstLabelText(QString("Downloaded: %1 of %2 bytes").arg(bytesRead).arg(totalBytes));
    return;
}

void AppDBWidget::timer_timeout(void){
    switch (this->action){
 case 1:
        httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, QString("/xmlexport/searchApp/%1/").arg(this->search));
        break;
 case 2:
        httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, QString("/xmlexport/searchApp/%1/%2/").arg(this->search).arg(page));
        break;
 case 3:
        httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, QString("/xmlexport/viewApp/%1/").arg(this->appid));
        break;
 case 4:
        httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, QString("/xmlexport/viewTest/%1/%2/%3/").arg(this->appid).arg(this->verid).arg(this->testid));
        break;
 case 5:
        httpcore->getAppDBXMLPage(APPDB_HOSTNAME, APPDB_PORT, QString("/xmlexport/viewCategory/%1/").arg(this->catid));
        break;
    }
    timer->stop();
}

void AppDBWidget::stateChanged (int state){
    switch (state){
    case 2:
        this->appdbHeader->updateFirstLabelText(tr("Status: Connecting to %1").arg(APPDB_HOSTNAME));
        break;
    case 3:
        this->appdbHeader->updateFirstLabelText(tr("Status: Sending request to the server."));
        break;
    case 4:
        this->appdbHeader->updateFirstLabelText(tr("Status: Reading server's response."));
        break;
    case 6:
        this->appdbHeader->updateFirstLabelText(tr("Status: Closing down connection."));
        this->setCursor(QCursor(Qt::ArrowCursor));
        break;
    case 0:
        this->appdbHeader->updateFirstLabelText(tr("Status: Not connected."));
        this->setCursor(QCursor(Qt::ArrowCursor));
        break;
    }

    return;
}

void AppDBWidget::setFocus(void){
    this->searchField->setFocus(Qt::OtherFocusReason);
    return;
}

void AppDBWidget::appdbOpen_Click(void){
    QDesktopServices::openUrl(QUrl(QString("http://%1/").arg("appdb.winehq.org"), QUrl::TolerantMode));
    return;
}

void AppDBWidget::appdbAppPage_Click(void){
    if (this->verid==0){
        QDesktopServices::openUrl(QUrl(QString("http://%1/objectManager.php?sClass=application&iId=%2").arg("appdb.winehq.org").arg(this->appid), QUrl::TolerantMode));
    } else {
        QDesktopServices::openUrl(QUrl(QString("http://%1/objectManager.php?sClass=version&iId=%2").arg("appdb.winehq.org").arg(this->verid), QUrl::TolerantMode));
    }
    return;
}

void AppDBWidget::appdbClear_Click(void){
    appdbClear->setEnabled(false);
    appdbAppPage->setEnabled(false);
    searchField->clear();
    this->appdbHeader->clear();
    this->appdbHeader->addLabel(tr("Status: Ready"));
    this->appdbScrollArea->clear();
    return;
}

void AppDBWidget::appdbClearSearch_Click(void){
    searchField->clear();
    return;
}

void AppDBWidget::appdbSearch_Click(void){
    if (!searchField->text().isEmpty())
        this->itemTrigged(1, searchField->text());
    return;
}

void AppDBWidget::appdbCat_Click(void){
    this->itemTrigged(5, "", 0);
    return;
}
