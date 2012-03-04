/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                           *
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

#include "fakedrivesettings.h"

FakeDriveSettings::FakeDriveSettings(QString prefixName, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
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

    this->prefixName=prefixName;
    wine64=false;

    setupUi(this);

    splitter.reset(new QSplitter(widgetContent));
    splitter->addWidget(optionsTreeWidget);
    splitter->addWidget(optionsStack);

    QList<int> size;
    size << 150 << 150;

    splitter->setSizes(size);

    std::auto_ptr<QVBoxLayout> vlayout (new QVBoxLayout);
    vlayout->addWidget(splitter.release());
    vlayout->setMargin(0);
    vlayout->setSpacing(0);
    widgetContent->setLayout(vlayout.release());

    setWindowTitle(tr("Fake drive settings"));
    lblCaption->setText(tr("Fake drive settings for prefix \"%1\"").arg(this->prefixName));

    connect(optionsTree, SIGNAL(itemClicked (QTreeWidgetItem *, int)), this, SLOT(optionsTree_itemClicked ( QTreeWidgetItem *, int)));

    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
    QString pic="", line="", prefixPath="";
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));

    loadThemeIcons();

    QList<QTreeWidgetItem *> items = optionsTree->findItems (tr("General"), Qt::MatchExactly);
    if (items.count()>0){
        items.at(0)->setExpanded(true);
        optionsTree->setCurrentItem(items.at(0));
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(0);
    }

    cmdGetWineDesktop->installEventFilter(this);
    cmdGetWineDesktopDoc->installEventFilter(this);
    cmdGetWineDesktopPic->installEventFilter(this);
    cmdGetWineDesktopMus->installEventFilter(this);
    cmdGetWineDesktopVid->installEventFilter(this);



}

void FakeDriveSettings::optionsTree_itemClicked ( QTreeWidgetItem *item, int){
    if (!item)
        return;

    item->setExpanded(true);

    QString itemText = item->text(0);

    if (itemText==tr("General")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(0);
    } else if (itemText==tr("Color Theme")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(1);
    } else if (itemText==tr("Wine Browsers")){
        optionsStack->setCurrentIndex(0);
        tabwGeneral->setCurrentIndex(2);
    } else if (itemText==tr("Video")){
        optionsStack->setCurrentIndex(1);
        tabwVideo->setCurrentIndex(0);
    } else if (itemText==tr("Direct3D") && item->parent()->text(0)==tr("Video")){
        optionsStack->setCurrentIndex(1);
        tabwVideo->setCurrentIndex(1);
    } else if (itemText==tr("OpenGL")){
        optionsStack->setCurrentIndex(1);
        tabwVideo->setCurrentIndex(2);
    } else if (itemText==tr("X11 Driver") && item->parent()->text(0)==tr("Video")){
        optionsStack->setCurrentIndex(1);
        tabwVideo->setCurrentIndex(3);
    } else if (itemText==tr("File System")){
        optionsStack->setCurrentIndex(3);
        tabwFileSystem->setCurrentIndex(0);
    } else if (itemText==tr("Wine Drives")){
        optionsStack->setCurrentIndex(3);
        tabwFileSystem->setCurrentIndex(0);
    } else if (itemText==tr("Desktop Paths")){
        optionsStack->setCurrentIndex(3);
        tabwFileSystem->setCurrentIndex(1);
    } else if (itemText==tr("Audio")){
        optionsStack->setCurrentIndex(2);
        tabwAudio->setCurrentIndex(0);
    } else if (itemText==tr("Sound Driver")){
        optionsStack->setCurrentIndex(2);
        tabwAudio->setCurrentIndex(0);
    } else if (itemText==tr("ALSA Driver")){
        optionsStack->setCurrentIndex(2);
        tabwAudio->setCurrentIndex(1);
    } else if (itemText==tr("Misc Audio")){
        optionsStack->setCurrentIndex(2);
        tabwAudio->setCurrentIndex(2);
    } else if (itemText==tr("Input")){
        optionsStack->setCurrentIndex(4);
        tabwInput->setCurrentIndex(0);
    } else if (itemText==tr("Direct3D") && item->parent()->text(0)==tr("Input")){
        optionsStack->setCurrentIndex(4);
        tabwInput->setCurrentIndex(0);
    } else if (itemText==tr("X11 Driver") && item->parent()->text(0)==tr("Input")){
        optionsStack->setCurrentIndex(4);
        tabwInput->setCurrentIndex(1);
    }
}

void FakeDriveSettings::loadPrefixSettings(){
    if (CoreLib->getWineProcessList(this->prefixName).count()>0){
        Progress prog(1, this->prefixName);
        if (prog.exec()==QDialog::Rejected){
            QTimer::singleShot(1000, this, SLOT(cmdCancel_Click()));
            return;
        }
    }
    //if (CoreLib->getWineProcessList(this->prefixName))
    this->loadSettings();
    return;
}

void FakeDriveSettings::loadDefaultPrefixSettings(){
    this->loadDefaultSettings();
    return;
}

void FakeDriveSettings::cmdOk_Click(){
#ifdef DEBUG
    qDebug()<<"[ii] Saveing fake drive settings";
#endif
    QRegExp rx("^\".*\"=\".*\"$");
    QList<QListWidgetItem *> listItems = listJoystickAxesMappings->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

#ifdef DEBUG
    qDebug()<<"[ii] Check joystic axes mapping syntax";
#endif

    for (int i=0; i < listItems.count(); i++){
        if (rx.indexIn(listItems.at(i)->text())!=0){
            QMessageBox::warning(this, tr("Error"), tr("Error in string:\n\n%1\n\nJoystick axes mappings might be defined as:\n\"Joystick name\"=\"axes mapping\"\n\nFor example:\n\"Logitech Logitech Dual Action\"=\"X,Y,Rz,Slider1,POV1\"\n\nSee help for details.").arg(listItems.at(i)->text()));
            return;
        }
    }

    QApplication::setOverrideCursor( Qt::BusyCursor );
    this->setEnabled(false);

#ifdef DEBUG
    qDebug()<<"[ii] Getting version";
#endif

    //Getting versions
    QString version;
    if (comboFakeVersion->currentText()=="Windows XP"){
        version = "winxp";
    } else if (comboFakeVersion->currentText()=="Windows 2008"){
        version = "win2008";
    } else if (comboFakeVersion->currentText()=="Windows 7"){
        version = "win7";
    } else  if (comboFakeVersion->currentText()=="Windows Vista"){
        version = "vista";
    } else if (comboFakeVersion->currentText()=="Windows 2003"){
        version = "win2003";
    } else if (comboFakeVersion->currentText()=="Windows 2000"){
        version = "win2k";
    } else if (comboFakeVersion->currentText()=="Windows ME"){
        version = "winme";
    } else if (comboFakeVersion->currentText()=="Windows 98"){
        version = "win98";
    } else if (comboFakeVersion->currentText()=="Windows 95"){
        version = "win95";
    } else if (comboFakeVersion->currentText()=="Windows NT 4.0"){
        version = "nt40";
    } else if (comboFakeVersion->currentText()=="Windows NT 3.0"){
        version = "nt351";
    } else if (comboFakeVersion->currentText()=="Windows 3.1"){
        version = "win31";
    } else if (comboFakeVersion->currentText()=="Windows 3.0"){
        version = "win30";
    } else if (comboFakeVersion->currentText()=="Windows 2.0"){
        version = "win20";
    } else {
        version = "winxp";
    }

    Registry reg(db_prefix.getPath(prefixName));
    QStringList list;
    list << "\"Desktop\""<<"\"My Music\""<<"\"My Pictures\""<<"\"My Videos\""<<"\"Personal\"";
    list = reg.readKeys("user", "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", list);

    QString prefixPath = db_prefix.getPath(prefixName);

    QDir dir;
    QFile file;
    QFileInfo fileInfo;

    QString userDir = prefixPath;
    userDir.append("/drive_c/users/");
    userDir.append(getenv("USER"));

    if (list.count()==5){
                desktopFolder = QString("%1/%2").arg(userDir).arg(CoreLib->decodeRegString(list.at(0).split("\\\\").last()));
                desktopMusic = QString("%1/%2").arg(userDir).arg(CoreLib->decodeRegString(list.at(1).split("\\\\").last()));
                desktopPictures = QString("%1/%2").arg(userDir).arg(CoreLib->decodeRegString(list.at(2).split("\\\\").last()));
                desktopDocuments = QString("%1/%2").arg(userDir).arg(CoreLib->decodeRegString(list.at(4).split("\\\\").last()));
                desktopVideos = QString("%1/%2").arg(userDir).arg(CoreLib->decodeRegString(list.at(3).split("\\\\").last()));
    } else {
         QMessageBox::warning(this, tr("Error"), tr("Can't read desktop paths!"));
        this->reject();
        return;
    }

#ifdef DEBUG
        qDebug()<<"[ii] Localizad folders path: "<<desktopFolder<<desktopDocuments<<desktopMusic<<desktopPictures<<desktopVideos;
#endif

    QString sh_cmd = "";
    QStringList sh_line;



        dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot  );

        prefixPath.append("/dosdevices/");
        if (!dir.cd(prefixPath)){
        qDebug()<<"[ee] Cannot cd to prefix directory: "<<prefixPath;
    } else {
                QFileInfoList drivelist = dir.entryInfoList();
                QFile file;
        for (int i = 0; i < drivelist.size(); ++i) {
            QFileInfo fileInfo = drivelist.at(i);
            if (fileInfo.isSymLink()){
                            if (!file.remove(fileInfo.filePath())){
                                QMessageBox::warning(this, tr("Error"), tr("Can't remove symlink: %1").arg(fileInfo.filePath()));
                                QApplication::restoreOverrideCursor();
                                reject();
                            }
            }
        }
    }

    if (listWineDrives->count()>0){
        for (int i=0; i<listWineDrives->count(); i++){
            std::auto_ptr<DriveListWidgetItem> item (dynamic_cast<DriveListWidgetItem*>(listWineDrives->item(i)));

            if (item.get()){
                            QString driveLink = QString("%1/%2").arg(prefixPath).arg(item->getLetter().toLower());
                            if (!file.link(item->getPath(), driveLink)){
                                QMessageBox::warning(this, tr("Error"), tr("Can't symlink \"%1\" to \"%2\"").arg(item->getPath()).arg(driveLink));
                                QApplication::restoreOverrideCursor();
                                reject();
                                return;
                            }
            }
            item.release();
        }
    }

        // Check and create user path
        if (!dir.exists(prefixPath)){
            if (!dir.mkdir(prefixPath)){
                QMessageBox::warning(this, tr("Error"), tr("Can't create dir: %1").arg(prefixPath));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        }

        // Desktop foolder
        fileInfo.setFile(desktopFolder);
        if (fileInfo.isSymLink() || fileInfo.exists()){
            if (!dir.remove(desktopFolder)){
                QMessageBox::warning(this, tr("Error"), tr("Can't remove dir: %1").arg(desktopFolder));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        }

        if (!txtWineDesktop->text().isEmpty()){
            /*
            file.setFileName(desktopFolder);
            if (!file.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this, tr("Error"), tr("Can't create file \"%1\"").arg(desktopFolder));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        } else {*/
            if (!file.link(txtWineDesktop->text(), desktopFolder)){
                QMessageBox::warning(this, tr("Error"), tr("Can't symlink \"%1\" to \"%2\"").arg(txtWineDesktop->text()).arg(desktopFolder));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        }

        // Doc folder
        if (!txtWineDesktopDoc->text().isEmpty()){
            fileInfo.setFile(desktopDocuments);
            if (fileInfo.isSymLink() || fileInfo.exists()){
                if (!dir.remove(desktopDocuments)){
                    QMessageBox::warning(this, tr("Error"), tr("Can't remove dir: %1").arg(desktopDocuments));
                    QApplication::restoreOverrideCursor();
                    reject();
                    return;
                }
            }

            if (!file.link(txtWineDesktopDoc->text(), desktopDocuments)){
                QMessageBox::warning(this, tr("Error"), tr("Can't symlink \"%1\" to \"%2\"").arg(txtWineDesktopDoc->text()).arg(desktopDocuments));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        }

        // Music folder
        if (!txtWineDesktopMus->text().isEmpty()){
            fileInfo.setFile(desktopMusic);
            if (fileInfo.isSymLink() || fileInfo.exists()){
                if (!dir.remove(desktopMusic)){
                    QMessageBox::warning(this, tr("Error"), tr("Can't remove dir: %1").arg(desktopMusic));
                    QApplication::restoreOverrideCursor();
                    reject();
                    return;
                }
            }

            if (!file.link(txtWineDesktopMus->text(), desktopMusic)){
                QMessageBox::warning(this, tr("Error"), tr("Can't symlink \"%1\" to \"%2\"").arg(txtWineDesktopMus->text()).arg(desktopMusic));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        }

        // Pic folder
        if (!txtWineDesktopPic->text().isEmpty()){
            fileInfo.setFile(desktopPictures);
            if (fileInfo.isSymLink() || fileInfo.exists()){
                if (!dir.remove(desktopPictures)){
                    QMessageBox::warning(this, tr("Error"), tr("Can't remove dir: %1").arg(desktopPictures));
                    QApplication::restoreOverrideCursor();
                    reject();
                    return;
                }
            }

            if (!file.link(txtWineDesktopPic->text(), desktopPictures)){
                QMessageBox::warning(this, tr("Error"), tr("Can't symlink \"%1\" to \"%2\"").arg(txtWineDesktopPic->text()).arg(desktopPictures));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        }

        // Vid folder
        if (!txtWineDesktopVid->text().isEmpty()){
            fileInfo.setFile(desktopVideos);
            if (fileInfo.isSymLink() || fileInfo.exists()){
                if (!dir.remove(desktopVideos)){
                    QMessageBox::warning(this, tr("Error"), tr("Can't remove dir: %1").arg(desktopVideos));
                    QApplication::restoreOverrideCursor();
                    reject();
                    return;
                }
            }

            if (!file.link(txtWineDesktopVid->text(), desktopVideos)){
                QMessageBox::warning(this, tr("Error"), tr("Can't symlink \"%1\" to \"%2\"").arg(txtWineDesktopVid->text()).arg(desktopVideos));
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }
        }

    // ---- End of Creating Dos drives ----

    Registry registry;

    if (!registry.init()){
        QApplication::restoreOverrideCursor();
        this->reject();
        return;
    }

    if (this->wine64){
#ifdef DEBUG
        qDebug()<<"[ii] Writedown wine64 settings.";
#endif
        registry.set("Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOrganization", txtOrganization->text(), "HKEY_LOCAL_MACHINE");
        registry.set("Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOwner", txtOwner->text(), "HKEY_LOCAL_MACHINE");
    } else {
#ifdef DEBUG
        qDebug()<<"[ii] Writedown wine32 settings.";
#endif
        registry.set("Software\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOrganization", txtOrganization->text(), "HKEY_LOCAL_MACHINE");
        registry.set("Software\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOwner", txtOwner->text(), "HKEY_LOCAL_MACHINE");
    }

    registry.set("Software\\Wine", "Version", version);


    if (cbCrashDialog->isChecked()){
        registry.set("Software\\Wine\\WineDbg", "ShowCrashDialog", "dword:00000000");
    } else {
        registry.unset("Software\\Wine\\WineDbg", "ShowCrashDialog");
    }

    if (listWineDrives->count()>0){

#ifdef DEBUG
    qDebug()<<"[ii] Wizard::Create wine drive config";
#endif

    for (int i=0; i<listWineDrives->count(); i++){
        std::auto_ptr<DriveListWidgetItem> item (dynamic_cast<DriveListWidgetItem*>(listWineDrives->item(i)));
#ifdef DEBUG
        qDebug()<<"[ii] Wizard::get DriveListWidgetItem for "<<listWineDrives->item(i)->text();
#endif
        if (item.get()){
            if (item->getType()=="auto"){
                registry.unset("Software\\Wine\\Drives", item->getLetter().toLower(), "HKEY_LOCAL_MACHINE");
            } else {
                registry.set("Software\\Wine\\Drives", "", QString("\"%1\"=\"%2\"").arg(item->getLetter().toLower()).arg(item->getType()), "HKEY_LOCAL_MACHINE");
            }
        }
        item.release();
    }
}

#ifdef DEBUG
    qDebug()<<"[ii] Wizard::creating registry cfg";
#endif

    if (!txtFakeBrowsers->text().isEmpty()){
        registry.set("Software\\Wine\\WineBrowser", "Browsers", txtFakeBrowsers->text());
    } else {
        registry.unset("Software\\Wine\\WineBrowser", "Browsers");
    }

    if (!txtFakeMailers->text().isEmpty()){
        registry.set("Software\\Wine\\WineBrowser", "Mailers", txtFakeMailers->text());
    } else {
        registry.unset("Software\\Wine\\WineBrowser", "Mailers");
    }

    if (comboFakeD3D_Multi->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "Multisampling", comboFakeD3D_Multi->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "Multisampling");
    }

    if (comboFakeD3D_Render->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "DirectDrawRenderer", comboFakeD3D_Render->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "DirectDrawRenderer");
    }

    if (comboFakeD3D_LMode->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "RenderTargetLockMode", comboFakeD3D_LMode->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "RenderTargetLockMode");
    }

    if (comboFakeD3D_SDOrder->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "StrictDrawOrdering", comboFakeD3D_SDOrder->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "StrictDrawOrdering");
    }

    if (comboFakeD3D_Offscreen->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "OffscreenRenderingMode", comboFakeD3D_Offscreen->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "OffscreenRenderingMode");
    }

    if (comboFakeD3D_Offscreen->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "OffscreenRenderingMode", comboFakeD3D_Offscreen->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "OffscreenRenderingMode");
    }

    if (comboFakeD3D_GLSL->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "UseGLSL", comboFakeD3D_GLSL->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "UseGLSL");
    }

    if (comboFakeD3D_N2M->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "Nonpower2Mode", comboFakeD3D_N2M->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "Nonpower2Mode");
    }

    if (!txtFakeVideoMemory->text().isEmpty()){
        registry.set("Software\\Wine\\Direct3D", "VideoMemorySize", txtFakeVideoMemory->text());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "VideoMemorySize");
    }

    if (!txtFakeVideoDescription->text().isEmpty()){
        registry.set("Software\\Wine\\Direct3D", "VideoDescription", txtFakeVideoDescription->text());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "VideoDescription");
    }

    if (!txtFakeVideoDriver->text().isEmpty()){
        registry.set("Software\\Wine\\Direct3D", "VideoDriver", txtFakeVideoDriver->text());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "VideoDriver");
    }

    if (comboFakeSoftwareEmulation->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "SoftwareEmulation", comboFakeSoftwareEmulation->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "SoftwareEmulation");
    }

    if (comboFakePixelShaderMode->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "PixelShaderMode", comboFakePixelShaderMode->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "PixelShaderMode");
    }

    if (comboFakeVertexShaderMode->currentText()!="default"){
        registry.set("Software\\Wine\\Direct3D", "VertexShaderMode", comboFakeVertexShaderMode->currentText());
    } else {
        registry.unset("Software\\Wine\\Direct3D", "VertexShaderMode");
    }

    if (!txtFakeDisabledExtensions->text().isEmpty()){
        registry.set("Software\\Wine\\OpenGL", "DisabledExtensions", txtFakeDisabledExtensions->text());
    } else {
        registry.unset("Software\\Wine\\OpenGL", "DisabledExtensions");
    }

    if (sboxFakeInput_scroll->value()>0){
        registry.set("Control Panel\\Desktop", "WheelScrollLines", QString("%1").arg(sboxFakeInput_scroll->value()));
    } else {
        registry.unset("Control Panel\\Desktop", "WheelScrollLines");
    }

    if (comboFakeInput_selection->currentText()!="default"){
        registry.set("Software\\Wine\\X11 Driver", "UsePrimarySelection", comboFakeInput_selection->currentText());
    } else {
        registry.unset("Software\\Wine\\X11 Driver", "UsePrimarySelection");
    }

    registry.unsetPath("Software\\Wine\\DirectInput");

    if (listJoystickAxesMappings->count()>0){
#ifdef DEBUG
        qDebug()<<"[ii] Wizard::creating registry cfg for joystik";
#endif

        for (int i=0; i<listJoystickAxesMappings->count(); i++){
            registry.set("Software\\Wine\\DirectInput", "", listJoystickAxesMappings->item(i)->text());
        }
    }

#ifdef DEBUG
    qDebug()<<"[ii] Wizard::creating registry cfg";
#endif


    if (comboFakeMouseWarp->currentText()!="default"){
        registry.set("Software\\Wine\\DirectInput", "MouseWarpOverride", comboFakeMouseWarp->currentText());
    } else {
        registry.unset("Software\\Wine\\DirectInput", "MouseWarpOverride");
    }

    if (comboFakeX11_WR->currentText()!="default"){
        registry.set("Software\\Wine\\X11 Driver", "ClientSideWithRender", comboFakeX11_WR->currentText());
    } else {
        registry.unset("Software\\Wine\\X11 Driver", "ClientSideWithRender");
    }

    if (comboFakeX11_AAR->currentText()!="default"){
        registry.set("Software\\Wine\\X11 Driver", "ClientSideAntiAliasWithRender", comboFakeX11_AAR->currentText());
    } else {
        registry.unset("Software\\Wine\\X11 Driver", "ClientSideAntiAliasWithRender");
    }

    if (comboFakeX11_AAC->currentText()!="default"){
        registry.set("Software\\Wine\\X11 Driver", "ClientSideAntiAliasWithCore", comboFakeX11_AAC->currentText());
    } else {
        registry.unset("Software\\Wine\\X11 Driver", "ClientSideAntiAliasWithCore");
    }

    if (comboFakeX11_XRandr->currentText()!="default"){
        registry.set("Software\\Wine\\X11 Driver", "UseXRandR", comboFakeX11_XRandr->currentText());
    } else {
        registry.unset("Software\\Wine\\X11 Driver", "UseXRandR");
    }

    if (comboFakeX11_XVid->currentText()!="default"){
        registry.set("Software\\Wine\\X11 Driver", "UseXVidMode", comboFakeX11_XVid->currentText());
    } else {
        registry.unset("Software\\Wine\\X11 Driver", "UseXVidMode");
    }

    if (comboFakeSound_Driver->currentText()!="default"){
        if (comboFakeSound_Driver->currentText()=="disabled"){
            registry.set("Software\\Wine\\Drivers", "Audio", "");
        } else {
            registry.set("Software\\Wine\\Drivers", "Audio", comboFakeSound_Driver->currentText());
        }
    } else {
        registry.unset("Software\\Wine\\Drivers", "Audio");
    }

    if (comboFakeAlsa_asCards->currentText()!="default"){
        registry.set("Software\\Wine\\Alsa Driver", "AutoScanCards", comboFakeAlsa_asCards->currentText());
    } else {
        registry.unset("Software\\Wine\\Alsa Driver", "AutoScanCards");
    }

    if (comboFakeAlsa_asDevices->currentText()!="default"){
        registry.set("Software\\Wine\\Alsa Driver", "AutoScanDevices", comboFakeAlsa_asDevices->currentText());
    } else {
        registry.unset("Software\\Wine\\Alsa Driver", "AutoScanDevices");
    }

    if (sboxFakeAlsa_devCount->value()>0){
        registry.set("Software\\Wine\\Alsa Driver", "DeviceCount", QString("%1").arg(sboxFakeAlsa_devCount->value()));
    } else {
        registry.unset("Software\\Wine\\Alsa Driver", "DeviceCount");
    }

    if (!txtFakeAlsa_CTLn->text().isEmpty()){
        registry.set("Software\\Wine\\Alsa Driver", "DeviceCTLn", txtFakeAlsa_CTLn->text());
    } else {
        registry.unset("Software\\Wine\\Alsa Driver", "DeviceCTLn");
    }

    if (!txtFakeAlsa_PCMn->text().isEmpty()){
        registry.set("Software\\Wine\\Alsa Driver", "DevicePCMn", txtFakeAlsa_PCMn->text());
    } else {
        registry.unset("Software\\Wine\\Alsa Driver", "DevicePCMn");
    }

    if (comboFakeAlsa_DirectHW->currentText()!="default"){
        registry.set("Software\\Wine\\Alsa Driver", "UseDirectHW", comboFakeAlsa_DirectHW->currentText());
    } else {
        registry.unset("Software\\Wine\\Alsa Driver", "UseDirectHW");
    }

    if (sboxFakeSound_shadow->value()>-1){
        registry.set("Software\\Wine\\DirectSound", "MaxShadowSize", QString("%1").arg(sboxFakeSound_shadow->value()));
    } else {
        registry.unset("Software\\Wine\\DirectSound", "MaxShadowSize");
    }

    if (rbColorsDefault->isChecked()){
        registry.unsetPath("Control Panel\\Colors");
    }

    if (rbColorsQt4->isChecked()){

#ifdef DEBUG
        qDebug()<<"[ii] Wizard::creating registry cfg for color";
#endif

        QColor color;
        QPalette cur_palette;

        cur_palette = qApp->palette();
        color = cur_palette.color(QPalette::Base);
        registry.set("Control Panel\\Colors", "Window", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::Window);
        registry.set("Control Panel\\Colors", "ActiveBorder", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "InactiveBorder", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "AppWorkSpace", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "Menu", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "MenuBar", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "Scrollbar", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "MenuHilight", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "ButtonFace", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::AlternateBase);
        registry.set("Control Panel\\Colors", "ButtonAlternateFace", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::Dark);
        registry.set("Control Panel\\Colors", "ButtonDkShadow", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "ButtonShadow", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "GrayText", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::Light);
        registry.set("Control Panel\\Colors", "ButtonHilight", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::ButtonText);
        registry.set("Control Panel\\Colors", "ButtonText", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::WindowText);
        registry.set("Control Panel\\Colors", "MenuText", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "WindowFrame", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
        registry.set("Control Panel\\Colors", "WindowText", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::Highlight);
        registry.set("Control Panel\\Colors", "Hilight", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::HighlightedText);
        registry.set("Control Panel\\Colors", "HilightText", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::ToolTipBase);
        registry.set("Control Panel\\Colors", "InfoWindow", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));

        color = cur_palette.color(QPalette::ToolTipText);
        registry.set("Control Panel\\Colors", "InfoText", QString("%1 %2 %3").arg(QString::number(color.red())) .arg(QString::number(color.green())) .arg(QString::number(color.blue())));
    }

#ifdef DEBUG
    qDebug()<<"[ii] Wizard::run registry import";
#endif

        if (registry.exec(this, db_prefix.getPath(prefixName), prefixName)){
        CoreLib->createPrefixDBStructure(prefixName);
#ifdef DEBUG
    qDebug()<<"[ii] Wizard::done";
#endif
    QTimer::singleShot(3000, this, SLOT(waitForWineEnd()));
        } else {
            QApplication::restoreOverrideCursor();
            reject();
        }
    return;
}

void FakeDriveSettings::waitForWineEnd(){
    QApplication::restoreOverrideCursor();
    this->accept();
}

void FakeDriveSettings::waitForWine(){
    QString prefixPath = db_prefix.getPath(this->prefixName);
    QFile file;

    if (file.exists(QString("%1/%2").arg(prefixPath).arg("system.reg")) && file.exists(QString("%1/%2").arg(prefixPath).arg("user.reg"))){
        QApplication::restoreOverrideCursor();
        this->setEnabled(true);
    } else {
        QTimer::singleShot(1000, this, SLOT(waitForWine()));
    }
}

void FakeDriveSettings::cmdCancel_Click(){
    this->reject();
    return;
}

void FakeDriveSettings::cmdHelp_Click(){
    QString rawurl;

    std::auto_ptr<QTreeWidgetItem> item (optionsTree->currentItem());
    if (!item.get()){
         item.release();
         return;
    }

    QString itemText = item->text(0);

    if (itemText==tr("General")){
        rawurl = "07-fake-drive-settings.html#general";
    } else if (itemText==tr("Color Theme")){
        rawurl = "07-fake-drive-settings.html#colors";
    } else if (itemText==tr("Wine Browsers")){
        rawurl = "07-fake-drive-settings.html#winebrowsers";
    } else if (itemText==tr("Video")){
        rawurl = "07-fake-drive-settings.html#video";
    } else if (itemText==tr("Direct3D") && item->parent()->text(0)==tr("Video")){
        rawurl = "07-fake-drive-settings.html#direct";
    } else if (itemText==tr("OpenGL")){
        rawurl = "07-fake-drive-settings.html#opengl";
    } else if (itemText==tr("X11 Driver") && item->parent()->text(0)==tr("Video")){
        rawurl = "07-fake-drive-settings.html#x11-video";
    } else if (itemText==tr("File System")){
        rawurl = "07-fake-drive-settings.html#winedrive";
    } else if (itemText==tr("Wine Drives")){
        rawurl = "07-fake-drive-settings.html#winedrive";
    } else if (itemText==tr("Desktop Paths")){
        rawurl = "07-fake-drive-settings.html#desktop";
    } else if (itemText==tr("Audio")){
        rawurl = "07-fake-drive-settings.html#sound";
    } else if (itemText==tr("Sound Driver")){
        rawurl = "07-fake-drive-settings.html#sound";
    } else if (itemText==tr("ALSA Driver")){
        rawurl = "07-fake-drive-settings.html#alsa";
    } else if (itemText==tr("Misc Audio")){
        rawurl = "07-fake-drive-settings.html#sound-misc";
    } else if (itemText==tr("Input")){
        rawurl = "07-fake-drive-settings.html#d3d-input";
    } else if (itemText==tr("Direct3D") && item->parent()->text(0)==tr("Input")){
        rawurl = "07-fake-drive-settings.html#d3d-input";
    } else if (itemText==tr("X11 Driver") && item->parent()->text(0)==tr("Input")){
        rawurl = "07-fake-drive-settings.html#x11-input";
    }

    item.release();

    CoreLib->openHelpUrl(rawurl);
    return;
}

void FakeDriveSettings::loadThemeIcons(){
    lblLogo->setPixmap(CoreLib->loadPixmap("data/exec.png"));
    connect(cmdJoystickEdit, SIGNAL(clicked()), this, SLOT(cmdJoystickEdit_Click()));
    connect(cmdJoystickAdd, SIGNAL(clicked()), this, SLOT(cmdJoystickAdd_Click()));
    connect(cmdJoystickDel, SIGNAL(clicked()), this, SLOT(cmdJoystickDel_Click()));
    connect(cmdWineDriveEdit, SIGNAL(clicked()), this, SLOT(cmdWineDriveEdit_Click()));
    connect(cmdWineDriveAdd, SIGNAL(clicked()), this, SLOT(cmdWineDriveAdd_Click()));
    connect(cmdWineDriveDel, SIGNAL(clicked()), this, SLOT(cmdWineDriveDel_Click()));
    cmdGetWineDesktop->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWineDesktopDoc->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWineDesktopPic->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWineDesktopMus->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWineDesktopVid->setIcon(CoreLib->loadIcon("data/folder.png"));

    return;
}

void FakeDriveSettings::cmdJoystickEdit_Click(){
    QListWidgetItem *item = listJoystickAxesMappings->currentItem();
    if (!item)
        return;

    bool ok;
    QString text = QInputDialog::getText(this, tr("Joystick Axes Mappings"), tr("Joystick axes mappings might be defined as:\n\"Joystick name\"=\"axes mapping\"\n\nFor example:\n\"Logitech Logitech Dual Action\"=\"X,Y,Rz,Slider1,POV1\"\n\nSee help for details."), QLineEdit::Normal, item->text(), &ok);
    if (ok && !text.isEmpty()){
        item->setText(text);
    }
    return;
}


void FakeDriveSettings::cmdJoystickAdd_Click(){
    bool ok;
    QString text = QInputDialog::getText(this, tr("Joystick Axes Mappings"), tr("Joystick axes mappings might be defined as:\n\"Joystick name\"=\"axes mapping\"\n\nFor example:\n\"Logitech Logitech Dual Action\"=\"X,Y,Rz,Slider1,POV1\"\n\nSee help for details."), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()){
        listJoystickAxesMappings->addItem(text);
    }
    return;
}

void FakeDriveSettings::cmdJoystickDel_Click(){
    std::auto_ptr<QListWidgetItem> item (listJoystickAxesMappings->currentItem());
    if (!item.get())
        return;

    delete item.release();
    return;
}

void FakeDriveSettings::cmdWineDriveEdit_Click(){
    std::auto_ptr<DriveListWidgetItem> item (dynamic_cast<DriveListWidgetItem*>(listWineDrives->currentItem()));

    if (!item.get())
        return;

    QStringList drives;
    drives.clear();
    if (listWineDrives->count()>0){
        for (int i=0; i<listWineDrives->count(); i++){
            if (listWineDrives->item(i)!=item.get())
                drives.append(listWineDrives->item(i)->text().left(2));
        }
    }

    WineDriveDialog drevedialog(drives, item->getLetter(), item->getPath(), item->getType());
    if (drevedialog.exec()==QDialog::Accepted){
        item->setDrive(drevedialog.getLetter(), drevedialog.getPath(), drevedialog.getType());
    }
    item.release();
    return;
}

void FakeDriveSettings::cmdWineDriveAdd_Click(){
    QStringList drives;
    drives.clear();
    if (listWineDrives->count()>0){
        for (int i=0; i<listWineDrives->count(); i++){
            drives.append(listWineDrives->item(i)->text().left(2));
        }
    }

    WineDriveDialog drevedialog(drives);
    if (drevedialog.exec()==QDialog::Accepted){
        std::auto_ptr<DriveListWidgetItem> item (new DriveListWidgetItem(listWineDrives));
        item->setDrive(drevedialog.getLetter(), drevedialog.getPath(), drevedialog.getType());
        listWineDrives->addItem(item.release());
    }
    return;
}

void FakeDriveSettings::cmdWineDriveDel_Click(){
    std::auto_ptr<QListWidgetItem> item (listWineDrives->currentItem());
    if (!item.get())
        return;

    if (item->text().left(2)=="C:"){
        QMessageBox::warning(this, tr("Error"), tr("Sorry, You can't delete or modify wine C: drive.<br>But You can change it in prefix settings dialog."));
        item.release();
        return;
    }

    delete item.release();
    return;
}

void FakeDriveSettings::loadSettings(){
    QString prefixPath = db_prefix.getPath(prefixName);
    std::auto_ptr<DriveListWidgetItem> item;

    if (prefixPath.isEmpty()){
        qDebug()<<" [EE] Cant get prefix path: "<<prefixName;
        return;
    }

    Registry reg(prefixPath);

    QStringList list;

    list.clear();
    list << "\"Desktop\""<<"\"My Music\""<<"\"My Pictures\""<<"\"My Videos\""<<"\"Personal\"";
    list = reg.readKeys("user", "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", list);

    if (list.count()==5){
        desktopFolder = CoreLib->decodeRegString(list.at(0).split("\\\\").last());
        desktopMusic = CoreLib->decodeRegString(list.at(1).split("\\\\").last());
        desktopPictures = CoreLib->decodeRegString(list.at(2).split("\\\\").last());
        desktopVideos = CoreLib->decodeRegString(list.at(3).split("\\\\").last());
        desktopDocuments = CoreLib->decodeRegString(list.at(4).split("\\\\").last());
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Can't read desktop paths!"));
        this->reject();
        return;
    }

    list.clear();
    list << "\"RegisteredOrganization\"" << "\"RegisteredOwner\"";
    list = reg.readKeys("system", "Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion", list);

    if (list.count()>0){
        txtOrganization->setText(list.at(0));
        txtOwner->setText(list.at(1));
    }

    if (txtOrganization->text().isEmpty() && txtOwner->text().isEmpty()){
        list.clear();
        list << "\"RegisteredOrganization\"" << "\"RegisteredOwner\"";
        list = reg.readKeys("system", "Software\\Microsoft\\Windows NT\\CurrentVersion", list);

        if (list.count()>0){
            txtOrganization->setText(list.at(0));
            txtOwner->setText(list.at(1));
        }
        this->wine64=false;
#ifdef DEBUG
        qDebug()<<"[ii] wine32 settings detected.";
#endif
    } else {
        this->wine64=true;
#ifdef DEBUG
        qDebug()<<"[ii] wine64 settings detected.";
#endif
    }

    list.clear();
    list << "\"Version\"";
    list = reg.readKeys("user", "Software\\Wine", list);
    if (list.count()>0){
        QString version = list.at(0);
        if (version == "win7"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 7"));
        } else if (version == "winxp"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows XP"));
        } else if (version == "win2008"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 2008"));
        } else if (version == "vista"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows Vista"));
        } else if (version == "win2003"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 2003"));
        } else if (version == "win2k"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 2000"));
        } else if (version == "winme"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows ME"));
        } else if (version == "win98"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 98"));
        } else if (version == "win95"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 95"));
        } else if (version == "nt40"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows NT 4.0"));
        } else if (version == "nt351"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows NT 3.0"));
        } else if (version == "win31"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 3.1"));
        } else if (version == "win30"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 3.0"));
        } else if (version == "win20"){
            comboFakeVersion->setCurrentIndex(comboFakeVersion->findText("Windows 2.0"));
        }
    }

    list.clear();
    list << "\"ShowCrashDialog\"";
    list = reg.readKeys("user", "Software\\Wine\\WineDbg", list);

    if (list.count()>0){
        if (!list.at(0).isEmpty())
            cbCrashDialog->setChecked(true);
    }

    list.clear();
    list << "\"Browsers\"" << "\"Mailers\"";
    list = reg.readKeys("user", "Software\\Wine\\WineBrowser", list);

    if (list.count()>0){
        txtFakeBrowsers->setText(list.at(0));
        txtFakeMailers->setText(list.at(1));
    }

    list.clear();
    list << "\"Multisampling\"" << "\"DirectDrawRenderer\"" << "\"RenderTargetLockMode\"" << "\"OffscreenRenderingMode\"" << "\"UseGLSL\"" << "\"VideoMemorySize\"" << "\"VideoDescription\"" << "\"VideoDriver\"" << "\"SoftwareEmulation\"" << "\"PixelShaderMode\"" << "\"VertexShaderMode\""<< "\"StrictDrawOrdering\""<< "\"Nonpower2Mode\"";
    list = reg.readKeys("user", "Software\\Wine\\Direct3D", list);

    if (list.count()>0){
        if (!list.at(0).isEmpty())
            comboFakeD3D_Multi->setCurrentIndex(comboFakeD3D_Multi->findText(list.at(0)));

        if (!list.at(1).isEmpty())
            comboFakeD3D_Render->setCurrentIndex(comboFakeD3D_Render->findText(list.at(1)));

        if (!list.at(2).isEmpty())
            comboFakeD3D_LMode->setCurrentIndex(comboFakeD3D_LMode->findText(list.at(2)));

        if (!list.at(3).isEmpty())
            comboFakeD3D_Offscreen->setCurrentIndex(comboFakeD3D_Offscreen->findText(list.at(3)));

        if (!list.at(4).isEmpty())
            comboFakeD3D_GLSL->setCurrentIndex(comboFakeD3D_GLSL->findText(list.at(4)));

        txtFakeVideoMemory->setText(list.at(5));
        txtFakeVideoDescription->setText(list.at(6));
        txtFakeVideoDriver->setText(list.at(7));

        if (!list.at(8).isEmpty())
            comboFakeSoftwareEmulation->setCurrentIndex(comboFakeSoftwareEmulation->findText(list.at(8)));

        if (!list.at(9).isEmpty())
            comboFakePixelShaderMode->setCurrentIndex(comboFakePixelShaderMode->findText(list.at(9)));

        if (!list.at(10).isEmpty())
            comboFakeVertexShaderMode->setCurrentIndex(comboFakeVertexShaderMode->findText(list.at(10)));

        if (!list.at(11).isEmpty())
            comboFakeD3D_SDOrder->setCurrentIndex(comboFakeD3D_SDOrder->findText(list.at(11)));

        if (!list.at(12).isEmpty())
            comboFakeD3D_N2M->setCurrentIndex(comboFakeD3D_N2M->findText(list.at(12)));
    }

    list.clear();
    list << "\"DisabledExtensions\"";
    list = reg.readKeys("user", "Software\\Wine\\OpenGL", list);

    if (list.count()>0){
        txtFakeDisabledExtensions->setText(list.at(0));
    }

    list.clear();
    list << "\"MouseWarpOverride\"";
    list = reg.readKeys("user", "Software\\Wine\\DirectInput", list);

    if (list.count()>0){
        if (!list.at(0).isEmpty())
            comboFakeMouseWarp->setCurrentIndex(comboFakeMouseWarp->findText(list.at(0)));
    }

    list.clear();
    list << "\"MouseWarpOverride\"";
    list = reg.readExcludedKeys("user", "Software\\Wine\\DirectInput", list, 10);

    if (list.count()>0){
        listJoystickAxesMappings->insertItems (0, list);
    }

    list.clear();
    list << "\"WheelScrollLines\"";
    list = reg.readKeys("user", "Control Panel\\Desktop", list);

    if (list.count()>0){
        if (!list.at(0).isEmpty())
            sboxFakeInput_scroll->setValue(list.at(0).toInt());
    }

    list.clear();
    list << "\"UsePrimarySelection\"";
    list = reg.readKeys("user", "Software\\Wine\\X11 Driver", list);

    if (list.count()>0){
        if (!list.at(0).isEmpty())
            comboFakeInput_selection->setCurrentIndex(comboFakeInput_selection->findText(list.at(0)));
    }

    list.clear();
    list << "\"ClientSideWithRender\"" << "\"ClientSideAntiAliasWithRender\"" << "\"ClientSideAntiAliasWithCore\"" << "\"UseXRandR\"" << "\"UseXVidMode\"";
    list = reg.readKeys("user", "Software\\Wine\\X11 Driver", list);

    if (list.count()>0){
        if (!list.at(0).isEmpty())
            comboFakeX11_WR->setCurrentIndex(comboFakeX11_WR->findText(list.at(0)));

        if (!list.at(1).isEmpty())
            comboFakeX11_AAR->setCurrentIndex(comboFakeX11_AAR->findText(list.at(1)));

        if (!list.at(2).isEmpty())
            comboFakeX11_AAC->setCurrentIndex(comboFakeX11_AAC->findText(list.at(2)));

        if (!list.at(3).isEmpty())
            comboFakeX11_XRandr->setCurrentIndex(comboFakeX11_XRandr->findText(list.at(3)));

        if (!list.at(4).isEmpty())
            comboFakeX11_XVid->setCurrentIndex(comboFakeX11_XVid->findText(list.at(4)));
    }

    list.clear();
    list << "\"Audio\"";
    list = reg.readKeys("user", "Software\\Wine\\Drivers", list);
    if (list.count()>0){
        if (list.at(0) == QString::null){
            comboFakeSound_Driver->setCurrentIndex(comboFakeSound_Driver->findText("default"));
        } else if (!list.at(0).isEmpty()){
            comboFakeSound_Driver->setCurrentIndex(comboFakeSound_Driver->findText(list.at(0)));
        } else {
            comboFakeSound_Driver->setCurrentIndex(comboFakeSound_Driver->findText("disabled"));
        }
    }

    list.clear();
    list << "\"AutoScanCards\"" << "\"AutoScanDevices\"" << "\"DeviceCount\"" << "\"DeviceCTLn\"" << "\"DevicePCMn\"" << "\"UseDirectHW\"";
    list = reg.readKeys("user", "Software\\Wine\\Alsa Driver", list);

    if (list.count()>0){
        if (!list.at(0).isEmpty())
            comboFakeAlsa_asCards->setCurrentIndex(comboFakeAlsa_asCards->findText(list.at(0)));

        if (!list.at(1).isEmpty())
            comboFakeAlsa_asDevices->setCurrentIndex(comboFakeAlsa_asDevices->findText(list.at(1)));

        sboxFakeAlsa_devCount->setValue(list.at(2).toInt());
        txtFakeAlsa_CTLn->setText(list.at(3));
        txtFakeAlsa_PCMn->setText(list.at(4));

        if (!list.at(5).isEmpty())
            comboFakeAlsa_DirectHW->setCurrentIndex(comboFakeAlsa_DirectHW->findText(list.at(5)));
    }

    list.clear();
    list << "\"MaxShadowSize\"";
    list = reg.readKeys("user", "Software\\Wine\\DirectSound", list);
    if (list.count()>0){
        if (!list.at(0).isEmpty())
            sboxFakeSound_shadow->setValue(list.at(0).toInt());
    }

    QDir wineDriveDir;
    wineDriveDir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot  );

    prefixPath.append("/dosdevices/");
    if (!wineDriveDir.cd(prefixPath)){
        qDebug()<<"Cannot cd to prefix directory: "<<prefixPath;
    } else {

        QFileInfoList drivelist = wineDriveDir.entryInfoList();
        for (int i = 0; i < drivelist.size(); ++i) {
            QFileInfo fileInfo = drivelist.at(i);
            QString path = "";
            if (fileInfo.fileName().toUpper()=="C:"){
                path = "../drive_c";
            } else {
                path = fileInfo.symLinkTarget();
            }

            list.clear();
            list<<QString("\"%1\"").arg(fileInfo.fileName());
            list = reg.readKeys("system", "Software\\Wine\\Drives", list);

            item.reset(new DriveListWidgetItem(listWineDrives));
            item->setDrive(fileInfo.fileName().toUpper(), path, list.at(0));
            listWineDrives->addItem(item.release());
        }
    }

    prefixPath.append("c:/users/");
    prefixPath.append(getenv("USER"));

    if (!wineDriveDir.cd(prefixPath)){
        qDebug()<<"Cannot cd to prefix directory: "<<prefixPath;
    } else {
        QFileInfo fileinfo(QString("%1/%2").arg(prefixPath).arg(desktopFolder));
        if (fileinfo.isSymLink()){
            txtWineDesktop->setText(fileinfo.symLinkTarget());
        } else {
            //txtWineDesktop->setText(fileinfo.filePath());
            txtWineDesktop->setText("");
        }

        fileinfo.setFile(QString("%1/%2").arg(prefixPath).arg(this->desktopDocuments));
        if (fileinfo.isSymLink()){
            txtWineDesktopDoc->setText(fileinfo.symLinkTarget());
        } else {
            //txtWineDesktopDoc->setText(fileinfo.filePath());
            txtWineDesktopDoc->setText("");
        }

        fileinfo.setFile(QString("%1/%2").arg(prefixPath).arg(this->desktopMusic));
        if (fileinfo.isSymLink()){
            txtWineDesktopMus->setText(fileinfo.symLinkTarget());
        } else {
            //txtWineDesktopMus->setText(fileinfo.filePath());
            txtWineDesktopMus->setText("");
        }

        fileinfo.setFile(QString("%1/%2").arg(prefixPath).arg(this->desktopPictures));
        if (fileinfo.isSymLink()){
            txtWineDesktopPic->setText(fileinfo.symLinkTarget());
        } else {
            //txtWineDesktopPic->setText(fileinfo.filePath());
            txtWineDesktopPic->setText("");
        }

        fileinfo.setFile(QString("%1/%2").arg(prefixPath).arg(this->desktopVideos));
        if (fileinfo.isSymLink()){
            txtWineDesktopVid->setText(fileinfo.symLinkTarget());
        } else {
            //txtWineDesktopVid->setText(fileinfo.filePath());
            txtWineDesktopVid->setText("");
        }
    }
}

void FakeDriveSettings::loadDefaultSettings(){
    this->setEnabled(false);
    QApplication::setOverrideCursor(Qt::BusyCursor);

    QString prefixPath = db_prefix.getPath(this->prefixName);

    QDir dir;
    ExecObject execObj;
    execObj.cmdargs = "-u -i";
    execObj.execcmd = "wineboot";

    if (!CoreLib->runWineBinary(execObj, prefixName, false)){
        QApplication::restoreOverrideCursor();
        reject();
        return;
    }

    txtOwner->setText(getenv("USER"));

    std::auto_ptr<DriveListWidgetItem> item;
    item.reset(new DriveListWidgetItem(listWineDrives));
    item->setDrive("C:", "../drive_c", "auto");
    listWineDrives->addItem(item.release());

    QString mount_point = db_prefix.getMountPoint(prefixName);
    QChar mount_point_windrive = db_prefix.getMountPointWindrive(prefixName);

    if ((!mount_point.isEmpty()) and (!mount_point_windrive.isNull())){
        item.reset(new DriveListWidgetItem(listWineDrives));
        item->setDrive(QString("%1:").arg(mount_point_windrive), mount_point, "cdrom");
        listWineDrives->addItem(item.release());
    }

    item.reset(new DriveListWidgetItem(listWineDrives));
    item->setDrive("Z:", "/", "auto");
    listWineDrives->addItem(item.release());

    txtWineDesktop->setText(QString("%1/desktop-integration/Desktop").arg(prefixPath));
    txtWineDesktopDoc->setText(QString("%1/desktop-integration/").arg(prefixPath));
    txtWineDesktopMus->setText(QString("%1/desktop-integration/").arg(prefixPath));
    txtWineDesktopPic->setText(QString("%1/desktop-integration/").arg(prefixPath));
    txtWineDesktopVid->setText(QString("%1/desktop-integration/").arg(prefixPath));

        if (!dir.exists(txtWineDesktopDoc->text())){
            if (!dir.mkdir(txtWineDesktopDoc->text())){
                QMessageBox::warning(this, tr("Error"), tr("Can't create dir: %1").arg(prefixPath));
                QApplication::restoreOverrideCursor();
                reject();
            }
        }

        if (!dir.exists(txtWineDesktop->text())){
            if (!dir.mkdir(txtWineDesktop->text())){
                QMessageBox::warning(this, tr("Error"), tr("Can't create dir: %1").arg(prefixPath));
                QApplication::restoreOverrideCursor();
                reject();
            }
        }

        // We need to wait for wine while it writes .reg files....
        QTimer::singleShot(1000, this, SLOT(waitForWine()));
}

bool FakeDriveSettings::eventFilter(QObject *obj, QEvent *event){
    /*
        User select folder dialog function
    */

    if (obj->objectName()== "FakeDriveSettings")
        return FALSE;

    if (event->type() == QEvent::MouseButtonRelease) {
        QString file;

#if QT_VERSION >= 0x040500
        QFileDialog::Options options;

        if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
                options = QFileDialog::DontUseNativeDialog | QFileDialog::DontResolveSymlinks;

        if (obj->objectName().right(3)=="Bin"){
            file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),   "All files (*)", 0, options);
        } else {
            file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),  options);
        }
#else
        if (obj->objectName().right(3)=="Bin"){
            file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),   "All files (*)");
        } else {
            file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath());
        }
#endif

        if (!file.isEmpty()){
            QString a;
            a.append("txt");
            a.append(obj->objectName().right(obj->objectName().length()-6));

            std::auto_ptr<QLineEdit> lineEdit (findChild<QLineEdit *>(a));
            if (lineEdit.get()){
                lineEdit->setText(file);
            } else {
                qDebug("Error");
            }
            lineEdit.release();

            if (obj==cmdGetWineDesktop){
                txtWineDesktopDoc->setText(file);
                txtWineDesktopPic->setText(file);
                txtWineDesktopMus->setText(file);
                txtWineDesktopVid->setText(file);
            }
        }
    }
    return FALSE;
}
