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

MainWindow::MainWindow(int startState, QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f){

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

    if (startState == 1)
        this->showMinimized();

    setWindowTitle(tr("%1 :. Qt4 GUI for Wine v%2").arg(APP_NAME) .arg(APP_VERS));

    std::auto_ptr<IconListWidget> lstIcons (new IconListWidget(THEME_NAME, tabPrograms));
    connect(lstIcons.get(), SIGNAL(iconItemClick(QString, QString, QString, QString, QString)), this, SLOT(updateIconDesc(QString, QString, QString, QString, QString)));
    connect(lstIcons.get(), SIGNAL(changeStatusText(QString)), this, SLOT(changeStatusText(QString)));
    connect(txtIconFilter, SIGNAL(textChanged(QString)), lstIcons.get(), SLOT(setFilterString(QString)));

    std::auto_ptr<PrefixTreeWidget> twPrograms (new PrefixTreeWidget(THEME_NAME, tabPrograms));
    connect(this, SIGNAL(updateDatabaseConnections()), twPrograms.get(), SLOT(getPrefixes()));
    connect(twPrograms.get(), SIGNAL(showFolderContents(QString, QString)), lstIcons.get(), SLOT(showFolderContents(QString, QString)));
    connect(twPrograms.get(), SIGNAL(changeStatusText(QString)), this, SLOT(changeStatusText(QString)));
    connect(this, SIGNAL(setDefaultFocus(QString, QString)), twPrograms.get(), SLOT(setDefaultFocus(QString, QString)));
    connect(cbPrefixes, SIGNAL(currentIndexChanged(QString)), twPrograms.get(), SLOT(setDefaultFocus(QString)));
    connect(twPrograms.get(), SIGNAL(prefixIndexChanged(QString)), this, SLOT(setcbPrefixesIndex(QString)));

    std::auto_ptr<WineProccesWidget> procWidget (new WineProccesWidget(THEME_NAME, tabProcces));
    connect(this, SIGNAL(stopProcTimer()), procWidget.get(), SLOT(stopTimer()));
    connect(this, SIGNAL(startProcTimer()), procWidget.get(), SLOT(startTimer()));
    tabProccesLayout->addWidget(procWidget.release());

    std::auto_ptr<PrefixControlWidget> prefixWidget (new PrefixControlWidget(THEME_NAME, tabPrefix));
    connect(prefixWidget.get(), SIGNAL(updateDatabaseConnections()), twPrograms.get(), SLOT(getPrefixes()));
    connect(prefixWidget.get(), SIGNAL(updateDatabaseConnections()), this, SLOT(updateDtabaseConnectedItems()));

    std::auto_ptr<QWidget> wid (new QWidget(tabPrograms));
    std::auto_ptr<QVBoxLayout> vlayout (new QVBoxLayout);

    vlayout ->addWidget(widgetFilter);
    vlayout ->addWidget(lstIcons.release());
    vlayout ->setMargin(0);
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
    this->getSettings();

    connect(tbwGeneral, SIGNAL(currentChanged(int)), this, SLOT(CoreFunction_ResizeContent(int)));
    connect(cmdManagePrefixes, SIGNAL(clicked()), this, SLOT(cmdManagePrefixes_Click()));
    connect(cmdCreateFake, SIGNAL(clicked()), this, SLOT(cmdCreateFake_Click()));
    connect(cmdUpdateFake, SIGNAL(clicked()), this, SLOT(cmdUpdateFake_Click()));
    connect(cmdWinetricks, SIGNAL(clicked()), this, SLOT(cmdWinetricks_Click()));
    connect(cmdClearFilter, SIGNAL(clicked()), this, SLOT(cmdClearFilter_Click()));
    connect(cmdTestWis, SIGNAL(clicked()), this, SLOT(cmdTestWis_Click()));

    // Signals commection for Icons and Folders
    //connect(twPrograms.get(), SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(twPrograms_ItemClick(QTreeWidgetItem *, int)));
    //connect(twPrograms.get(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(twPrograms_ShowContextMenu(const QPoint &)));

    // Signals for updating toolbars
    //connect(tableProc, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableProc_ShowContextMenu(const QPoint &)));
    //connect(tableProc, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tablePrefix_UpdateContentList(const QModelIndex &)));

    // Init and connect SLOT & SIGNALS for context menus
    //connect(tablePrefix, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tablePrefix_ShowContextMenu(const QPoint &)));
    //connect(tablePrefix, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tablePrefix_UpdateContentList(const QModelIndex &)));

    //Main menu actions connection to slots
    connect(mainRun, SIGNAL(triggered()), this, SLOT(mainRun_Click()));
    connect(mainPrograms, SIGNAL(triggered()), this, SLOT(mainPrograms_Click()));
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




#ifndef WITH_ICOUTILS
    mainExportIcons->setEnabled(false);
#endif

#ifdef WITH_DEVELOP_STUFF
    cmdTestWis->setEnabled(true);
#else
    cmdTestWis->setEnabled(false);
#endif

    connect(mainOptions, SIGNAL(triggered()), this, SLOT(mainOptions_Click()));
    connect(mainInstall, SIGNAL(triggered()), this, SLOT(mainInstall_Click()));
    connect(mainExit, SIGNAL(triggered()), this, SLOT(mainExit_Click()));

    // Setting context menu policy
    //tableProc->setContextMenuPolicy(Qt::CustomContextMenu);
    //twPrograms->setContextMenuPolicy(Qt::CustomContextMenu);
   // tablePrefix->setContextMenuPolicy(Qt::CustomContextMenu);
    //lstIcons->setContextMenuPolicy(Qt::CustomContextMenu);

    // Creating actions for context menus & toolbars
    createMenuActions();

    // Enveropment path initialization
    HOME_PATH = QDir::homePath();
    ROOT_PATH = QDir::rootPath();
    TEMP_PATH = QDir::tempPath();

    // Setting default IconsSize for lstIcons (Wine-Programm-Menu) need for user settings

    //twPrograms->installEventFilter(this);

    txtIconFilter->installEventFilter(this);
    installEventFilter(this);
    cmdClearFilter->installEventFilter(this);

    // FIXME: Move this into shared libaray
    runAutostart();
    createTrayIcon();

    // Creating AppDBScrollWidget and place it into frameAppDBWidget layout
    appdbWidget.reset(new AppDBWidget());
    connect (this, SIGNAL(appdbWidget_startSearch(short int, QString)), appdbWidget.get(), SLOT(itemTrigged(short int, QString)));
    //connect (appdbWidget.get(), SIGNAL(xdgOpenUrl(QString)), this, SLOT(xdgOpenUrl(QString)));

    frameAppDBWidgetLayout->addWidget(appdbWidget.release());

    connect (cmdAppDBSearch, SIGNAL(clicked()), this, SLOT(cmdAppDBSearch_Click()));

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

void MainWindow::cmdTestWis_Click(){
    return;
}

void MainWindow::cmdAppDBSearch_Click(){
    if (!cbSearchText->currentText().isEmpty())
        emit(appdbWidget_startSearch(1, cbSearchText->currentText()));

    return;
}

void MainWindow::cmdWinetricks_Click() {
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

    val = CoreLib->getSetting("wine", "WineBin");
    DEFAULT_WINE_BIN=val.toString();
    val = CoreLib->getSetting("wine", "ServerBin");
    DEFAULT_WINE_SERVER=val.toString();
    val = CoreLib->getSetting("wine", "LoaderBin");
    DEFAULT_WINE_LOADER=val.toString();
    val = CoreLib->getSetting("wine", "WineLibs");
    DEFAULT_WINE_LIBS=val.toString();

    val = CoreLib->getSetting("app", "showTrareyIcon", false);
    SHOW_TRAREY_ICON=val.toBool();

    val = CoreLib->getSetting("system", "tar");
    TAR_BIN=val.toString();
    val = CoreLib->getSetting("system", "mount");
    MOUNT_BIN=val.toString();
    val = CoreLib->getSetting("system", "umount");
    UMOUNT_BIN=val.toString();
    val = CoreLib->getSetting("system", "sudo");
    SUDO_BIN=val.toString();
    val = CoreLib->getSetting("system", "gui_sudo");
    GUI_SUDO_BIN=val.toString();
    val = CoreLib->getSetting("system", "nice");
    NICE_BIN=val.toString();
    val = CoreLib->getSetting("system", "renice");
    RENICE_BIN=val.toString();
    val = CoreLib->getSetting("system", "sh");
    SH_BIN=val.toString();

    val = CoreLib->getSetting("console", "bin");
    CONSOLE_BIN=val.toString();
    val = CoreLib->getSetting("console", "args", false);
    CONSOLE_ARGS=val.toString();

#ifdef WITH_ICOUTILS
    val = CoreLib->getSetting("icotool", "wrestool");
    WRESTOOL_BIN=val.toString();
    val = CoreLib->getSetting("icotool", "icotool");
    ICOTOOL_BIN=val.toString();
#endif



    if (CoreLib->getSetting("quickmount", "type", FALSE).toString().isEmpty()){
        QSettings settings(APP_SHORT_NAME, "default");
        settings.beginGroup("quickmount");

        if (CoreLib->getWhichOut("fuseiso", false).isEmpty()){
#ifdef WITH_EMBEDDED_FUSEISO
            settings.setValue("type", 3);
            settings.setValue("mount_drive_string", CoreLib->getMountString(3));
            settings.setValue("mount_image_string", CoreLib->getMountImageString(3));
            settings.setValue("umount_string", CoreLib->getUmountString(3));
#else
            settings.setValue("type", 0);
            settings.setValue("mount_drive_string", CoreLib->getMountString(0));
            settings.setValue("mount_image_string", CoreLib->getMountImageString(0));
            settings.setValue("umount_string", CoreLib->getUmountString(0));
#endif
        } else {
            settings.setValue("type", 2);
            settings.setValue("mount_drive_string", CoreLib->getMountString(2));
            settings.setValue("mount_image_string", CoreLib->getMountImageString(2));
            settings.setValue("umount_string", CoreLib->getUmountString(2));
        }
        settings.endGroup();
    }

    emit(setDefaultFocus(CoreLib->getSetting("LastPrefix", "prefix", false).toString(), CoreLib->getSetting("LastPrefix", "dir", false).toString()));

    return;
}

void MainWindow::changeStatusText(QString text){
    statusBar()->showMessage(text);
}

void MainWindow::updateDtabaseConnectedItems(){
    QString curPrefix =  cbPrefixes->currentText();

    cbPrefixes->clear();
    QList<QStringList> result = db_prefix.getFields();
    for (int i = 0; i < result.size(); ++i) {
        cbPrefixes->addItem (result.at(i).at(1));
    }

    emit(updateDatabaseConnections());

    if (!curPrefix.isEmpty()){
        cbPrefixes->setCurrentIndex(cbPrefixes->findText(curPrefix));
    }
    return;
}

void MainWindow::cmdManagePrefixes_Click(){
    tbwGeneral->setCurrentIndex (3);
    return;
}

void MainWindow::createTrayIcon(){
    /*
    trayIconMenu.reset(new QMenu(this));
    trayIconMenu->addAction(mainRun);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(mainExportIcons);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(mainImageManage);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(mainPrograms);
    trayIconMenu->addAction(mainProcess);
    trayIconMenu->addAction(mainSetup);
    trayIconMenu->addAction(mainPrefix);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(mainExit);

    trayIcon.reset(new QSystemTrayIcon(this));
    trayIcon->setContextMenu(trayIconMenu.get());

    QIcon icon = loadIcon("data/q4wine.png");

    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    if (SHOW_TRAREY_ICON){
        trayIcon->show();
    } else {
        trayIcon->hide();
    }

    connect(trayIcon.get(), SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIcon_Activate(QSystemTrayIcon::ActivationReason)));
    */
}

void MainWindow::closeEvent(QCloseEvent *event){
    /*if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }*/

    QSettings settings(APP_SHORT_NAME, "default");
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("splitterSize0", splitter->sizes().at(0));
    settings.setValue("splitterSize1", splitter->sizes().at(1));
    settings.endGroup();

    return;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    // twPrograms events

    /*
    if (obj == this){
        if (event->type()==QEvent::Resize){
            if (!this->isActiveWindow())
                return false;
        }
    }

    if (obj == twPrograms) {
        std::auto_ptr<QKeyEvent> keyEvent (static_cast<QKeyEvent*>(event));
        if (event->type() == QEvent::KeyRelease) {
            if ((keyEvent->key()==Qt::Key_Up) or (keyEvent->key()==Qt::Key_Down)){
                if (twPrograms->currentItem())
                    twPrograms_ItemClick(twPrograms->currentItem(), 0);
                keyEvent.release();
                return true;
            }
        }

        if (event->type() == QEvent::KeyPress){
            if ((keyEvent->key()==Qt::Key_Tab)){
                lstIcons->setFocus();
                if (lstIcons->currentItem())
                    lstIcons_ItemClick(lstIcons->currentItem());
                keyEvent.release();
                return true;
            }

            if (keyEvent->key()==Qt::Key_Delete){
                if (twPrograms->currentItem())
                    dirDelete_Click();
                keyEvent.release();
                return true;
            }

            if (keyEvent->key()==Qt::Key_F2){
                if (twPrograms->currentItem())
                    dirRename_Click();
                keyEvent.release();
                return true;
            }
        }
        keyEvent.release();
        return false;

    }

    // lstIcons events

    if (obj == lstIcons.get()){
        std::auto_ptr<QKeyEvent> keyEvent (static_cast<QKeyEvent*>(event));

        if (event->type() == QEvent::KeyRelease) {
            if ((keyEvent->key()==Qt::Key_Up) or (keyEvent->key()==Qt::Key_Down) or (keyEvent->key()==Qt::Key_Left) or (keyEvent->key()==Qt::Key_Right)){
                if (lstIcons->currentItem())
                    lstIcons_ItemClick(lstIcons->currentItem());
                keyEvent.release();
                return true;
            }
        }

        if (event->type() == QEvent::KeyPress){
            if ((keyEvent->key()==Qt::Key_Tab)){
                txtIconFilter->setFocus();
                keyEvent.release();
                return true;
            }

            if (keyEvent->key()==Qt::Key_Return){
                if (lstIcons->currentItem())
                    lstIcons_ItemDoubleClick(lstIcons->currentItem());
                keyEvent.release();
                return true;
            }

            if (keyEvent->key()==Qt::Key_Delete){
                if (lstIcons->currentItem())
                    iconDelete_Click();
                keyEvent.release();
                return true;
            }

            if (keyEvent->key()==Qt::Key_F2){
                if (lstIcons->currentItem())
                    iconRename_Click();
                keyEvent.release();
                return true;
            }
        }
        keyEvent.release();
        return false;
    }

    if (obj == txtIconFilter){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (event->type() == QEvent::KeyPress){
            if ((keyEvent->key()==Qt::Key_Tab)){
                cmdClearFilter->setFocus();
                return true;
            }
        }
        return false;
    }

    if (obj == cmdClearFilter){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (event->type() == QEvent::KeyPress){
            if ((keyEvent->key()==Qt::Key_Tab)){
                twPrograms->setFocus();
                return true;
            }
        }
        return false;
    }
    */
    return QMainWindow::eventFilter(obj, event);

}


void MainWindow::resizeEvent (QResizeEvent){
    /*
        Function for hendle resize event: tableProc (см код)
    */

    CoreFunction_ResizeContent(1);
    CoreFunction_ResizeContent(3);

    return;
}

void MainWindow::CoreFunction_ResizeContent(int tabIndex){
    switch (tabIndex){
 case 1:
        //Initiate /proc reading
        emit(startProcTimer());
        break;
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

            Process *exportProcess = new Process(args, "/bin/rm", HOME_PATH, tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));

            if (exportProcess->exec()!=QDialog::Accepted){
                return;
            }
        } else {
            return;
        }
    }

    QDir fakeDir (prefixPath);
    if (!fakeDir.exists())
        fakeDir.mkdir(prefixPath);

    Wizard *createFakeDriveWizard = new Wizard(2, cbPrefixes->currentText());
    if (createFakeDriveWizard->exec()==QDialog::Accepted){
        updateDtabaseConnectedItems();
    }
    delete(createFakeDriveWizard);
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
        Wizard *createFakeDriveWizard = new Wizard(3, cbPrefixes->currentText());
        if (createFakeDriveWizard->exec()==QDialog::Accepted){
            updateDtabaseConnectedItems();
        }
        delete(createFakeDriveWizard);
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
    settings.endGroup();

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

    tbwGeneral->setCurrentIndex ( 3 );
    return;
}

void MainWindow::mainAbout_Click(){
    /*
     * main Menu shows About dialog
     */

    if (!isVisible())
        setMeVisible(TRUE);

    if (isMinimized ())
        showNormal ();

    About *about = new About();
    about->exec();
    delete(about);
    return;
}

void MainWindow::mainRun_Click(){
    /*
     * main Menu shows Run dialog
     */

//	if (!twPrograms->currentItem())
        return;

//	QTreeWidgetItem *treeItem = twPrograms->currentItem();
        /*
    QStringList dataList;

    if (!isVisible())
        setMeVisible(TRUE);

    if (isMinimized ())
        showNormal ();

    Run run;

    if (treeItem->parent()){
        run.prepare(treeItem->parent()->text(0));
    } else {
        run.prepare(treeItem->text(0));
    }

    if (run.exec()==QDialog::Accepted)
        CoreLib->runWineBinary(run.execObj);
*/
    return;
}


void MainWindow::mainImageManager_Click(){
    /*
     * CD Image Manager
     */

    ImageManager *manager = new ImageManager(0);
    manager->exec();

    delete(manager);
    return;
}

void MainWindow::mainOptions_Click(){
    /*
     * main Menu shows About dialog
     */

    AppSettings *options = new AppSettings();

    if (options->exec()==QDialog::Accepted){
        getSettings();

        if (SHOW_TRAREY_ICON){
            trayIcon->show();
        } else {
            trayIcon->hide();
        }

    }

    delete(options);
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
}

void MainWindow::mainFAQ_Click(){
    CoreLib->openHelpUrl("00-short-faq.html");
}

void MainWindow::mainIndex_Click(){
    CoreLib->openHelpUrl("index.html");
}

void MainWindow::mainWebsite_Click(){
    CoreLib->openHomeUrl("");
}

void MainWindow::mainDonate_Click(){
    CoreLib->openHomeUrl("donate/");
}

void MainWindow::mainBugs_Click(){
    CoreLib->openHomeUrl("bugs/");
}


void MainWindow::mainExportIcons_Click(){
    /*
     * main Menu allow export icons
     */

    if (!isVisible())
        setMeVisible(TRUE);

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

    Process *exportProcess = new Process(args, WRESTOOL_BIN, HOME_PATH, tr("Exporting icon from binary file.<br>This can take a while..."), tr("Exporting icon"), FALSE);

    if (exportProcess->exec()==QDialog::Accepted){
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

            exportProcess = new Process(args, ICOTOOL_BIN, HOME_PATH, tr("Convering icon from binary file.<br>This can take a while..."), tr("Converting icon"), FALSE);

            if (exportProcess->exec()==QDialog::Accepted){
                IconsView *iconsView = new IconsView(tmpDir);
                iconsView->exec();
            }

        } else {
            IconsView *iconsView = new IconsView(tmpDir);
            iconsView->exec();
        }
    }
    delete(exportProcess);

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

void MainWindow::createMenuActions(){

    /*


    menuProc.reset(new QMenu(this));
    menuProc->addAction(processKillSelected.get());
    menuProc->addSeparator();
    menuProc->addAction(processKillWine.get());
    menuProc->addSeparator();
    menuProc->addAction(processRenice.get());
    menuProc->addSeparator();
    menuProc->addAction(processRefresh.get());

    menuPrefix.reset(new QMenu(this));
    menuPrefix->addAction(prefixAdd.get());
    menuPrefix->addSeparator();
    menuPrefix->addAction(prefixImport.get());
    menuPrefix->addAction(prefixExport.get());
    menuPrefix->addSeparator();
    menuPrefix->addAction(prefixDelete.get());
    menuPrefix->addSeparator();
    menuPrefix->addAction(prefixSettings.get());

*/

    return;
}

QIcon MainWindow::loadIcon(QString iconName){
    // Function tryes to load icon image from theme dir
    // If it fails -> load default from rsource file

    QIcon icon;

    if ((!THEME_NAME.isEmpty()) and (THEME_NAME!="Default")){
        icon.addFile(QString("%1/%2").arg(THEME_NAME).arg(iconName));
        if (icon.isNull()){
            icon.addFile(QString(":/%1").arg(iconName));
        }
    } else {
        icon.addFile(QString(":/%1").arg(iconName));
    }

    return icon;
}

void MainWindow::runAutostart(void){
    QList<QStringList> iconsList, prefixList;

    prefixList = db_prefix.getFields();
    for (int i = 0; i < prefixList.size(); ++i) {
        iconsList = db_icon.getByPrefixAndDirName(prefixList.at(i).at(1), "autostart");
        for (int j = 0; j < iconsList.size(); ++j) {
            CoreLib->runIcon(prefixList.at(i).at(1), "autostart", iconsList.at(j).at(1));
        }
    }
    return;
}

void MainWindow::messageReceived(const QString message) const{
    statusBar()->showMessage(message);
    return;
}

void MainWindow::setcbPrefixesIndex(const QString text) const{
    cbPrefixes->setCurrentIndex(cbPrefixes->findText(text));
    return;
}
