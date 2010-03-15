/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                           *
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

#include "mainwindow.h"

MainWindow::MainWindow(int startState, QString run_binary, QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f){

	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	clearTmp();
	// Base GUI setup
	setupUi(this);

    if (!this->createSocket()){
        this->close();
        return;
     }

	if (startState == 1)
		this->showMinimized();

	setWindowTitle(tr("%1 :. Qt4 GUI for Wine v%2").arg(APP_NAME) .arg(APP_VERS));

	cbPrefixes.reset(new QComboBox(tabPrefixSeup));
	cbPrefixes->setMinimumWidth(180);
	std::auto_ptr<QToolBar> toolbar (new QToolBar(tabPrefixSeup));
	std::auto_ptr<QLabel> label (new QLabel(tr(" Current prefix: ")));

	toolbar->addWidget(label.release());
	toolbar->addWidget(cbPrefixes.get());

	std::auto_ptr<QAction> action (new QAction(CoreLib->loadIcon("data/configure.png"), tr("Manage prefixes"), this));
	action->setStatusTip(tr("Manage prefixes"));
	connect(action.get(), SIGNAL(triggered()), this, SLOT(prefixManage_Click()));

	toolbar->addAction(action.release());
	toolbar->addSeparator();

    action.reset(new QAction(CoreLib->loadIcon("data/regedit.png"), tr("Run winetriks plugin"), this));
    action->setStatusTip(tr("Run winetriks plugin (wine manage utility created by DanKegel)"));
	connect(action.get(), SIGNAL(triggered()), this, SLOT(prefixRunWinetriks_Click()));

	toolbar->addAction(action.release());

	std::auto_ptr<QVBoxLayout> vlayout (new QVBoxLayout);
	vlayout->addWidget(toolbar.release());
	vlayout->addWidget(frame);
	vlayout->setMargin(0);
	vlayout->setSpacing(0);
	tabPrefixSeup->setLayout(vlayout.release());

	frame->setAutoFillBackground(true);

    std::auto_ptr<LoggingWidget> logWidget (new LoggingWidget(tabLogging));
    connect (this, SIGNAL(reloadLogData()), logWidget.get(), SLOT(getLogRecords()));

     if (CoreLib->getSetting("logging", "enable", false, 1).toInt()==1){
         logWidget->getLogRecords();
     } else {
         tabLogging->setEnabled(false);
     }

     logLayout->addWidget(logWidget.release());

	std::auto_ptr<IconListWidget> lstIcons (new IconListWidget(tabPrograms));
	connect(lstIcons.get(), SIGNAL(iconItemClick(QString, QString, QString, QString, QString)), this, SLOT(updateIconDesc(QString, QString, QString, QString, QString)));
	connect(lstIcons.get(), SIGNAL(changeStatusText(QString)), this, SLOT(changeStatusText(QString)));
	connect(txtIconFilter, SIGNAL(textChanged(QString)), lstIcons.get(), SLOT(setFilterString(QString)));
	connect(lstIcons.get(), SIGNAL(searchRequest(QString)), this, SLOT(searchRequest(QString)));

	std::auto_ptr<PrefixTreeWidget> twPrograms (new PrefixTreeWidget(tabPrograms));
	connect(this, SIGNAL(updateDatabaseConnections()), twPrograms.get(), SLOT(getPrefixes()));
    connect(twPrograms.get(), SIGNAL(showFolderContents(QString, QString)), lstIcons.get(), SLOT(showFolderContents(QString, QString)));
    connect(twPrograms.get(), SIGNAL(setSearchFocus()), this, SLOT(setSearchFocus()));
    connect(twPrograms.get(), SIGNAL(changeStatusText(QString)), this, SLOT(changeStatusText(QString)));
	connect(this, SIGNAL(setDefaultFocus(QString, QString)), twPrograms.get(), SLOT(setDefaultFocus(QString, QString)));
	connect(cbPrefixes.get(), SIGNAL(currentIndexChanged(QString)), twPrograms.get(), SLOT(setDefaultFocus(QString)));
	connect(twPrograms.get(), SIGNAL(prefixIndexChanged(QString)), this, SLOT(setcbPrefixesIndex(QString)));

	std::auto_ptr<WineProcessWidget> procWidget (new WineProcessWidget(tabProcess));
	connect(this, SIGNAL(stopProcTimer()), procWidget.get(), SLOT(stopTimer()));
	connect(this, SIGNAL(startProcTimer()), procWidget.get(), SLOT(startTimer()));
	connect(procWidget.get(), SIGNAL(changeStatusText(QString)), this, SLOT(changeStatusText(QString)));
	tabProcessLayout->addWidget(procWidget.release());

	std::auto_ptr<PrefixControlWidget> prefixWidget (new PrefixControlWidget(tabPrefix));
	connect(prefixWidget.get(), SIGNAL(updateDatabaseConnections()), twPrograms.get(), SLOT(getPrefixes()));
	connect(prefixWidget.get(), SIGNAL(updateDatabaseConnections()), this, SLOT(updateDtabaseConnectedItems()));
	connect(cbPrefixes.get(), SIGNAL(currentIndexChanged(QString)), prefixWidget.get(), SLOT(setDefaultFocus(QString)));
	connect(prefixWidget.get(), SIGNAL(prefixIndexChanged(QString)), this, SLOT(setcbPrefixesIndex(QString)));

	std::auto_ptr<QWidget> wid (new QWidget(tabPrograms));

	vlayout.reset(new QVBoxLayout);
    vlayout->addWidget(widgetFilter);
    vlayout->addWidget(lstIcons.release());
    vlayout->setMargin(0);
	wid->setLayout(vlayout.release());

	splitter.reset(new QSplitter(tabPrograms));
	splitter->addWidget(twPrograms.release());
	splitter->addWidget(wid.release());

	vlayout.reset(new QVBoxLayout);
	vlayout->addWidget(splitter.get());
	vlayout->addWidget(gbInfo);
	vlayout->setMargin(3);
	tabPrograms->setLayout(vlayout.release());
	tabPrefixLayout->addWidget(prefixWidget.release());

	// Updating database connected items
	updateDtabaseConnectedItems();

	// Getting settings from config file
	this->createTrayIcon();
	this->getSettings();

	connect(tbwGeneral, SIGNAL(currentChanged(int)), this, SLOT(tbwGeneral_CurrentTabChange(int)));
	connect(cmdCreateFake, SIGNAL(clicked()), this, SLOT(cmdCreateFake_Click()));
	connect(cmdUpdateFake, SIGNAL(clicked()), this, SLOT(cmdUpdateFake_Click()));
	connect(cmdClearFilter, SIGNAL(clicked()), this, SLOT(cmdClearFilter_Click()));

	//Main menu actions connection to slots
	connect(mainRun, SIGNAL(triggered()), this, SLOT(mainRun_Click()));
	connect(mainPrograms, SIGNAL(triggered()), this, SLOT(mainPrograms_Click()));
    connect(mainLogging, SIGNAL(triggered()), this, SLOT(mainLogging_Click()));
	connect(mainProcess, SIGNAL(triggered()), this, SLOT(mainProcess_Click()));
	connect(mainSetup, SIGNAL(triggered()), this, SLOT(mainSetup_Click()));
	connect(mainPrefix, SIGNAL(triggered()), this, SLOT(mainPrefix_Click()));
	connect(mainImageManage, SIGNAL(triggered()), this, SLOT(mainImageManager_Click()));
	connect(mainAbout, SIGNAL(triggered()), this, SLOT(mainAbout_Click()));
	connect(mainAboutQt, SIGNAL(triggered()), this, SLOT(mainAboutQt_Click()));
	connect(mainExportIcons, SIGNAL(triggered()), this, SLOT(mainExportIcons_Click()));
	connect(mainFirstSteps, SIGNAL(triggered()), this, SLOT(mainFirstSteps_Click()));
	connect(mainFAQ, SIGNAL(triggered()), this, SLOT(mainFAQ_Click()));
	connect(mainIndex, SIGNAL(triggered()), this, SLOT(mainIndex_Click()));
	connect(mainWebsite, SIGNAL(triggered()), this, SLOT(mainWebsite_Click()));
	connect(mainDonate, SIGNAL(triggered()), this, SLOT(mainDonate_Click()));
	connect(mainBugs, SIGNAL(triggered()), this, SLOT(mainBugs_Click()));
	connect(mainOptions, SIGNAL(triggered()), this, SLOT(mainOptions_Click()));
	connect(mainInstall, SIGNAL(triggered()), this, SLOT(mainInstall_Click()));
	connect(mainExit, SIGNAL(triggered()), this, SLOT(mainExit_Click()));



	CoreLib->runAutostart();

#ifndef WITH_ICOUTILS
	mainExportIcons->setEnabled(false);
#endif

#ifndef WITH_WINEAPPDB
	mainAppDB->setEnabled(false);
	tabAppDB->deleteLater();
#else
	connect(mainAppDB, SIGNAL(triggered()), this, SLOT(mainAppDB_Click()));
	// Creating AppDBScrollWidget and place it into frameAppDBWidget layout
	appdbWidget.reset(new AppDBWidget(this));
	connect (this, SIGNAL(appdbWidget_startSearch(short int, QString)), appdbWidget.get(), SLOT(itemTrigged(short int, QString)));
    connect (this, SIGNAL(setAppDBFocus()), appdbWidget.get(), SLOT(setFocus()));
	tabAppDBLayout->addWidget(appdbWidget.release());   
#endif

        if (!run_binary.isEmpty())
            messageReceived(run_binary);

        if (!trayIcon->isVisible())
            show();



	return;
}

void MainWindow::setSearchFocus(){
    txtIconFilter->setFocus(Qt::OtherFocusReason);
    return;
}

void MainWindow::clearTmp(){
	QString fileName = QDir::homePath();
	fileName.append("/.config/");
	fileName.append(APP_SHORT_NAME);
	fileName.append("/tmp/");

	QDir dir(fileName);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFile(list.at(i).absoluteFilePath()).remove();
	}
	return;
}

void MainWindow::prefixRunWinetriks_Click() {
#ifndef WITH_WINETRIKS
	QMessageBox::warning(this, tr("Warning"), tr("<p>q4wine was compiled without winetriks support.</p><p>If you wish to enable winetriks support add:</p><p> \"-DWITH_WINETRIKS=ON\" to cmake arguments.</p>"));
#else
	QMessageBox::warning(this, tr("Warning"), tr("<p>Winetricks officaly NOT supported by q4wine.</p><p>There was some repports about bugs, slows and errors on winetriks and q4wine usage at same time.</p>"));

	if (CoreLib->getSetting("console", "bin").toString().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("<p>You do not set default console binary.</p><p>Set it into q4wine option dialog.</p>"));
		return;
	}
	winetricks triks(cbPrefixes->currentText());
	triks.exec();
#endif

	return;
}

void MainWindow::trayIcon_Activate(QSystemTrayIcon::ActivationReason reason){
	if (reason==QSystemTrayIcon::Trigger){
		if (!isVisible()){
			setMeVisible(TRUE);
		} else {
			setMeVisible(FALSE);
		}
	}
	return;
}

void MainWindow::updateIconDesc(QString program, QString args, QString desc, QString console, QString desktop){
	/*
	 * This is function for selection icons, and displaying
	 * icon informationm like path and description
	 */

	lblIconInfo0->setText(tr("Program: %1<br> Args: %2").arg(program) .arg(args));
	lblIconInfo2->setText(tr("Description: %1").arg(desc));

	QString useconsole="";
	if (console=="1"){
		useconsole=tr("Yes");
	} else {
		useconsole=tr("No");
	}

	QString desktopsize="";
	if (desktop.isEmpty()){
		desktopsize = tr("Default");
	} else {
		desktopsize = desktop;
	}

	lblIconInfo1->setText(tr("Runs in console: %1<br> Desktop size: %2").arg(useconsole) .arg(desktopsize));

	return;
}

void MainWindow::getSettings(){
	/*
	 * Getting application settings
	 */
	QVariant val;

	val = CoreLib->getSetting("MainWindow", "size", false, QSize(400, 450));
	this->resize(val.toSize());
	val = CoreLib->getSetting("MainWindow", "pos", false, QPoint(200, 200));
	this->move(val.toPoint());

	QList<int> a;
	a.append(CoreLib->getSetting("MainWindow", "splitterSize0", false, 170).toInt());
	a.append(CoreLib->getSetting("MainWindow", "splitterSize1", false, 379).toInt());

	splitter->setSizes(a);

	if (CoreLib->getSetting("app", "showTrareyIcon", false).toBool()){
		trayIcon->show();
	} else {
		trayIcon->hide();
	}

	CoreLib->checkSettings();

	emit(setDefaultFocus(CoreLib->getSetting("LastPrefix", "prefix", false).toString(), CoreLib->getSetting("LastPrefix", "dir", false).toString()));

	return;
}

void MainWindow::changeStatusText(QString text){
	statusBar()->showMessage(text);
}

void MainWindow::updateDtabaseConnectedItems(){
	QString curPrefix =  cbPrefixes->currentText();

	cbPrefixes->clear();
    QStringList list = db_prefix.getPrefixList();
    for (int i = 0; i < list.size(); ++i) {
        cbPrefixes->addItem (list.at(i));
	}

	emit(updateDatabaseConnections());

	if (!curPrefix.isEmpty()){
		cbPrefixes->setCurrentIndex(cbPrefixes->findText(curPrefix));
	}
	return;
}

#ifdef WITH_WINEAPPDB
void MainWindow::searchRequest(QString search){
	tbwGeneral->setCurrentIndex (4);
	emit(appdbWidget_startSearch(1, search));
}
#endif

void MainWindow::prefixManage_Click(){
	tbwGeneral->setCurrentIndex (3);
	return;
}

bool MainWindow::createSocket(){
    serverSoket.reset(new QLocalServer(this));
#ifdef DEBUG
    qDebug()<<"[ii] Creating q4wine socket";
#endif

    char *user = getenv("USER");

    QString soketFile = QString("/tmp/q4wine-%1.sock").arg(user);

    if (QFile(soketFile).exists()){
        QMessageBox msgBox;
        msgBox.setText(tr("Socket file \"%1\" already exists!").arg(soketFile));
        msgBox.setInformativeText("It seems another instance of q4wine is running, or q4wine was shutdown incorrectly. Do you want to remove it?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        if (msgBox.exec() == QMessageBox::Ok)
            QFile(soketFile).remove();
    }

    if (!serverSoket->listen(soketFile)){
        QTextStream QErr(stderr);
        QErr<<"[EE] Can't create q4wine socket: "<<serverSoket->errorString()<<endl;
        return false;
    }

    connect (serverSoket.get(), SIGNAL(newConnection()), this, SLOT(newConnection()));

    return true;
}

void MainWindow::newConnection (){

    if (serverSoket->hasPendingConnections ()){
        std::auto_ptr<QLocalSocket> localSocket;
        localSocket.reset(serverSoket->nextPendingConnection ());

        connect(localSocket.get(), SIGNAL(disconnected()), localSocket.get(), SLOT(deleteLater()));

        while (localSocket->waitForReadyRead()){
            QDataStream in(localSocket.get());
            in.setVersion(QDataStream::Qt_4_0);


            quint16 blockSize = 0;
            if (blockSize == 0) {
                if (localSocket->bytesAvailable() < (int)sizeof(quint16))
                    return;
                in >> blockSize;
            }

            if (in.atEnd())
                return;

            QString message;
            in >> message;

            QStringList list = message.split("/");

            if (list.count()>1){
                if (list.at(0)=="start"){
                    if (list.count()==3){
                        this->changeStatusText(tr("Application: \"%1\" started fine for prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));

                        if ((!this->isVisible()) and (trayIcon->isVisible())){
                            trayIcon->showMessage(tr("q4wine-helper"), tr("Application: \"%1\" started fine for prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));
                        }

                        emit(reloadLogData());

                    } else {
                        this->showSocketError(message);
                    }
                } else if (list.at(0)=="console"){
                    if (list.count()==3){
                        this->changeStatusText(tr("Console started fine for Application: \"%1\" in prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));

                        if ((!this->isVisible()) and (trayIcon->isVisible())){
                            trayIcon->showMessage(tr("q4wine-helper"), tr("Application: \"%1\" started fine for prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));
                        }

                        emit(reloadLogData());

                    } else {
                        this->showSocketError(message);
                    }
                } else if (list.at(0)=="finish"){
                    if (list.count()==4){
                        this->changeStatusText(tr("Application: \"%1\" finished for prefix: \"%2\". Exit code is: \"%3\".").arg(list.at(1)).arg(list.at(2)).arg(list.at(3)));
                        if ((!this->isVisible()) and (trayIcon->isVisible())){
                            trayIcon->showMessage(tr("q4wine-helper"), tr("Application: \"%1\" finished for prefix: \"%2\". Exit code is: \"%3\".").arg(list.at(1)).arg(list.at(2)).arg(list.at(3)));
                        }

                        emit(reloadLogData());
                    } else {
                        this->showSocketError(message);
                    }
                } else if (list.at(0)=="error"){
                    if (list.count()==3){
                        this->changeStatusText(tr("Can't start application: \"%1\" for prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));
                        if ((!this->isVisible()) and (trayIcon->isVisible())){
                            trayIcon->showMessage(tr("q4wine-helper"), tr("Can't start application: \"%1\" for prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));
                        }
                    } else {
                        this->showSocketError(message);
                    }
                } else if (list.at(0)=="console_error"){
                    if (list.count()==3){
                        this->changeStatusText(tr("Can't start console for application: \"%1\" in prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));
                        if ((!this->isVisible()) and (trayIcon->isVisible())){
                            trayIcon->showMessage(tr("q4wine-helper"), tr("Can't start console for application: \"%1\" in prefix: \"%2\".").arg(list.at(1)).arg(list.at(2)));
                        }
                    } else {
                        this->showSocketError(message);
                    }
                }
            } else {
                this->showSocketError(message);
            }

#ifdef DEBUG
            qDebug()<<"[ii] Socket message:"<<message;
#endif
        }
    }
    return;
}

void MainWindow::showSocketError(QString message){
    QTextStream QErr(stderr);
    QErr<<"[ee] Unexpected socket message: "<<message;
    this->changeStatusText(tr("Unexpected socket message recived."));
    return;
}

void MainWindow::createTrayIcon(){
	std::auto_ptr<QMenu> trayIconMenu(new QMenu(this));
	trayIconMenu->addAction(mainRun);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainExportIcons);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainImageManage);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainPrograms);
	trayIconMenu->addAction(mainProcess);
    trayIconMenu->addAction(mainLogging);
	trayIconMenu->addAction(mainSetup);
	trayIconMenu->addAction(mainPrefix);
	trayIconMenu->addAction(mainAppDB);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainExit);

	trayIcon.reset(new QSystemTrayIcon(this));
	trayIcon->setContextMenu(trayIconMenu.release());

	QIcon icon = CoreLib->loadIcon("data/q4wine.png");

	trayIcon->setIcon(icon);
	setWindowIcon(icon);

	if (CoreLib->getSetting("app", "showTrareyIcon", false).toBool()){
        QApplication::setQuitOnLastWindowClosed(false);
		trayIcon->show();
	} else {
        QApplication::setQuitOnLastWindowClosed(true);
		trayIcon->hide();
	}

	connect(trayIcon.get(), SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIcon_Activate(QSystemTrayIcon::ActivationReason)));
	return;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if (trayIcon->isVisible()) {
		hide();
		event->ignore();
	} else {
        this->mainExit_Click();
	}
	return;
}

void MainWindow::tbwGeneral_CurrentTabChange(int tabIndex){
	switch (tabIndex){
    case 0:
        emit(stopProcTimer());
        setSearchFocus();
        break;
 case 1:
		//Initiate /proc reading
		emit(startProcTimer());
		break;
#ifdef WITH_WINEAPPDB
    case 5:
        emit(stopProcTimer());
        emit(setAppDBFocus());
        break;
#endif
 default:
		emit(stopProcTimer());
		break;
	}

	return;
}

void MainWindow::cmdCreateFake_Click(){

	QString prefixPath = db_prefix.getPath(cbPrefixes->currentText());
	QString sysregPath;
	sysregPath.append(prefixPath);
	sysregPath.append("/system.reg");

	QFile sysreg_file (sysregPath);

	if (sysreg_file.exists()){
		if (QMessageBox::warning(this, tr("Warning"), tr("There are other Fake drive installed in %1 prefix directory.<br><br>Do you wish to REMOVE ALL FILES from prefix folder?").arg(prefixPath), QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

			QStringList args;
			args << "-rdf";
			args << prefixPath;

			Process exportProcess(args, "/bin/rm", QDir::homePath(), tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));

			if (exportProcess.exec()!=QDialog::Accepted){
				return;
			}
		} else {
			return;
		}
	}

	QDir fakeDir (prefixPath);
	if (!fakeDir.exists())
		fakeDir.mkdir(prefixPath);

	Wizard createFakeDriveWizard(2, cbPrefixes->currentText());
	if (createFakeDriveWizard.exec()==QDialog::Accepted){
		updateDtabaseConnectedItems();
	}

	return;
}


void MainWindow::cmdUpdateFake_Click(){
	QString prefixPath = db_prefix.getPath(cbPrefixes->currentText());

	QString sysregPath;
	sysregPath.append(prefixPath);
	sysregPath.append("/system.reg");

	QFile sysreg_file (sysregPath);

	if (!sysreg_file.exists()){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, no fake drive configuration found.<br>Create fake drive configuration before update it!"));
	} else {
		Wizard createFakeDriveWizard(3, cbPrefixes->currentText());
		if (createFakeDriveWizard.exec()==QDialog::Accepted){
			updateDtabaseConnectedItems();
		}
	}
	return;
}

void MainWindow::cmdClearFilter_Click(){
	txtIconFilter->setText("");
	return;
}

void MainWindow::mainExit_Click(){
	/*
	 * main Menu Exit
	 */

	QSettings settings(APP_SHORT_NAME, "default");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
    if (splitter->sizes().at(0) != splitter->sizes().at(1)){
        settings.setValue("splitterSize0", splitter->sizes().at(0));
        settings.setValue("splitterSize1", splitter->sizes().at(1));
    }
	settings.endGroup();

    serverSoket->close();

	qApp->quit();
	return;
}

void MainWindow::setMeVisible(bool visible){
	/*
	 * Hide and show MainWindow on TrayIcon click event
	 */
	setVisible(visible);
	return;
}

void MainWindow::mainPrograms_Click(){
	/*
	 * main Menu go Programs manage tool
	 */

	if (!isVisible())
		setMeVisible(TRUE);

	if (isMinimized ())
		showNormal ();


	tbwGeneral->setCurrentIndex (0);
	return;
}

void MainWindow::mainProcess_Click(){
	/*
	 * main Menu go Process manage tool
	 */

	if (!isVisible())
		setMeVisible(TRUE);

	if (isMinimized ())
		showNormal ();

	tbwGeneral->setCurrentIndex ( 1 );
	return;
}

void MainWindow::mainSetup_Click(){
	/*
	 * main Menu go Prefix Setup tool
	 */

	if (!isVisible())
		setMeVisible(TRUE);

	if (isMinimized ())
		showNormal ();

    tbwGeneral->setCurrentIndex ( 3 );
	return;
}

void MainWindow::mainLogging_Click(){
    /*
     * main Menu go Prefix Setup tool
     */

    if (!isVisible())
        setMeVisible(TRUE);

    if (isMinimized ())
        showNormal ();

    tbwGeneral->setCurrentIndex ( 2 );
    return;
}

void MainWindow::mainPrefix_Click(){
	/*
	 * main Menu go Prefix manage tool
	 */

	if (!isVisible())
		setMeVisible(TRUE);

	if (isMinimized ())
		showNormal ();

    tbwGeneral->setCurrentIndex ( 4 );
	return;
}

void MainWindow::mainAbout_Click(){
	/*
	 * main Menu shows About dialog
	 */

	About about;
	about.exec();

	return;
}

void MainWindow::mainRun_Click(){
	/*
	 * main Menu shows Run dialog
	 */
	if (cbPrefixes->currentText().isEmpty())
		return;

	Run run;
	run.prepare(cbPrefixes->currentText());

	if (run.exec()==QDialog::Accepted)
        CoreLib->runWineBinary(run.execObj, cbPrefixes->currentText());

	return;
}


void MainWindow::mainImageManager_Click(){
	/*
	 * CD Image Manager
	 */

	ImageManager manager(0);
	manager.exec();

	return;
}

void MainWindow::mainOptions_Click(){
	/*
	 * main Menu shows About dialog
	 */

	AppSettings options;

	if (options.exec()==QDialog::Accepted){
		CoreLib->checkSettings();

        if (CoreLib->getSetting("app", "showTrareyIcon", false).toBool()){
            QApplication::setQuitOnLastWindowClosed(false);
            trayIcon->show();
        } else {
            QApplication::setQuitOnLastWindowClosed(true);
            trayIcon->hide();
        }

        if (CoreLib->getSetting("logging", "enable", false, 1).toInt()==1){
            emit(reloadLogData());
            tabLogging->setEnabled(true);
        } else {
            tabLogging->setEnabled(false);
        }
	}

	return;
}

void MainWindow::mainAboutQt_Click(){
	/*
	 * main Menu shows AboutQt dialog
	 */

	QMessageBox::aboutQt ( this );

	return;
}

void MainWindow::mainInstall_Click(){
	/*
	 * main Menu shows install Wizard dialog
	 */

	QMessageBox::warning(this, tr("WIP"), tr("Sorry, no install wizard yet. It'l  implemented at v0.110."));

	return;
}

void MainWindow::mainFirstSteps_Click(){
	CoreLib->openHelpUrl("05-first-steps.html");
	return;
}

void MainWindow::mainFAQ_Click(){
	CoreLib->openHelpUrl("00-short-faq.html");
	return;
}

void MainWindow::mainIndex_Click(){
	CoreLib->openHelpUrl("index.html");
	return;
}

void MainWindow::mainWebsite_Click(){
	CoreLib->openHomeUrl("");
	return;
}

void MainWindow::mainDonate_Click(){
	CoreLib->openHomeUrl("donate/");
	return;
}

void MainWindow::mainBugs_Click(){
	CoreLib->openHomeUrl("bugs/");
	return;
}

void MainWindow::mainAppDB_Click(){
	if (!isVisible())
		setMeVisible(TRUE);

	if (isMinimized ())
		showNormal ();

    tbwGeneral->setCurrentIndex ( 5 );
	return;
}

void MainWindow::mainExportIcons_Click(){
	/*
	 * main Menu allow export icons
	 */

	QString fileName, tmpDir;
	QStringList args;

	fileName = QFileDialog::getOpenFileName(this, tr("Open image file"), QDir::homePath(), tr("Win32 Executable and Shared libraies (*.exe *.dll);;Win32 Executable (*.exe);;Win32 Shared libraies (*.dll)") );

	if(fileName.isEmpty())
		return;

	args << "-x";
	args << "-t" << "14";

	QStringList list1 = fileName.split("/");

	tmpDir.clear();

	tmpDir.append(QDir::homePath());
	tmpDir.append("/.config/");
	tmpDir.append(APP_SHORT_NAME);
	tmpDir.append("/tmp/");
	tmpDir.append(list1.last());

	QDir tmp(tmpDir);
	tmp.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList list = tmp.entryInfoList();

	if (tmp.exists(tmpDir)){
		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			if (!tmp.remove(fileInfo.filePath()))
				qDebug()<<"[EE] - Can't delete files at: "<<fileInfo.filePath();
		}

	} else {
		if (!tmp.mkdir(tmpDir)){
			qDebug()<<"[EE] - Can't create temp directory at: "<<tmpDir;
		}
	}


	args << "-o" << tmpDir;
	args << fileName;

	Process exportProcess(args, CoreLib->getSetting("icotool", "wrestool").toString(), QDir::homePath(), tr("Exporting icon from binary file.<br>This can take a while..."), tr("Exporting icon"), FALSE);

	if (exportProcess.exec()==QDialog::Accepted){
		//icotool -x -o ./regedit.png --width=32 --height=32 ./regedit.exe_14_100_0.ico



		args.clear();
		args << "-x";
		args << "-o" << QString("%1/").arg(tmpDir);

		// Updating file index
		list = tmp.entryInfoList();

		//Creating file list for converting
		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			if (fileInfo.fileName().right(3)=="ico")
				args << fileInfo.filePath();
		}


		//Look here, this function checks is some icons found, or not. 5 -- is default number of arguments,
		//if more -- then we have some ico file to convert
		if (args.size()>=4){

			Process exportProcess(args, CoreLib->getSetting("icotool", "icotool").toString(), QDir::homePath(), tr("Convering icon from binary file.<br>This can take a while..."), tr("Converting icon"), FALSE);

			if (exportProcess.exec()==QDialog::Accepted){
				IconsView iconsView(tmpDir);
				iconsView.exec();
			}

		} else {
			IconsView iconsView(tmpDir);
			iconsView.exec();
		}
	}

	//Clearing temp files
	list = tmp.entryInfoList();

	//Creating file list for converting
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (!QFile::remove(fileInfo.filePath()))
			qDebug()<<"[EE] - Can't delete files at: "<<fileInfo.filePath();
	}

	if (!tmp.rmdir(tmpDir))
		qDebug()<<"[EE] - Can't delete tmp dir: "<<tmpDir;

	return;
}

void MainWindow::messageReceived(const QString message){
    if (message.isEmpty()){
        if (!isVisible())
            this->setMeVisible(TRUE);

        if (isMinimized())
            this->showNormal ();

        statusBar()->showMessage(tr("Only one instance of %1 can be runned at same time.").arg(APP_SHORT_NAME));
    } else  {
        if (!QFile(message).exists()){
            if (!trayIcon->isVisible()){
                statusBar()->showMessage(tr("Binary \"%1\" do not exists.").arg(message));
            } else {
                trayIcon->showMessage(tr("Can't run binary"), tr("Binary \"%1\" do not exists.").arg(message));
            }
        } else {
            if (cbPrefixes->currentText().isEmpty())
                return;

            QString wrkDir;
            wrkDir = message.left(message.length() - QStringList(message.split("/")).last().length());

            Run run;
            run.prepare(cbPrefixes->currentText(), wrkDir, "", "", "", "", "", "", 0, message);

            if (run.exec()==QDialog::Accepted)
                CoreLib->runWineBinary(run.execObj, cbPrefixes->currentText());
        }
    }

	return;
}

void MainWindow::setcbPrefixesIndex(const QString text) const{
	cbPrefixes->setCurrentIndex(cbPrefixes->findText(text));
	return;
}
