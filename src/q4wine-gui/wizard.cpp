/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                                 *
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

#include "wizard.h"

void Wizard::loadThemeIcons(int Scene){
    QPixmap pixmap;
    switch (Scene){
    case 1:
        // First startup
        lblPicture->setPixmap(CoreLib->loadPixmap("data/firstc.png"));

        cmdGetWineBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetWineServerBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetWineLoaderBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetWineDllPath->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetTarBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetMountBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetUmountBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetSudoBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetGuiSudoBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetNiceBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetReniceBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetShBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetConsoleBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetWrestoolBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        cmdGetIcotoolBin->setIcon(CoreLib->loadIcon("data/folder.png"));
        break;
    }
    return;
}

Wizard::Wizard(int WizardType, QString var1, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
    /*
        Note: var1 is optional data, used for different scene actions ;)
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

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    Scena=WizardType;
    Page=1;
    QString console_w;

    loadThemeIcons(Scena);

    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(reject ()));
    connect(cmdNext, SIGNAL(clicked()), this, SLOT(nextWizardPage()));
    connect(cmdBack, SIGNAL(clicked()), this, SLOT(previosWizardPage()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
    connect(comboProxyType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboProxyType_indexChanged(QString)));
    connect(comboMountProfiles, SIGNAL(currentIndexChanged(int)), this, SLOT(comboMountProfiles_currentIndexChanged(int)));

    switch (Scena){
 case 1:
        TotalPage=8;

        setWindowTitle(tr("First startup wizard"));
        lblCaption->setText(tr("<b>First startup wizard</b>"));
        lblStep->setText(tr("<b>Step %1 of %2</b>").arg(Page).arg(TotalPage));

        cmdGetWineBin->installEventFilter(this);
        cmdGetWineServerBin->installEventFilter(this);
        cmdGetWineLoaderBin->installEventFilter(this);
        cmdGetWineDllPath->installEventFilter(this);

        cmdGetTarBin->installEventFilter(this);
        cmdGetMountBin->installEventFilter(this);
        cmdGetUmountBin->installEventFilter(this);
        cmdGetSudoBin->installEventFilter(this);
        cmdGetGuiSudoBin->installEventFilter(this);
        cmdGetNiceBin->installEventFilter(this);
        cmdGetReniceBin->installEventFilter(this);
        cmdGetShBin->installEventFilter(this);

        cmdGetConsoleBin->installEventFilter(this);

        txtWineBin->setText(CoreLib->getWhichOut("wine"));
        txtWineServerBin->setText(CoreLib->getWhichOut("wineserver"));
        txtWineLoaderBin->setText(CoreLib->getWhichOut("wine"));

        QStringList libs_loc, libs_arch;
        libs_loc << "/usr/lib" << "/usr/local/lib" << "/local/usr/lib";
        libs_arch << "64" << "32" << "";

        foreach (QString loc, libs_loc){
            foreach (QString arch, libs_arch){
                QString libs_path = loc;
                libs_path.append(arch);
                QString libwine_path = libs_path;
                libwine_path.append("/libwine.so");
                if (QFile(libwine_path).exists()){
                    txtWineDllPath->setText(libs_path);
                    break;
                }
            }
        }

        txtTarBin->setText(CoreLib->getWhichOut("tar"));
        txtMountBin->setText(CoreLib->getWhichOut("mount"));
        txtUmountBin->setText(CoreLib->getWhichOut("umount"));
        txtSudoBin->setText(CoreLib->getWhichOut("sudo"));

        QStringList guisudo;
#ifdef _OS_DARWIN_
        guisudo << "osascript";
#else
        guisudo << "kdesudo" << "kdesu" << "gksudo" << "gksu" << "sudo";
#endif
        foreach (QString bin, guisudo){
            QString path = CoreLib->getWhichOut(bin, FALSE);
            if (!path.isEmpty()){
                txtGuiSudoBin->setText(path);
                break;
            }
        }

        txtNiceBin->setText(CoreLib->getWhichOut("nice"));
        txtReniceBin->setText(CoreLib->getWhichOut("renice"));
        txtShBin->setText(CoreLib->getWhichOut("sh"));

#ifdef _OS_DARWIN_
        txtConsoleBin->setText("/usr/X11/bin/xterm");
        txtConsoleArgs->setText("-e");
#else
        console_w = CoreLib->getWhichOut("konsole", FALSE);
        if (!console_w.isEmpty()){
            txtConsoleBin->setText(console_w);
            txtConsoleArgs->setText("--noclose -e");
        } else {
            console_w = CoreLib->getWhichOut("xterm", FALSE);
            if (!console_w.isEmpty()){
                txtConsoleBin->setText(console_w);
                txtConsoleArgs->setText("-e");
            }
        }
#endif

#ifdef WITH_ICOUTILS
        txtWrestoolBin->setText(CoreLib->getWhichOut("wrestool"));
        txtIcotoolBin->setText(CoreLib->getWhichOut("icotool"));
        cmdGetWrestoolBin->installEventFilter(this);
        cmdGetIcotoolBin->installEventFilter(this);
#else
        cmdGetWrestoolBin->setEnabled(false);
        cmdGetIcotoolBin->setEnabled(false);
        txtWrestoolBin->setEnabled(false);
        txtIcotoolBin->setEnabled(false);
#endif

#ifdef _OS_DARWIN_
        this->comboMountProfiles->setCurrentIndex(0);
        this->comboMountProfiles_currentIndexChanged(0);
#else
        if (CoreLib->getWhichOut("fuseiso", false).isEmpty()){
            this->comboMountProfiles->setCurrentIndex(0);
        } else {
            this->comboMountProfiles->setCurrentIndex(2);
        }
#endif

        break;
    }

    updateScena();

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

    this->installEventFilter(this);

    return;
}

void Wizard::comboProxyType_indexChanged(QString text){
    if (text==tr("No Proxy")){
        txtProxyHost->setEnabled(FALSE);
        txtProxyPort->setEnabled(FALSE);
        txtProxyUser->setEnabled(FALSE);
        txtProxyPass->setEnabled(FALSE);
    } else {
        txtProxyHost->setEnabled(TRUE);
        txtProxyPort->setEnabled(TRUE);
        txtProxyUser->setEnabled(TRUE);
        txtProxyPass->setEnabled(TRUE);
    }

    return;
}

void Wizard::changeBoxState(int state){
    /*
        Checkbox selecting event
    */

    /*
    switch(Scena){
        case 0:
            switch (Page){
                case 2:
                    if (state==Qt::Checked){
                        cbFakeVersion->setEnabled(TRUE);
                    } else {
                        cbFakeVersion->setEnabled(FALSE);
                    }
                break;
            }
        break;
    }
    */

    return;
}

bool Wizard::eventFilter(QObject *obj, QEvent *event){
    /*
        User select folder dialog function
    */

    if (obj->objectName()== "Wizard")
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

            if (obj==cmdGetWineBin){
                QString wrkDir;
                QStringList list1 = file.split("/");
                wrkDir = file.left(file.length() - list1.last().length());
                txtWineServerBin->setText(QString("%1wineserver").arg(wrkDir));
                txtWineLoaderBin->setText(QString("%1wine").arg(wrkDir));
            }
        }
    }
    return FALSE;
}

bool Wizard::checkEntry(QString fileName, QString info, bool isFile){
    /*
     *	This function check user entry
     */

    if (fileName.isEmpty()){
        switch (isFile){
  case FALSE:
            QMessageBox::warning(this, tr("Error"), tr("Sorry, specify %1 directory.").arg(info));
            break;
  case TRUE:
            QMessageBox::warning(this, tr("Error"), tr("Sorry, specify %1 binary.").arg(info));
            break;
        }
        return FALSE;
    } else {
        if (!QFile::exists(fileName)){
            switch (isFile){
   case FALSE:
                QMessageBox::warning(this, tr("Error"), tr("Sorry, specified %1 directory not exists.").arg(info));
                break;
   case TRUE:
                QMessageBox::warning(this, tr("Error"), tr("Sorry, specified %1 binary not exists.").arg(info));
                break;
            }
            return FALSE;
        }
    }

    return TRUE;
}

void Wizard::nextWizardPage(){
    /*
        Function for processing next\finish button click events
    */

    switch(Scena){
 case 1:
        switch (Page){
  case 3:
            if (!checkEntry(txtWineBin->text(), "wine"))
                return;
            if (!checkEntry(txtWineServerBin->text(), "wine server"))
                return;
            if (!checkEntry(txtWineLoaderBin->text(), "wine loader"))
                return;
            if (!checkEntry(txtWineDllPath->text(), "wine library", FALSE))
                return;
            break;
  case 4:
            if (!checkEntry(txtTarBin->text(), "tar"))
                return;
            if (!checkEntry(txtMountBin->text(), "mount"))
                return;
            if (!checkEntry(txtUmountBin->text(), "umount"))
                return;
            if (!checkEntry(txtSudoBin->text(), "sudo"))
                return;
            if (!checkEntry(txtGuiSudoBin->text(), "GUI sudo"))
                return;
            if (!checkEntry(txtUmountBin->text(), "nice"))
                return;
            if (!checkEntry(txtUmountBin->text(), "renice"))
                return;
            if (!checkEntry(txtUmountBin->text(), "sh"))
                return;
            break;
  case 5:
            if (!checkEntry(txtConsoleBin->text(), "console"))
                return;
#ifdef WITH_ICOUTILS
            if (!checkEntry(txtWrestoolBin->text(), "wrestool"))
                return;
            if (!checkEntry(txtIcotoolBin->text(), "icotool"))
                return;
#endif
            break;
  case 6:
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
            break;
                case 8:
            QSettings settings(APP_SHORT_NAME, "default");
            settings.setValue("configure", "yes");
            settings.beginGroup("wine");
            settings.setValue("WineBin", txtWineBin->text());
            settings.setValue("ServerBin", txtWineServerBin->text());
            settings.setValue("LoaderBin", txtWineLoaderBin->text());
            settings.setValue("WineLibs", txtWineDllPath->text());
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
            settings.beginGroup("icotool");
            settings.setValue("wrestool", txtWrestoolBin->text());
            settings.setValue("icotool", txtIcotoolBin->text());
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

            settings.beginGroup("quickmount");

            settings.setValue("type", this->comboMountProfiles->currentIndex());
            settings.setValue("mount_drive_string", txtMountString->text());
            settings.setValue("mount_image_string", txtMountImageString->text());
            settings.setValue("umount_string", txtUmountString->text());

            settings.endGroup();

            CoreLib->openHelpUrl("05-first-steps.html");

            CoreLib->createPrefixDBStructure("Default");

            accept();
            break;
        }
        break;
    }

    Page++;
    updateScena();
    return;
}


void Wizard::cmdHelp_Click(){
    QString rawurl;
    switch (Scena){
 case 0:
        switch (Page){
  case 2:
            rawurl = "06-prefix-creation-wizard.html#general";
            break;
  case 3:
            rawurl = "06-prefix-creation-wizard.html#winepath";
            break;
  case 4:
            rawurl = "06-prefix-creation-wizard.html#quickmount";
            break;
  default:
            rawurl = "06-prefix-creation-wizard.html";
            break;
        }
        break;
        case 1:
        switch (Page){
  case 3:
            rawurl = "03-first-startup-wizard.html#winepath";
            break;
  case 4:
            rawurl = "03-first-startup-wizard.html#sysutils";
            break;

  case 5:
            rawurl = "03-first-startup-wizard.html#userutils";
            break;
  case 6:
            rawurl = "03-first-startup-wizard.html#proxy";
            break;
  case 7:
            rawurl = "03-first-startup-wizard.html#qmount";
            break;
  default:
            rawurl = "03-first-startup-wizard.html";
            break;
        }
        break;
    }

    CoreLib->openHelpUrl(rawurl);
}

void Wizard::previosWizardPage(){
    /*
        Back command function clicking
    */

    /*	switch(Scena){
            case 0:
                switch (Page){
                    case 1:

                    break;
                }
            break;
        }
    */
    Page--;
    updateScena();
    return;
}

void Wizard::updateScena(){
    lblStep->setText(Wizard::tr("<b>Step %1 of %2</b>").arg(Page).arg(TotalPage));
    switch(Scena){
 case 1:
               /*
                * New prefix creation
                */
        switch (Page){
  case 1:
            lblCaption->setText(Wizard::tr("<b>First startup wizrad</b>"));
            lblWizardInfo->setText(Wizard::tr("<p>Welcome to first startup wizard.</p><p>This wizard helps you to make all necessary steps for successful %1 setup.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>").arg(APP_NAME));
            stkWizards->setCurrentIndex(0);
            cmdNext->setEnabled(TRUE);
            cmdBack->setEnabled(FALSE);
            break;
  case 2:
            txtInfo->setText(tr("<p><b><span style='font-weight:600; color:#6495ed;'>%1</span></b> was initially written by Alexey S. Malakhov aka John Brezerk  [<a href='mailto:brezerk@gmail.com'>brezerk@gmail.com</a>]</p><p>General idea comes from <b><span style='font-weight:600; color:#6495ed;'>WineTools</span></b> scripts which was initially written by Frank Hendriksen [<a href='mailto:frank@frankscorner.org'>frank@frankscorner.org</a>]</p><p>It is licensed under the <b><span style='font-weight:600; color:#6495ed;'>GPL v3</span></b>.</p><p>Send comments, bugreports, etc. to [<a href='mailto:brezerk@gmail.com'>brezerk@gmail.com</a>]</p><p><b><span style='font-weight:600; color:#6495ed;'>Note</span></b>: This software comes with absolutely no warranty. You will <b><span style='font-weight:600; color:#7D1D10;'>NOT</span></b> get any support or help for WineTools, Wine, software installations, Linux or Microsoft Windows from the author.</p><p>If you <span style='font-weight:600; color:#6495ed;'>need help</span>, ask the mailing lists at <a href='http://www.winehq.org/site/forums'>http://www.winehq.org/site/forums</a>.</p><p>If you <span style='font-weight:600; color:#6495ed;'>want support</span>, buy the commercial versions of wine: CodeWeavers CrossOver Office (<a href='http://www.codeweavers.com'>http://www.codeweavers.com</a>) for Desktop Applications</p>").arg(APP_NAME));
            stkWizards->setCurrentIndex(1);
            cmdNext->setEnabled(TRUE);
            cmdBack->setEnabled(TRUE);
            break;
  case 3:
            stkWizards->setCurrentIndex(2);
            stkFirstStartup->setCurrentIndex(0);
            break;
  case 4:
            stkFirstStartup->setCurrentIndex(1);
            break;
  case 5:
            stkFirstStartup->setCurrentIndex(2);
            break;
  case 6:
            stkFirstStartup->setCurrentIndex(3);
            cmdNext->setText(tr("Next >"));
            break;
  case 7:
            stkFirstStartup->setCurrentIndex(4);
            stkWizards->setCurrentIndex(2);
            cmdNext->setText(tr("Next >"));
            break;
  case 8:
            stkWizards->setCurrentIndex(0);
            lblWizardInfo->setText(tr("<p>All ready for finishing %1 setup. </p><p>Please, press the <b>Finish</b> button to create finish setup process. Or press <b>Back</b> button for return.</p>").arg(APP_NAME));
            cmdNext->setText(tr("Finish"));
            break;
        }
        break;
    }

    cmdNext->setFocus(Qt::ActiveWindowFocusReason);

    return;
}

void Wizard::comboMountProfiles_currentIndexChanged(int index){
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
