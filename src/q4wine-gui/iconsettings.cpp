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

#include "iconsettings.h"

IconSettings::IconSettings(QString prefix_name, QString dir_name, QString icon_name, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
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
    libq4wine.setFileName(_CORELIB_PATH_);;
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->prefix_name = prefix_name;
    this->dir_name = dir_name;
    this->icon_name = icon_name;
    this->prefix_path = db_prefix.getByName(this->prefix_name).value("path");
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

    QString cd_mount = db_prefix.getMountPoint(this->prefix_name);

    if (!cd_mount.isEmpty())
      if (QDir().exists(cd_mount))
          prefix_urls << QUrl::fromLocalFile(cd_mount);

    this->loadThemeIcons();


    QString res;
    switch (icon_name.isEmpty()){
        case TRUE:
            lblCaption->setText(tr("Adding new icon"));
            setWindowTitle(tr("Adding new icon"));

            res = CoreLib->getSetting("advanced", "defaultDesktopSize", false, "").toString();
            if (res.isEmpty()){
                cboxDesktopSize->setCurrentIndex(0);
            } else {
                cboxDesktopSize->setCurrentIndex(cboxDesktopSize->findText(res));
            }
        break;
        case FALSE:
            lblCaption->setText(tr("Icon settings"));
            setWindowTitle(tr("Icon settings"));
            getIconReccord();
        break;
    }

    connect(twbGeneral, SIGNAL(currentChanged(int)), this, SLOT(ResizeContent(int)));
    connect(cmdAdd, SIGNAL(clicked()), this, SLOT(cmdAdd_Click()));
    connect(cmdGetProgram, SIGNAL(clicked()), this, SLOT(cmdGetProgram_Click()));
    connect(cmdGetWorkDir, SIGNAL(clicked()), this, SLOT(cmdGetWorkDir_Click()));
    connect(cmdGetIcon, SIGNAL(clicked()), this, SLOT(cmdGetIcon_Click()));
    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));

    connect(cbUseConsole, SIGNAL(stateChanged(int)), this, SLOT(cbUseConsole_stateChanged(int)));

    twDlls->installEventFilter(this);

    cboxDlls->addItems(CoreLib->getWineDlls(db_prefix.getByName(prefix_name).value("libs")));
    cboxDlls->setMaxVisibleItems (10);

    cmdOk->setFocus(Qt::ActiveWindowFocusReason);

    return;
}

void IconSettings::loadThemeIcons(){
    lblLogo->setPixmap(CoreLib->loadPixmap("data/exec.png"));
    cmdGetProgram->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWorkDir->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetIcon->setIcon(CoreLib->loadIcon("data/exec_wine.png"));

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
        if (QFile(iconPath).exists()){
            cmdGetIcon->setIcon (QIcon(iconPath));
        } else {
            if (iconPath=="console"){
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/wineconsole.png"));
            } else if (iconPath=="regedit"){
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/regedit.png"));
            } else if (iconPath=="wordpad"){
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/notepad.png"));
            } else if (iconPath=="winecfg"){
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/winecfg.png"));
            } else if (iconPath=="uninstaller"){
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/uninstaller.png"));
            } else if (iconPath=="eject"){
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/eject.png"));
            } else if (iconPath=="explorer"){
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/explorer.png"));
            } else {
                cmdGetIcon->setIcon(CoreLib->loadIcon("data/exec_wine.png"));
                iconPath="";
            }
        }
    }

    txtDesc->setText(iconRec.value("desc"));
    txtDisplay->setText(iconRec.value("display"));
    txtWinedebug->setText(iconRec.value("winedebug"));
    txtWorkDir->setText(iconRec.value("wrkdir"));
    txtEnvLang->setText(iconRec.value("lang"));

    if (iconRec.value("desktop").isEmpty()){
        cboxDesktopSize->setCurrentIndex(0);
    } else {
        cboxDesktopSize->setCurrentIndex(cboxDesktopSize->findText(iconRec.value("desktop")));
    }

    spinNice->setValue(iconRec.value("nice").toInt());

    if (iconRec.value("useconsole")=="1"){
        cbUseConsole->setCheckState(Qt::Checked);
        txtWinedebug->setEnabled(TRUE);
    } else {
        cbUseConsole->setCheckState(Qt::Unchecked);
        txtWinedebug->setEnabled(FALSE);
    }

    QStringList override = iconRec.value("override").split(";");
    QString overrideorder="";

    for (int i=0; i<override.count()-1; i++){
        QStringList list2 = override.at(i).split("=");
            twDlls->insertRow (0);
            std::auto_ptr<QTableWidgetItem> newItem (new QTableWidgetItem(list2.at(0)));
            newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            twDlls->setItem(0, 0, newItem.release());

            if (list2.at(1)=="n")
                overrideorder = tr("Native");
            if (list2.at(1)=="b")
                overrideorder = tr("Buildin");
            if (list2.at(1)=="n,b")
                overrideorder = tr("Native, Buildin");
            if (list2.at(1)=="b,n")
                overrideorder = tr("Buildin, Native");

            newItem.reset(new QTableWidgetItem(overrideorder));
            newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            twDlls->setItem(0, 1, newItem.release());
    }

    return;
}

bool IconSettings::eventFilter( QObject *object, QEvent *event )
{
   //  firstly, check whether the object is the QTableWidget and if it's a mouse press event
    if (object == twDlls)
        if (event->type() == QEvent::KeyPress)
        {   // if yes, we need to cast the event
        std::auto_ptr<QKeyEvent> keyEvent (static_cast<QKeyEvent*>(event));
        if (keyEvent->key()==Qt::Key_Delete)
            twDlls->removeRow(twDlls->currentRow());

        keyEvent.release();
        return true;
        }

    return QWidget::eventFilter(object, event);
}

void IconSettings::ResizeContent(int TabIndex){

    switch (TabIndex){
        case 1:
            twDlls->resizeRowsToContents();
            twDlls->resizeColumnsToContents();
            twDlls->horizontalHeader()->setStretchLastSection(TRUE);
        break;
    }

    return;
}

void IconSettings::resizeEvent (QResizeEvent){
    ResizeContent(2);
    return;
}

/*************************************************************\
|						CheckBox Events								 |
\*************************************************************/

void IconSettings::cbUseConsole_stateChanged(int){
    switch(cbUseConsole->checkState()){
        case Qt::PartiallyChecked:
            txtWinedebug->setEnabled(TRUE);
        break;
        case Qt::Checked:
            txtWinedebug->setEnabled(TRUE);
        break;
        case Qt::Unchecked:
            txtWinedebug->setEnabled(FALSE);
        break;
    }

    return;
}

/*************************************************************\
|						BUTTONS EVENTS								 |
\*************************************************************/

void IconSettings::cmdGetWorkDir_Click(){

    QString fileName, searchPath=this->prefix_path;

    if ((!txtProgramPath->text().isEmpty()) and (QDir().exists(txtProgramPath->text()))){
        searchPath=txtProgramPath->text().left(txtProgramPath->text().length() - txtProgramPath->text().split("/").last().length());;
    }

    if (!QDir(searchPath).exists()){
        if (QDir(this->prefix_path).exists()){
           searchPath=this->prefix_path;
        } else {
           searchPath=QDir::homePath();
        }
     }

    QFileDialog dialog(this);
      dialog.setFilter(QDir::Dirs | QDir::Hidden);

      dialog.setFileMode(QFileDialog::Directory);
      dialog.setWindowTitle(tr("Open Directory"));
      dialog.setDirectory(searchPath);

#if QT_VERSION >= 0x040500
      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

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
        std::auto_ptr<QTableWidgetItem> newItem (new QTableWidgetItem(cboxDlls->currentText()));
        newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        twDlls->setItem(0, 0, newItem.release());
        newItem.reset(new QTableWidgetItem(cboxOveride->currentText()));
        newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        twDlls->setItem(0, 1, newItem.release());
    }

    twDlls->resizeRowsToContents();
    twDlls->resizeColumnsToContents();
    twDlls->horizontalHeader()->setStretchLastSection(TRUE);
    return;
}

void IconSettings::cmdGetProgram_Click(){
    QString fileName, searchPath = this->prefix_path;

    if (!txtProgramPath->text().isEmpty()){
        if (!txtWorkDir->text().isEmpty()){
        searchPath=txtWorkDir->text();
        } else {
        searchPath=txtProgramPath->text().left(txtProgramPath->text().length() - txtProgramPath->text().split("/").last().length());;
        }
    }

    if ((!QDir(searchPath).exists()) or (searchPath.isEmpty())){
        if (QDir(dir_name).exists()){
           searchPath=this->prefix_path;
        } else {
           searchPath=QDir::homePath();
        }
     }

    QFileDialog dialog(this);
      dialog.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden );
      dialog.setWindowTitle(tr("Open Exe file"));
      dialog.setDirectory(searchPath);
      dialog.setFileMode(QFileDialog::ExistingFile);
      dialog.setNameFilter(tr("Exe files (*.exe)"));

#if QT_VERSION >= 0x040500
      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

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
        cmdGetIcon->setIcon(CoreLib->loadIcon("data/exec_wine.png"));
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

#if QT_VERSION >= 0x040500
      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

      if ((!iconPath.isEmpty()) and (QFile(iconPath).exists())){
          QStringList list = iconPath.split("/");
          searchPath = iconPath.left(iconPath.length() - list.last().length());
      }
      dialog.setDirectory(searchPath);

        #ifndef WITH_ICOUTILS
        dialog.setNameFilter(tr("Image files (*.png *.jpg *.gif *.bmp *.xpm)"));
        #else
        dialog.setNameFilter(tr("Image and Win32 binary files (*.png *.jpg *.gif *.bmp *.xpm *.exe *.dll);;Image files (*.png *.jpg *.gif *.bmp *.xpm);;Win32 Executable (*.exe);;Win32 Shared libraies (*.dll);;Win32 Executable and Shared libraies (*.exe *.dll)"));
        #endif
      //dialog.setSidebarUrls(add_prefix_urls);

     if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    if(!fileName.isEmpty()){
        if ((fileName.toLower().right(3)!="exe") && (fileName.toLower().right(3)!="dll")){
            cmdGetIcon->setIcon (QIcon(fileName));
        } else {

            QStringList args;
            args << "-x";
            args << "-t" << "14";

            QString tmpDir="";
            QStringList list1 = fileName.split("/");

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

                QDir ico_dir(tmpDir);
                // Updating file index
                list = ico_dir.entryInfoList();

                //Creating file list for converting
                for (int i = 0; i < list.size(); ++i) {
                    QFileInfo fileInfo = list.at(i);
                    qDebug() << fileInfo.fileName();
                    if (fileInfo.fileName().right(3)=="ico")
                        args << fileInfo.filePath();
                }

                args << "-o" << QString("%1/").arg(tmpDir);

                //Converting ico files to png

                //Look here, this function checks is some icons found, or not. 5 -- is default number of arguments,
                //if more -- then we have some ico file to convert
                if (args.size()>=4){
                    Process exportProcess(args, CoreLib->getSetting("icotool", "icotool").toString(), QDir::homePath(), tr("Convering icon from binary file.<br>This can take a while..."), tr("Converting icon"), FALSE);
                    if (exportProcess.exec()==QDialog::Accepted){
                        IconsView iconsView(tmpDir);
                        if (iconsView.exec()==QDialog::Accepted){
                            fileName=iconsView.selectedFile;
                            cmdGetIcon->setIcon (QIcon(fileName));
                        } else {
                            fileName.clear();
                        }
                    }
                } else {
                    IconsView iconsView(tmpDir);
                    if (iconsView.exec()==QDialog::Accepted){
                        fileName=iconsView.selectedFile;
                        cmdGetIcon->setIcon (QIcon(fileName));
                    } else {
                        fileName.clear();
                    }
                }
            } else {
                qDebug()<<"wrestool testing Rejected";
                fileName.clear();
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
      QMessageBox::warning(this, tr("Error"), tr("Selected working directory not exists."));
      return;
  }

  if (txtName->text().isEmpty()){
      QMessageBox::warning(this, tr("Error"), tr("No program name specified."));
      return;
  }

  if (icon_name!=txtName->text()){
      if (this->dir_name.isEmpty()){
          if (db_icon.isExistsByName(this->prefix_name, txtName->text())){
              QMessageBox::warning(this, tr("Error"), tr("Sorry, but icon named %1 already exists.").arg(txtName->text()));
              return;
          }
      } else {
          if (db_icon.isExistsByName(this->prefix_name, this->dir_name, txtName->text())){
              QMessageBox::warning(this, tr("Error"), tr("Sorry, but icon named %1 already exists.").arg(txtName->text()));
              return;
          }
      }
  }

    QString override="";
    for (int i=1; i<=twDlls->rowCount(); i++){
        override.append(QString("%1=").arg(twDlls->item(i-1, 0)->text()));
        if (twDlls->item(i-1, 1)->text()==tr("Native"))
            override.append("n;");
        if (twDlls->item(i-1, 1)->text()==tr("Disabled"))
            override.append("'';");
        if (twDlls->item(i-1, 1)->text()==tr("Buildin"))
            override.append("b;");
        if (twDlls->item(i-1, 1)->text()==tr("Native, Buildin"))
            override.append("n,b;");
        if (twDlls->item(i-1, 1)->text()==tr("Buildin, Native"))
            override.append("b,n;");
    }

    QString useconsole="";
    if (cbUseConsole->checkState()==Qt::Checked){
        useconsole="1";
    } else {
        useconsole="0";
    }

    QString desktopSize=cboxDesktopSize->currentText();

    if (desktopSize==tr("No virtual desktop"))
        desktopSize="";

    switch (this->icon_name.isEmpty()){
        case TRUE:
            db_icon.addIcon(txtCmdArgs->text(), txtProgramPath->text(), iconPath, txtDesc->text(), this->prefix_name, this->dir_name, txtName->text(), override, txtWinedebug->text(), useconsole, txtDisplay->text(), txtWorkDir->text(), desktopSize, spinNice->value(), txtEnvLang->text());
        break;
        case FALSE:
            db_icon.updateIcon(txtCmdArgs->text(), txtProgramPath->text(), iconPath, txtDesc->text(), this->prefix_name, this->dir_name, txtName->text(), icon_name, override, txtWinedebug->text(), useconsole, txtDisplay->text(), txtWorkDir->text(), desktopSize, spinNice->value(), txtEnvLang->text());
        break;
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
    default:
        rawurl = "08-icon-dialog.html";
    break;
    }

    CoreLib->openHelpUrl(rawurl);
}

