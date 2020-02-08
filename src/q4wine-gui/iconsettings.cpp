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

#include "iconsettings.h"

IconSettings::IconSettings(QString prefix_name, QString dir_name, QString icon_name, QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    /*
     ** This functions is constructor for IconSettings dialog
     *
     * Args:
     * @newIcon -- This define which type of dialog we'l use "New Icon" or "Edit Icon"
     * @p_id    -- PrefixId
     * @p_id    -- PrefixId
     */
    setupUi(this);

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
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

    this->prefix_name = prefix_name;
    QHash<QString,QString> result = db_prefix.getByName(this->prefix_name);

    this->dir_name = dir_name;
    this->icon_name = icon_name;
    this->prefix_path = result.value("path");
    if (this->prefix_path.isEmpty()){
        this->prefix_path = QDir::homePath();
        this->prefix_path.append("/.wine/drive_c/");
    } else {
        this->prefix_path.append("/drive_c/");
    }


    // Creating side bar urls
    prefix_urls << QUrl::fromLocalFile(QDir::rootPath());
    prefix_urls << QUrl::fromLocalFile(QDir::homePath());

    if (QDir(this->prefix_path).exists())
       prefix_urls << QUrl::fromLocalFile(this->prefix_path);

    if ((!txtWorkDir->text().isEmpty()) and (QDir(txtWorkDir->text()).exists()))
       prefix_urls << QUrl::fromLocalFile(txtWorkDir->text());

    QString cd_mount = result.value("mount");

    if (!cd_mount.isEmpty())
      if (QDir().exists(cd_mount))
          prefix_urls << QUrl::fromLocalFile(cd_mount);

    setWindowIcon(CoreLib->loadIcon(CoreLib->getSetting("app", "icon", false, "q4wine").toString()));
    this->loadThemeIcons();


    QString res;
    if (icon_name.isEmpty()){
            setWindowTitle(tr("Adding new icon"));
            res = CoreLib->getSetting("advanced", "defaultDesktopSize", false, "").toString();
            if (res.isEmpty()){
                cboxDesktopSize->setCurrentIndex(0);
            } else {
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
            }
    } else {
            setWindowTitle(tr("Icon settings"));
            getIconReccord();
    }

    connect(cmdAdd, SIGNAL(clicked()), this, SLOT(cmdAdd_Click()));
    connect(cmdGetProgram, SIGNAL(clicked()), this, SLOT(cmdGetProgram_Click()));
    connect(cmdGetWorkDir, SIGNAL(clicked()), this, SLOT(cmdGetWorkDir_Click()));
    connect(cmdGetIcon, SIGNAL(clicked()), this, SLOT(cmdGetIcon_Click()));
    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
    connect(cmdGetPreRun, SIGNAL(clicked()), this, SLOT(cmdGetPreRun_Click()));
    connect(cmdGetPostRun, SIGNAL(clicked()), this, SLOT(cmdGetPostRun_Click()));

    connect(cbUseConsole, SIGNAL(stateChanged(int)), this, SLOT(cbUseConsole_stateChanged(int)));

    twDlls->installEventFilter(this);

    QString libs_path = result.value("libs");

    if (libs_path.isEmpty()){
        Version vers;
        vers.id_ = result.value("version_id");
        if (vers.load()){
            if (result.value("arch") == "win32"){
                libs_path = vers.wine_dllpath32_;
            } else if (result.value("arch") == "win64"){
                libs_path = vers.wine_dllpath64_;
            } else {
                if (vers.wine_dllpath64_.isEmpty()){
                    libs_path = vers.wine_dllpath32_;
                } else {
                    libs_path = vers.wine_dllpath64_;
                }
            }
        }
    }

    twDlls->horizontalHeader()->setStretchLastSection(true);
    twDlls->setColumnWidth(0, 400);

    cboxDlls->addItems(CoreLib->getWineDlls(libs_path));
    cboxDlls->setMaxVisibleItems (10);

    cmdOk->setFocus(Qt::ActiveWindowFocusReason);

    return;
}

void IconSettings::loadThemeIcons(){
    cmdGetProgram->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWorkDir->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetIcon->setIcon(CoreLib->loadIcon("application-x-ms-dos-executable"));
    cmdGetPreRun->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetPostRun->setIcon(CoreLib->loadIcon("document-open"));
    return;
}

void IconSettings::getIconReccord(){
    QHash<QString, QString> iconRec;

    iconRec = db_icon.getByName(this->prefix_name, this->dir_name, this->icon_name);

    txtName->setText(iconRec.value("name"));
    txtCmdArgs->setText(iconRec.value("cmdargs"));
    txtProgramPath->setText(iconRec.value("exec"));

    iconPath = iconRec.value("icon_path");

    if (!iconPath.isEmpty()){
        QIcon ico = CoreLib->loadAppIcon(iconPath);
        cmdGetIcon->setIcon(ico);
    }

    txtDesc->setText(iconRec.value("desc"));
    txtDisplay->setText(iconRec.value("display"));
    txtWinedebug->setText(iconRec.value("winedebug"));
    txtWorkDir->setText(iconRec.value("wrkdir"));
    txtEnvLang->setText(iconRec.value("lang"));

    if (iconRec.value("desktop").isEmpty()){
        cboxDesktopSize->setCurrentIndex(0);
    } else {
        QString res = iconRec.value("desktop");
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
    }

    spinNice->setValue(iconRec.value("nice").toInt());

    if (iconRec.value("useconsole")=="1"){
        cbUseConsole->setCheckState(Qt::Checked);
        txtWinedebug->setEnabled(true);
    } else {
        cbUseConsole->setCheckState(Qt::Unchecked);
        txtWinedebug->setEnabled(false);
    }

    QStringList override = iconRec.value("override").split(";");
    QString overrideorder="";

    for (int i=0; i<override.count()-1; i++){
        QStringList list2 = override.at(i).split("=");
            twDlls->insertRow (0);
            std::unique_ptr<QTableWidgetItem> newItem (new QTableWidgetItem(list2.at(0)));
            newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
            twDlls->setItem(0, 0, newItem.release());

            if (list2.at(1)=="n"){
                overrideorder = tr("Native");
            } else if (list2.at(1)=="b"){
                overrideorder = tr("Built-in");
            } else if (list2.at(1)=="n,b"){
                overrideorder = tr("Native, Built-in");
            } else if (list2.at(1)=="b,n"){
                overrideorder = tr("Built-in, Native");
            } else {
                overrideorder = tr("Disabled");
            }

            std::unique_ptr<QComboBox> overidebox (new QComboBox(twDlls));
            overidebox->addItem(tr("Native"));
            overidebox->addItem(tr("Built-in"));
            overidebox->addItem(tr("Native, Built-in"));
            overidebox->addItem(tr("Built-in, Native"));
            overidebox->addItem(tr("Disabled"));
            overidebox->setCurrentIndex(overidebox->findText(overrideorder));

            twDlls->setCellWidget( 0, 1, overidebox.release());
    }

    txtPreRun->setText(iconRec.value("prerun"));
    txtPostRun->setText(iconRec.value("postrun"));

    return;
}

void IconSettings::updateSidebarUrls( QFileDialog *dialog )
{
   if ((!txtWorkDir->text().isEmpty()) and (QDir(txtWorkDir->text()).exists())) {
      prefix_urls << QUrl::fromLocalFile(txtWorkDir->text());
   }
   dialog->setSidebarUrls(prefix_urls);
}

bool IconSettings::eventFilter( QObject *object, QEvent *event )
{
   //  firstly, check whether the object is the QTableWidget and if it's a mouse press event
    if (object == twDlls)
        if (event->type() == QEvent::KeyPress)
        {   // if yes, we need to cast the event
        std::unique_ptr<QKeyEvent> keyEvent (static_cast<QKeyEvent*>(event));
        if (keyEvent->key()==Qt::Key_Delete)
            twDlls->removeRow(twDlls->currentRow());

        keyEvent.release();
        return true;
        }

    return QWidget::eventFilter(object, event);
}

/*************************************************************\
|						CheckBox Events								 |
\*************************************************************/

void IconSettings::cbUseConsole_stateChanged(int){
    switch(cbUseConsole->checkState()){
        case Qt::PartiallyChecked:
            txtWinedebug->setEnabled(true);
        break;
        case Qt::Checked:
            txtWinedebug->setEnabled(true);
        break;
        case Qt::Unchecked:
            txtWinedebug->setEnabled(false);
        break;
    }

    return;
}

/*************************************************************\
|						BUTTONS EVENTS								 |
\*************************************************************/

void IconSettings::cmdGetWorkDir_Click(){

    QString fileName, searchPath;

    if (txtWorkDir->text().isEmpty()){
        searchPath = this->prefix_path;
    } else {
        if (QDir(txtWorkDir->text()).exists()) {
            searchPath = txtWorkDir->text();
        } else {
            searchPath = this->prefix_path;
        }
    }

    QFileDialog dialog(this);
      dialog.setFilter(QDir::Dirs | QDir::Hidden);

      dialog.setFileMode(QFileDialog::Directory);
      dialog.setWindowTitle(tr("Open Directory"));
      dialog.setDirectory(searchPath);

      this->updateSidebarUrls(&dialog);

      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }

    if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    if(!fileName.isEmpty()){
        txtWorkDir->setText(fileName);
    }
    return;
}

void IconSettings::cmdAdd_Click(){
    if (!cboxDlls->currentText().isEmpty()){
        twDlls->insertRow (0);
        std::unique_ptr<QTableWidgetItem> newItem (new QTableWidgetItem(cboxDlls->currentText()));
        newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
        twDlls->setItem(0, 0, newItem.release());

        std::unique_ptr<QComboBox> overidebox (new QComboBox(twDlls));
        overidebox->addItem(tr("Native"));
        overidebox->addItem(tr("Built-in"));
        overidebox->addItem(tr("Native, Built-in"));
        overidebox->addItem(tr("Built-in, Native"));
        overidebox->addItem(tr("Disabled"));
        overidebox->setCurrentIndex(overidebox->findText(cboxOveride->currentText()));

        twDlls->setCellWidget(0, 1, overidebox.release());
    }
    return;
}

void IconSettings::cmdGetProgram_Click(){
    QString fileName, searchPath;

    if (txtProgramPath->text().isEmpty()){
        searchPath = this->prefix_path;
    } else {
        if (QFile(txtProgramPath->text()).exists()){
            searchPath = QFileInfo(txtProgramPath->text()).absolutePath();
        } else {
            searchPath = this->prefix_path;
        }
    }

    QFileDialog dialog(this);
      dialog.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden );
      dialog.setWindowTitle(tr("Open Exe file"));
      dialog.setDirectory(searchPath);
      dialog.setFileMode(QFileDialog::ExistingFile);
      dialog.setNameFilter(tr("Exe, MSI, BAT files (*.exe *.EXE *.msi *.MSI *.bat *.BAT);;Exe files (*.exe *.EXE);;MSI files (*.msi *.MSI);;BAT files (*.bat *.BAT);;All files (*)"));

      this->updateSidebarUrls(&dialog);

      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }

     if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    if(!fileName.isEmpty()){
            QStringList list1 = fileName.split("/");
            txtName->setText(list1.last().left(list1.last().length() - list1.last().split(".").last().length() - 1));
            txtProgramPath->setText(fileName);

            QString wrkDir;
            wrkDir = fileName.left(fileName.length() - list1.last().length());
            txtWorkDir->setText(wrkDir);

            getProgramIcon(txtName->text().toLower());
    }

    return;
}

void IconSettings::getProgramIcon(QString name){
    QString local_path = QDir::homePath();
    local_path.append("/.local/share/icons/");

    QDir dir(local_path, QString("*_%1.*").arg(name));
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    QFileInfoList list = dir.entryInfoList();
    if (list.size()>0){
        cmdGetIcon->setIcon (QIcon(list.at(0).filePath()));
        this->iconPath=list.at(0).filePath();
    } else {
        cmdGetIcon->setIcon(CoreLib->loadIcon("application-x-ms-dos-executable"));
    }
    return;
}

void IconSettings::cmdGetIcon_Click(){
    QString fileName, searchPath=this->prefix_path;

    if ((!txtWorkDir->text().isEmpty()) and (QDir(txtWorkDir->text()).exists())){
        searchPath = txtWorkDir->text();
    } else {
        if (QDir(this->prefix_path).exists()){
           searchPath=this->prefix_path;
        } else {
           searchPath=QDir::homePath();
        }
    }

    QFileDialog dialog(this);
      dialog.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden);
      dialog.setFileMode(QFileDialog::ExistingFile);
      dialog.setWindowTitle(tr("Open image file"));

      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }

      if ((!iconPath.isEmpty()) and (QFile(iconPath).exists())){
          QStringList list = iconPath.split("/");
          searchPath = iconPath.left(iconPath.length() - list.last().length());
      }
      dialog.setDirectory(searchPath);

      this->updateSidebarUrls(&dialog);

        #ifndef WITH_ICOUTILS
        dialog.setNameFilter(tr("Image files (*.png *.PNG *.jpg *.JPG *.gif *.GIF *.bmp *.BMP *.xpm *.XPM *.svg *.SVG *.svgz *.SVGZ)"));
        #else
        dialog.setNameFilter(tr("Image and Win32 binary files (*.png *.PNG *.jpg *.JPG *.gif *.GIF *.bmp *.BMP *.xpm *.XPM *.exe *.EXE *.dll *.DLL);;Image files (*.png *.PNG *.jpg *.JPG *.gif *.GIF *.bmp *.BMP *.xpm *.XPM *.svg *.SVG *.svgz *.SVGZ);;Win32 Executable (*.exe *.EXE);;Win32 Shared libraries (*.dll *.DLL);;Win32 Executable and Shared libraries (*.exe *.EXE *.dll *.DLL);;All files (*)"));
        #endif

     if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    if(!fileName.isEmpty()){
        if ((fileName.toLower().right(3)!="exe") && (fileName.toLower().right(3)!="dll")){
            cmdGetIcon->setIcon (QIcon(fileName));
        } else {

            QStringList args;
            args << "-x";
            args << "-t" << "14";

            QString tmpDir = QDir::tempPath();
            tmpDir.append("/");
            tmpDir.append(APP_SHORT_NAME);
            tmpDir.append("-");
            tmpDir.append(fileName.split("/").last());

            QDir tmp(tmpDir);
            QFileInfoList allFilesList = tmp.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

            if (tmp.exists(tmpDir)){
                for (int i = 0; i < allFilesList.size(); ++i) {
                    QFileInfo fileInfo = allFilesList.at(i);
                    if (!tmp.remove(fileInfo.filePath()))
                        qDebug()<<"[EE] - Cannot delete files at: "<<fileInfo.filePath();
                }
            } else {
                if (!tmp.mkdir(tmpDir)){
                    qDebug()<<"[EE] - Cannot create temp directory at: "<<tmpDir;
                }
            }

            args << "-o" << tmpDir;
            args << fileName;

            Process exportProcess(args, CoreLib->getSetting("icotool", "wrestool").toString(), QDir::homePath(), tr("Extracting icon from binary file.<br>This can take a while..."), tr("Extracting icon"), false);

            if (exportProcess.exec()==QDialog::Accepted){
            //icotool -x -o ./regedit.png --width=32 --height=32 ./regedit.exe_14_100_0.ico
                args.clear();
                args << "-x";

                // Updating file index
                QStringList nameFilters;
                nameFilters << "*.ico";

                QFileInfoList icoFilesList = tmp.entryInfoList(nameFilters, QDir::Files | QDir::Hidden | QDir::NoSymLinks);

                //Creating file list for converting
                for (int i = 0; i < icoFilesList.size(); ++i) {
                    QFileInfo fileInfo = icoFilesList.at(i);
                    qDebug() << fileInfo.fileName();
                    args << fileInfo.filePath();
                }

                args << "-o" << QString("%1/").arg(tmpDir);

                //Converting ico files to png

                //Look here, this function checks if some icons found, or not. 5 -- is default number of arguments,
                //if more -- then we have some ico file to convert
                if (args.size()>=4){
                    Process exportProcess(args, CoreLib->getSetting("icotool", "icotool").toString(), QDir::homePath(), tr("Extracting icon from binary file.<br>This can take a while..."), tr("Extracting icon"), false);
                    if (exportProcess.exec()==QDialog::Accepted){
                        IconsView iconsView(tmpDir);
                        if (iconsView.exec()==QDialog::Accepted){
                            fileName=iconsView.selectedFile;
                            cmdGetIcon->setIcon(QIcon(fileName));
                        } else {
                            fileName.clear();
                        }
                    }
                } else {
                    IconsView iconsView(tmpDir);
                    if (iconsView.exec()==QDialog::Accepted){
                        fileName=iconsView.selectedFile;
                        cmdGetIcon->setIcon(QIcon(fileName));
                    } else {
                        fileName.clear();
                    }
                }
            } else {
                qDebug()<<"wrestool testing Rejected";
                fileName.clear();
            }

            //Clearing temp files
            allFilesList = tmp.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

                //Creating file list for converting
            for (int i = 0; i < allFilesList.size(); ++i) {
                QFileInfo fileInfo = allFilesList.at(i);
                    if (!QFile::remove(fileInfo.filePath()))
                        qDebug()<<"[EE] - Cannot delete files at: "<<fileInfo.filePath();
            }

            if (!tmp.rmdir(tmpDir))
                qDebug()<<"[EE] - Cannot delete tmp dir: "<<tmpDir;
        }

        if (!fileName.isEmpty())
            iconPath=fileName;
    }

    return;
}

void IconSettings::cmdCancel_Click(){
    reject();
    return;
}

void IconSettings::cmdOk_Click(){

  if (txtProgramPath->text().isEmpty()){
      QMessageBox::warning(this, tr("Error"), tr("No program selected."));
      return;
  }

  if (!QDir(txtWorkDir->text()).exists()){
      QMessageBox::warning(this, tr("Error"), tr("Selected working directory does not exist."));
      return;
  }

  if (txtName->text().isEmpty()){
      QMessageBox::warning(this, tr("Error"), tr("No program name specified."));
      return;
  }

  if (icon_name!=txtName->text()){
      if (this->dir_name.isEmpty()){
          if (db_icon.isExistsByName(this->prefix_name, txtName->text())){
              QMessageBox::warning(this, tr("Error"), tr("Sorry, an icon named %1 already exists.").arg(txtName->text()));
              return;
          }
      } else {
          if (db_icon.isExistsByName(this->prefix_name, this->dir_name, txtName->text())){
              QMessageBox::warning(this, tr("Error"), tr("Sorry, an icon named %1 already exists.").arg(txtName->text()));
              return;
          }
      }
  }

    QString override="";
    for (int i=0; i<=twDlls->rowCount()-1; i++){
        override.append(QString("%1=").arg(twDlls->item(i, 0)->text()));

        QString method = twDlls->cellWidget(i, 1)->property("currentText").toString();

        if (method==tr("Native"))
            override.append("n;");
        if (method==tr("Disabled"))
            override.append(";");
        if (method==tr("Built-in"))
            override.append("b;");
        if (method==tr("Native, Built-in"))
            override.append("n,b;");
        if (method==tr("Built-in, Native"))
            override.append("b,n;");
    }

    QString useconsole="";
    if (cbUseConsole->checkState()==Qt::Checked){
        useconsole="1";
    } else {
        useconsole="0";
    }

    QString desktopSize=cboxDesktopSize->currentText();

    if (desktopSize==tr("No virtual desktop")) {
        desktopSize="";
    } else {
        QRegExp res_match("^\\d{3,}x\\d{3,}$");
        if (!res_match.exactMatch(desktopSize)){
            QMessageBox::warning(this, tr("Error"), tr("Invalid virtual desktop size."));
            return;
        }
    }

    if (this->icon_name.isEmpty()){
            db_icon.addIcon(txtCmdArgs->text(), txtProgramPath->text(), iconPath, txtDesc->text(), this->prefix_name, this->dir_name, txtName->text(), override, txtWinedebug->text(), useconsole, txtDisplay->text(), txtWorkDir->text(), desktopSize, spinNice->value(), txtEnvLang->text(), txtPreRun->text(), txtPostRun->text());
#ifndef _OS_DARWIN_
            if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool())
                CoreLib->createDesktopFile(this->prefix_name, this->dir_name, txtName->text(), true);
#endif
    } else {
            db_icon.updateIcon(txtCmdArgs->text(), txtProgramPath->text(), iconPath, txtDesc->text(), this->prefix_name, this->dir_name, txtName->text(), icon_name, override, txtWinedebug->text(), useconsole, txtDisplay->text(), txtWorkDir->text(), desktopSize, spinNice->value(), txtEnvLang->text(), txtPreRun->text(), txtPostRun->text());
#ifndef _OS_DARWIN_
            if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool()){
                CoreLib->deleteDesktopFile(this->prefix_name, this->dir_name, this->icon_name);
                CoreLib->createDesktopFile(this->prefix_name, this->dir_name, txtName->text(), true);
            }
#endif
    }

    accept();

    return;
}

void IconSettings::cmdHelp_Click(){
    QString rawurl;
    switch (twbGeneral->currentIndex()){
    case 0:
        rawurl = "08-icon-dialog.html#general";
    break;
    case 1:
        rawurl = "08-icon-dialog.html#override";
    break;
    case 2:
        rawurl = "08-icon-dialog.html#advanced";
    break;
    case 3:
        rawurl = "08-icon-dialog.html#scripts";
    break;
    default:
        rawurl = "08-icon-dialog.html";
    break;
    }

    CoreLib->openHelpUrl(rawurl);
}

void IconSettings::cmdGetPreRun_Click(){
    QString fileName;
    QString searchPath = "";
    if (txtPreRun->text().isEmpty()) {
        searchPath = QDir::homePath();
        searchPath.append("/.config/q4wine/scripts");
    } else {
        searchPath = QFileInfo(txtPreRun->text()).absolutePath();
    }
    QFileDialog dialog(this);
    dialog.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden );
    dialog.setWindowTitle(tr("Open Exe file"));
    dialog.setDirectory(searchPath);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Shell script (*.sh)"));

    if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
        dialog.setOptions(QFileDialog::DontUseNativeDialog);
    }

    if (dialog.exec()){
       fileName = dialog.selectedFiles().first();
       txtPreRun->setText(fileName);
    }
    return;
}

void IconSettings::cmdGetPostRun_Click(){
    QString fileName;
    QString searchPath = "";
    if (txtPostRun->text().isEmpty()) {
        searchPath = QDir::homePath();
        searchPath.append("/.config/q4wine/scripts");
    } else {
        searchPath = QFileInfo(txtPostRun->text()).absolutePath();
    }
    QFileDialog dialog(this);
    dialog.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden );
    dialog.setWindowTitle(tr("Open Exe file"));
    dialog.setDirectory(searchPath);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Shell script (*.sh)"));

    if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
        dialog.setOptions(QFileDialog::DontUseNativeDialog);
    }

    if (dialog.exec())
       fileName = dialog.selectedFiles().first();

    txtPostRun->setText(fileName);
    return;
}
