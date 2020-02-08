/***************************************************************************
 *   Copyright (C) 2008-2020 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "appsettings.h"

AppSettings::AppSettings(QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
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

    // Getting corelib class pointer
    CoreLibClassPointer = reinterpret_cast<CoreLibPrototype*>(libq4wine.resolve("createCoreLib"));
    CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

    setupUi(this);

    splitter.reset(new QSplitter(widgetContent));
    splitter->addWidget(optionsTreeWidget);
    splitter->addWidget(optionsStack);

    QList<int> size;
    size << 150 << 150;

    splitter->setSizes(size);

    std::unique_ptr<QVBoxLayout> vlayout (new QVBoxLayout);
    vlayout->addWidget(splitter.release());
    vlayout->setMargin(0);
    vlayout->setSpacing(0);
    widgetContent->setLayout(vlayout.release());

    setWindowTitle(tr("%1 settings").arg(APP_NAME));
    setWindowIcon(CoreLib->loadIcon(CoreLib->getSetting("app", "icon", false, "q4wine").toString()));

    connect(optionsTree, SIGNAL(itemClicked (QTreeWidgetItem *, int)), this, SLOT(optionsTree_itemClicked ( QTreeWidgetItem *, int)));

    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
    connect(cmdVersionManager, SIGNAL(clicked()), this, SLOT(cmdVersionManager_Click()));

    connect(comboProxyType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboProxyType_indexChanged(QString)));

    //Installing event filters for get buttuns
    cmdGetTarBin->installEventFilter(this);
    cmdGetMountBin->installEventFilter(this);
    cmdGetUmountBin->installEventFilter(this);
    cmdGetSudoBin->installEventFilter(this);
    cmdGetGuiSudoBin->installEventFilter(this);
    cmdGetNiceBin->installEventFilter(this);
    cmdGetReniceBin->installEventFilter(this);
    cmdGetConsoleBin->installEventFilter(this);
    cmdGetShBin->installEventFilter(this);
    cmdGetDefPrefixPath->installEventFilter(this);
    cmdGetWineDesktop->installEventFilter(this);
    cmdGetWineDesktopDoc->installEventFilter(this);
    cmdGetWineDesktopMus->installEventFilter(this);
    cmdGetWineDesktopPic->installEventFilter(this);
    cmdGetWineDesktopVid->installEventFilter(this);

    QSettings settings(APP_SHORT_NAME, "default");

    settings.beginGroup("logging");
    connect (cbShowTray, SIGNAL(stateChanged (int)), this, SLOT(cbShowTray_stateChanged (int)));

    if (settings.value("autoClear", 1).toInt()==1){
        cbClearLogs->setCheckState(Qt::Checked);
    } else {
        cbClearLogs->setCheckState(Qt::Unchecked);
    }
    settings.endGroup();

    settings.beginGroup("app");

    QString appIcon = settings.value("icon").toString();
    if (appIcon == "q4wine-ambiance"){
        comboAppIcons->setCurrentIndex(comboAppIcons->findText(tr("Ambient Light")));
    } else if (appIcon == "q4wine-ambiance-dark"){
        comboAppIcons->setCurrentIndex(comboAppIcons->findText(tr("Ambient Dark")));
    } else {
        comboAppIcons->setCurrentIndex(comboAppIcons->findText(tr("Default")));
    }

    if (settings.value("showTrareyIcon", 0).toInt()==1){
        cbShowTray->setCheckState(Qt::Checked);
    } else {
        cbShowTray->setCheckState(Qt::Unchecked);
    }

    if (settings.value("showNotifications", 1).toInt()==1){
        cbShowNotifications->setCheckState(Qt::Checked);
    } else {
        cbShowNotifications->setCheckState(Qt::Unchecked);
    }

    if (settings.value("minimizeToTray", 0).toInt()==1){
        cbMinimizeToTray->setCheckState(Qt::Checked);
    } else {
        cbMinimizeToTray->setCheckState(Qt::Unchecked);
    }

    if (settings.value("minimizeToTrayAtAppStart", 0).toInt()==1){
        cbMinimizeToTrayApp->setCheckState(Qt::Checked);
    } else {
        cbMinimizeToTrayApp->setCheckState(Qt::Unchecked);
    }

    loadThemeIcons();

    getLangs();

    if (settings.value("lang").toString().isEmpty()){
        comboLangs->setCurrentIndex(comboLangs->findText(tr("System Default")));
    } else {
        QString lName = settings.value("lang").toString();
        //Strip q4wine_ from setting if any
        lName.replace("q4wine_", "");
        comboLangs->setCurrentIndex(comboLangs->findText(QLocale(lName).nativeLanguageName()));
    }

    settings.endGroup();

    settings.beginGroup("system");
    txtTarBin->setText(settings.value("tar").toString());
    txtMountBin->setText(settings.value("mount").toString());
    txtUmountBin->setText(settings.value("umount").toString());
    txtSudoBin->setText(settings.value("sudo").toString());
    txtGuiSudoBin->setText(settings.value("gui_sudo").toString());
    txtNiceBin->setText(settings.value("nice").toString());
    txtReniceBin->setText(settings.value("renice").toString());
    txtShBin->setText(settings.value("sh").toString());
    settings.endGroup();

    settings.beginGroup("console");
    txtConsoleBin->setText(settings.value("bin").toString());
    txtConsoleArgs->setText(settings.value("args").toString());
    settings.endGroup();

    settings.beginGroup("quickmount");

    comboMountProfiles->setCurrentIndex(settings.value("type", 0).toInt());
    connect(comboMountProfiles, SIGNAL(currentIndexChanged(int)), this, SLOT(comboMountProfiles_currentIndexChanged(int)));

    if (!settings.value("mount_drive_string").toString().isEmpty())
        txtMountString->setText(settings.value("mount_drive_string").toString());
    if (!settings.value("mount_image_string").toString().isEmpty())
        txtMountImageString->setText(settings.value("mount_image_string").toString());
    if (!settings.value("umount_string").toString().isEmpty())
        txtUmountString->setText(settings.value("umount_string").toString());

    settings.endGroup();

#ifdef WITH_ICOUTILS
    settings.beginGroup("icotool");
    txtWrestoolBin->setText(settings.value("wrestool").toString());
    txtIcotoolBin->setText(settings.value("icotool").toString());
    settings.endGroup();
    cmdGetWrestoolBin->installEventFilter(this);
    cmdGetIcotoolBin->installEventFilter(this);
#else
    txtWrestoolBin->setEnabled(false);
    txtIcotoolBin->setEnabled(false);
    cmdGetWrestoolBin->setEnabled(false);
    cmdGetIcotoolBin->setEnabled(false);
#endif

    settings.beginGroup("network");
    txtProxyHost->setText(settings.value("host").toString());
    txtProxyPort->setText(settings.value("port").toString());
    txtProxyUser->setText(settings.value("user").toString());
    txtProxyPass->setText(settings.value("pass").toString());

    comboProxyType->setCurrentIndex(settings.value("type").toInt());

    settings.endGroup();

#ifdef _OS_FREEBSD_
    this->comboMountProfiles->removeItem(3);
    this->comboMountProfiles->removeItem(2);
#endif
#ifdef _OS_DARWIN_
    this->comboMountProfiles->removeItem(3);
    this->comboMountProfiles->removeItem(2);
    this->comboMountProfiles->removeItem(1);
    this->comboMountProfiles->setItemText(0, tr("generic"));
    this->txtMountString->setEnabled(false);
    this->frameMount->setVisible(false);
#endif

    settings.beginGroup("advanced");
    txtDefPrefixPath->setText(settings.value("prefixDefaultPath").toString());

    if (settings.value("openRunDialog", 0).toInt()==0){
        cbOpenRunDialog->setChecked(false);
    } else {
        cbOpenRunDialog->setChecked(true);
    }

    if (settings.value("useSingleClick", 0).toInt()==0){
        cbUseSingleClick->setChecked(false);
    } else {
        cbUseSingleClick->setChecked(true);
    }

    if (settings.value("hideArguments", 0).toInt()==0){
        cbHideArguments->setChecked(false);
    } else {
        cbHideArguments->setChecked(true);
    }

    if (settings.value("useNativeFileDialog", 1).toInt()==1){
        cbUseNativeDialog->setChecked(true);
    } else {
        cbUseNativeDialog->setChecked(false);
    }

    QString res = settings.value("defaultDesktopSize").toString();
    if (res.isEmpty()){
        cboxDesktopSize->setCurrentIndex(0);
    } else {
        int i = cboxDesktopSize->findText(res);
        if (i >= 0) {
            cboxDesktopSize->setCurrentIndex(i);
        } else {
            cboxDesktopSize->setCurrentText(res);
        }
    }

    settings.endGroup();


    settings.beginGroup("DesktopImport");
    if (settings.value("remove", 0).toInt()==0){
        cbRemoveDesktopFiles->setChecked(false);
    } else {
        cbRemoveDesktopFiles->setChecked(true);
    }

    if (settings.value("importAtStartup", 0).toInt()==0){
        cbImportDesktopFilesAtStartup->setChecked(false);
    } else {
        cbImportDesktopFilesAtStartup->setChecked(true);
    }
    settings.endGroup();

    settings.beginGroup("AppDB");
    if (settings.value("useSystemBrowser", 1).toInt()==1){
        cbUseSystemBrowser->setChecked(true);
    } else {
        cbUseSystemBrowser->setChecked(false);
    }
    settings.endGroup();

#ifndef _OS_DARWIN_
    settings.beginGroup("Plugins");
    if (settings.value("enableMenuDesktop", 1).toInt()==1){
        cbEnableDesktopMenu->setChecked(true);
    } else {
        cbEnableDesktopMenu->setChecked(false);
    }
    if (settings.value("ExportSystemFolder", 0).toInt()==1){
        cbExportSystemFolder->setChecked(true);
    } else {
        cbExportSystemFolder->setChecked(false);
    }

    settings.endGroup();
    connect(cmdDesktopMenu_Regen, SIGNAL(clicked()), this, SLOT(cmdDesktopMenu_Regen_Click()));
    connect(cmdDesktopMenu_Remove, SIGNAL(clicked()), this, SLOT(cmdDesktopMenu_Remove_Click()));
#else
    cbEnableDesktopMenu->setEnabled(false);
    cmdDesktopMenu_Regen->setEnabled(false);
    cmdDesktopMenu_Remove->setEnabled(false);
#endif

    settings.beginGroup("DesktopIntegration");
    this->txtWineDesktop->setText(settings.value("WineDesktop", "").toString());
    this->txtWineDesktopDoc->setText(settings.value("WineDesktopDoc", "").toString());
    this->txtWineDesktopMus->setText(settings.value("WineDesktopMus", "").toString());
    this->txtWineDesktopPic->setText(settings.value("WineDesktopPic", "").toString());
    this->txtWineDesktopVid->setText(settings.value("WineDesktopVid", "").toString());
    settings.endGroup();

    QList<QTreeWidgetItem *> items = optionsTree->findItems (tr("General"), Qt::MatchExactly);
    if (items.count()>0){
        items.at(0)->setExpanded(true);
        optionsTree->setCurrentItem(items.at(0));
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(0);
    }

    cmdOk->setFocus(Qt::ActiveWindowFocusReason);
    return;
}

void AppSettings::optionsTree_itemClicked ( QTreeWidgetItem *item, int){
    if (!item)
        return;

    item->setExpanded(true);

    QString itemText = item->text(0);

    if (itemText==tr("General")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(0);
    } else if (itemText==tr("System")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(1);
    } else if (itemText==tr("Utils")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(2);
    } else if (itemText==tr("Network")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(3);
    } else if (itemText==tr("Quick Mount")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(4);
    } else if (itemText==tr("Interface")){
        optionsStack->setCurrentIndex(1);
        tabwInterface->setCurrentIndex(0);
    } else if (itemText==tr("Language")){
        optionsStack->setCurrentIndex(1);
        tabwInterface->setCurrentIndex(0);
    } else if (itemText==tr("Themes")){
        optionsStack->setCurrentIndex(1);
        tabwInterface->setCurrentIndex(1);
    } else if (itemText==tr("Subsystems")){
        optionsStack->setCurrentIndex(2);
        tabwInterface->setCurrentIndex(0);
    } else if (itemText==tr("Logging")){
        optionsStack->setCurrentIndex(2);
        tabwSubsystems->setCurrentIndex(0);
    } else if (itemText==tr("AppDB browser")){
        optionsStack->setCurrentIndex(2);
        tabwSubsystems->setCurrentIndex(1);
    } else if (itemText==tr("Plugins")){
        optionsStack->setCurrentIndex(3);
        tabwPlugins->setCurrentIndex(1);
    } else if (itemText==tr("Desktop Menu")){
        optionsStack->setCurrentIndex(3);
        tabwPlugins->setCurrentIndex(1);
    } else if (itemText==tr("Advanced")){
        optionsStack->setCurrentIndex(4);
        pabwAdvanced->setCurrentIndex(0);
    } else if (itemText==tr("Defaults")){
       optionsStack->setCurrentIndex(4);
        pabwAdvanced->setCurrentIndex(0);
    } else if (itemText==tr("Desktop Integration")){
        optionsStack->setCurrentIndex(4);
        pabwAdvanced->setCurrentIndex(1);
    } else if (itemText==tr("Run dialog")){
        optionsStack->setCurrentIndex(4);
        pabwAdvanced->setCurrentIndex(2);
    } else if (itemText==tr("Wine desktop import")){
        optionsStack->setCurrentIndex(4);
        pabwAdvanced->setCurrentIndex(3);
    }
}

bool AppSettings::eventFilter(QObject *obj, QEvent *event){
    /*
            function for displaying file\dir dialog
      */

    if (event->type() == QEvent::MouseButtonRelease) {
        QString file;
        QString obj_name;
        QString search_path;

        obj_name.append("txt");
        obj_name.append(obj->objectName().right(obj->objectName().length()-6));

        std::unique_ptr<QLineEdit> lineEdit (findChild<QLineEdit *>(obj_name));
        search_path = lineEdit.get()->text();

        QFileDialog::Options options;

        if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
                options = QFileDialog::DontUseNativeDialog | QFileDialog::DontResolveSymlinks;

        if (obj->objectName().right(3)=="Bin"){
            file = QFileDialog::getOpenFileName(this, tr("Open File"), search_path, "All files (*)", 0, options);
        } else {
            file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), search_path,  options);
        }

        if (!file.isEmpty()){
            if (lineEdit.get()){
                lineEdit->setText(file);
            } else {
                qDebug("Error getting lineEdit object");
            }


        }

        lineEdit.release();
    }

    return false;
}

void AppSettings::comboProxyType_indexChanged(QString text){
    if (text==tr("No Proxy")){
        txtProxyHost->setEnabled(false);
        txtProxyPort->setEnabled(false);
        txtProxyUser->setEnabled(false);
        txtProxyPass->setEnabled(false);
    } else {
        txtProxyHost->setEnabled(true);
        txtProxyPort->setEnabled(true);
        txtProxyUser->setEnabled(true);
        txtProxyPass->setEnabled(true);
    }

    return;
}

void AppSettings::getLangs(){
    QString themeDir="";
#ifdef RELEASE
    #ifdef _OS_DARWIN_
        themeDir = QDir::currentPath();
        themeDir.append("/");
        themeDir.append(APP_SHORT_NAME);
        themeDir.append(".app/Contents");
    #else
        themeDir = APP_PREF;
        themeDir.append("/share/");
        themeDir.append(APP_SHORT_NAME);
    #endif
#else
    themeDir = QString::fromUtf8(APP_BUILD);
#endif
    themeDir.append("/l10n");

    QDir tmp(themeDir);
    tmp.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList list = tmp.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName().right(2)=="qm"){
            QString lName = fileInfo.fileName().replace("q4wine_", "");
            QLocale lObj = QLocale(lName);
            QString lNameNative = lObj.nativeLanguageName();
            lng_hash[lNameNative] = lObj.name();
            comboLangs->addItem(lNameNative);
        }
    }
    return;
}

void AppSettings::loadThemeIcons(){
    cmdGetTarBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetMountBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetUmountBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetSudoBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetGuiSudoBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetNiceBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetReniceBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetShBin->setIcon(CoreLib->loadIcon("document-open"));

    cmdGetConsoleBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWrestoolBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetIcotoolBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetDefPrefixPath->setIcon(CoreLib->loadIcon("document-open"));

    cmdGetWineDesktop->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineDesktopDoc->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineDesktopMus->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineDesktopPic->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineDesktopVid->setIcon(CoreLib->loadIcon("document-open"));
    return;
}

void AppSettings::cmdCancel_Click(){
    reject();
    return;
}

void AppSettings::cmdOk_Click(){

    if (!checkEntry(txtTarBin->text(), "tar"))
        return;

    if (!checkEntry(txtMountBin->text(), "mount"))
        return;

    if (!checkEntry(txtUmountBin->text(), "umount"))
        return;

    if (!checkEntry(txtSudoBin->text(), "sudo"))
        return;

    if (!checkEntry(txtGuiSudoBin->text(), "gui_sudo"))
        return;

    if (!checkEntry(txtUmountBin->text(), "nice"))
        return;

    if (!checkEntry(txtUmountBin->text(), "renice"))
        return;

    if (!checkEntry(txtUmountBin->text(), "sh"))
        return;

    if (!checkEntry(txtConsoleBin->text(), "console"))
        return;

#ifdef WITH_ICOUTILS
    if (!checkEntry(txtWrestoolBin->text(), "wrestool"))
        return;

    if (!checkEntry(txtIcotoolBin->text(), "icotool"))
        return;
#endif

    if (comboProxyType->currentText()!=tr("No Proxy")){
        if (txtProxyHost->text().isEmpty()){
            QMessageBox::warning(this, tr("Error"), tr("Sorry, specify proxy host."));
            return;
        }
        if (txtProxyPort->text().isEmpty()){
            QMessageBox::warning(this, tr("Error"), tr("Sorry, specify proxy port."));
            return;
        }
    }

    QSettings settings(APP_SHORT_NAME, "default");

    settings.beginGroup("logging");
    if (cbClearLogs->checkState()==Qt::Checked) {
        settings.setValue("autoClear", 1);
    } else {
        settings.setValue("autoClear", 0);
    }
    settings.endGroup();

    settings.beginGroup("app");

    if (comboAppIcons->currentText() == tr("Ambient Light")){
        settings.setValue("icon", "q4wine-ambiance");
    } else if (comboAppIcons->currentText() == tr("Ambient Dark")){
        settings.setValue("icon", "q4wine-ambiance-dark");
    } else {
        settings.setValue("icon", "q4wine");
    }

    if (cbShowTray->checkState()==Qt::Checked) {
        settings.setValue("showTrareyIcon", 1);
    } else {
        settings.setValue("showTrareyIcon", 0);
    }

    if (cbMinimizeToTray->checkState()==Qt::Checked) {
        settings.setValue("minimizeToTray", 1);
    } else {
        settings.setValue("minimizeToTray", 0);
    }

    if (cbMinimizeToTrayApp->checkState()==Qt::Checked) {
        settings.setValue("minimizeToTrayAtAppStart", 1);
    } else {
        settings.setValue("minimizeToTrayAtAppStart", 0);
    }

    if (cbShowNotifications->checkState()==Qt::Checked) {
        settings.setValue("showNotifications", 1);
    } else {
        settings.setValue("showNotifications", 0);
    }

    if (comboLangs->currentText()==tr("System Default")){
        settings.setValue("lang", "");
    } else {
        settings.setValue("lang", lng_hash[comboLangs->currentText()]);
    }

    settings.endGroup();
    settings.beginGroup("system");
    settings.setValue("tar", txtTarBin->text());
    settings.setValue("mount", txtMountBin->text());
    settings.setValue("umount", txtUmountBin->text());
    settings.setValue("sudo", txtSudoBin->text());
    settings.setValue("gui_sudo", txtGuiSudoBin->text());
    settings.setValue("nice", txtNiceBin->text());
    settings.setValue("renice", txtReniceBin->text());
    settings.setValue("sh", txtShBin->text());
    settings.endGroup();

    settings.beginGroup("console");
    settings.setValue("bin", txtConsoleBin->text());
    settings.setValue("args", txtConsoleArgs->text());
    settings.endGroup();
#ifdef WITH_ICOUTILS
    settings.beginGroup("icotool");
    settings.setValue("wrestool", txtWrestoolBin->text());
    settings.setValue("icotool", txtIcotoolBin->text());
    settings.endGroup();
#endif

    settings.beginGroup("quickmount");

    settings.setValue("type", this->comboMountProfiles->currentIndex());
    settings.setValue("mount_drive_string", txtMountString->text());
    settings.setValue("mount_image_string", txtMountImageString->text());
    settings.setValue("umount_string", txtUmountString->text());

    settings.endGroup();

    settings.beginGroup("network");
    settings.setValue("host", txtProxyHost->text());
    settings.setValue("port", txtProxyPort->text());
    settings.setValue("user", txtProxyUser->text());
    settings.setValue("pass", txtProxyPass->text());
    if (comboProxyType->currentText()==tr("No Proxy")){
        settings.setValue("type", 0);
    } else {
        if (comboProxyType->currentText()=="HTTP"){
            settings.setValue("type", 1);
        } else {
            settings.setValue("type", 2);
        }
    }

    settings.endGroup();

    settings.beginGroup("advanced");
    settings.setValue("prefixDefaultPath", txtDefPrefixPath->text());
    if (cbOpenRunDialog->isChecked()){
        settings.setValue("openRunDialog", 1);
    } else {
        settings.setValue("openRunDialog", 0);
    }

    if (cbUseSingleClick->isChecked()){
        settings.setValue("useSingleClick", 1);
    } else {
        settings.setValue("useSingleClick", 0);
    }

    if (cbHideArguments->isChecked()){
        settings.setValue("hideArguments", 1);
    } else {
        settings.setValue("hideArguments", 0);
    }

    if (cbUseNativeDialog->isChecked()){
        settings.setValue("useNativeFileDialog", 1);
    } else {
        settings.setValue("useNativeFileDialog", 0);
    }

    QString desktopSize=cboxDesktopSize->currentText();
    if (desktopSize==tr("No virtual desktop")){
        desktopSize="";
    } else {
        QRegExp res_match("^\\d{3,}x\\d{3,}$");
        if (!res_match.exactMatch(desktopSize)){
            QMessageBox::warning(this, tr("Error"), tr("Invalid virtual desktop size."));
            return;
        }
    }

    settings.setValue("defaultDesktopSize", desktopSize);

    settings.endGroup();

    settings.beginGroup("DesktopImport");
    if (cbRemoveDesktopFiles->isChecked()){
        settings.setValue("remove", 1);
    } else {
        settings.setValue("remove", 0);
    }

    if (cbImportDesktopFilesAtStartup->isChecked()){
        settings.setValue("importAtStartup", 1);
    } else {
        settings.setValue("importAtStartup", 0);
    }
    settings.endGroup();

    settings.beginGroup("AppDB");
    if (cbUseSystemBrowser->isChecked()){
        settings.setValue("useSystemBrowser", 1);
    } else {
        settings.setValue("useSystemBrowser", 0);
    }
    settings.endGroup();
#ifndef _OS_DARWIN_
    settings.beginGroup("Plugins");
    if (cbEnableDesktopMenu->isChecked()){
        settings.setValue("enableMenuDesktop", 1);
    } else {
        settings.setValue("enableMenuDesktop", 0);
    }
    if (cbExportSystemFolder->isChecked()){
        settings.setValue("ExportSystemFolder", 1);
    } else {
        settings.setValue("ExportSystemFolder", 0);
    }
    settings.endGroup();
#endif

    settings.beginGroup("DesktopIntegration");
    settings.setValue("WineDesktop", this->txtWineDesktop->text());
    settings.setValue("WineDesktopDoc", this->txtWineDesktopDoc->text());
    settings.setValue("WineDesktopMus", this->txtWineDesktopMus->text());
    settings.setValue("WineDesktopPic", this->txtWineDesktopPic->text());
    settings.setValue("WineDesktopVid", this->txtWineDesktopVid->text());
    settings.endGroup();

    accept();
    return;
}

bool AppSettings::checkEntry(QString fileName, QString info, bool isFile){
    /*
      *	This function check user entry
      */

    if (fileName.isEmpty()){
        if (!isFile){
            QMessageBox::warning(this, tr("Error"), tr("Sorry, specify %1 directory.").arg(info));
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Sorry, specify %1 binary.").arg(info));
        }
        return false;
    } else {
        if (!QFile::exists(fileName)){
            if (!isFile){
                QMessageBox::warning(this, tr("Error"), tr("Sorry, specified %1 directory does not exist.").arg(info));
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Sorry, specified %1 binary does not exist.").arg(info));
            }
            return false;
        }
    }

    return true;
}

void AppSettings::cmdHelp_Click(){
    QString rawurl;

    std::unique_ptr<QTreeWidgetItem> item (optionsTree->currentItem());
    if (!item.get()){
         item.release();
         return;
    }

    QString itemText = item->text(0);

    if (itemText==tr("General")){
        rawurl = "11-settings.html#general";
    } else if (itemText==tr("System")){
        rawurl = "11-settings.html#sysutils";
    } else if (itemText==tr("Utils")){
        rawurl = "11-settings.html#userutils";
    } else if (itemText==tr("Network")){
        rawurl = "11-settings.html#network";
    } else if (itemText==tr("Quick Mount")){
        rawurl = "11-settings.html#qmount";
    } else if (itemText==tr("Interface")){
        rawurl = "11-settings.html#interface";
    } else if (itemText==tr("Language")){
        rawurl = "11-settings.html#interface";
    } else if (itemText==tr("Themes")){
        rawurl = "11-settings.html#interface";
    } else if (itemText==tr("Subsystems")){
        rawurl = "11-settings.html#logging";
    } else if (itemText==tr("Logging")){
        rawurl = "11-settings.html#logging";
    } else if (itemText==tr("AppDB browser")){
        rawurl = "11-settings.html#appdb";
    } else if (itemText==tr("Plugins")){
        rawurl = "11-settings.html#plugins";
    } else if (itemText==tr("Desktop Menu")){
        rawurl = "11-settings.html#desktopmenu";
    } else if (itemText==tr("Advanced")){
        rawurl = "11-settings.html#defaults";
    } else if (itemText==tr("Defaults")){
        rawurl = "11-settings.html#defaults";
    } else if (itemText==tr("Desktop Integration")){
        rawurl = "11-settings.html#desktopintegration";
    } else if (itemText==tr("Run dialog")){
        rawurl = "11-settings.html#rundialog";
    } else if (itemText==tr("Wine desktop import")){
        rawurl = "11-settings.html#wineimport";
    }

    item.release();

    CoreLib->openHelpUrl(rawurl);
    return;
}

void AppSettings::comboMountProfiles_currentIndexChanged(int index){
    switch (index){
    case 2:
        if (CoreLib->getWhichOut("fusermount").isEmpty()){
            this->comboMountProfiles->setCurrentIndex(0);
            return;
        }
        if (CoreLib->getWhichOut("fuseiso").isEmpty()){
            this->comboMountProfiles->setCurrentIndex(0);
            return;
        }
        break;
    }

    txtMountString->setText(CoreLib->getMountString(index));
    txtMountImageString->setText(CoreLib->getMountImageString(index));
    txtUmountString->setText(CoreLib->getUmountString(index));
}

void AppSettings::cbShowTray_stateChanged ( int state ){
    if (state==0){
        cbShowNotifications->setEnabled(false);
    } else {
        cbShowNotifications->setEnabled(true);
    }
}

void AppSettings::cmdDesktopMenu_Regen_Click(){

    //save changes in order to apply user-selected settings
    QSettings settings(APP_SHORT_NAME, "default");
    settings.beginGroup("Plugins");
    if (cbEnableDesktopMenu->isChecked()){
        settings.setValue("enableMenuDesktop", 1);
    } else {
        settings.setValue("enableMenuDesktop", 0);
    }
    if (cbExportSystemFolder->isChecked()){
        settings.setValue("ExportSystemFolder", 1);
    } else {
        settings.setValue("ExportSystemFolder", 0);
    }
    settings.endGroup();

    system_menu sys_menu;
    sys_menu.create_dir_info();
    sys_menu.generateSystemMenu();
}

void AppSettings::cmdDesktopMenu_Remove_Click(){
    system_menu sys_menu;
    sys_menu.wipeSystemMenu();
}

void AppSettings::cmdVersionManager_Click(){
    VersionManager* vers = new VersionManager();
    vers->exec();
    delete(vers);
}
