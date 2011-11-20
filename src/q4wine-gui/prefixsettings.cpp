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

#include "prefixsettings.h"

PrefixSettings::PrefixSettings(QString prefix_name, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
    // Setup base UI
    setupUi(this);

    // Setting class prefix name
    this->prefix_name=prefix_name;

    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName("libq4wine-core");
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif
    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->addNew=false;
    this->loadThemeIcons();

    QHash<QString,QString> result = db_prefix.getByName(prefix_name);
    if (result.value("id").isEmpty())
        return;

    prefix_id=result.value("id");

    txtMountPoint->setText(result.value("mount"));

    connect(txtMountPoint, SIGNAL(editingFinished()), this, SLOT(getWineCdromLetter()));

    if (prefix_name=="Default"){
        txtPrefixName->setEnabled(FALSE);
        txtPrefixPath->setEnabled(FALSE);

        cmdGetPrefixPath->setEnabled(FALSE);

        txtWineLibs->setEnabled(FALSE);
        txtWineLoaderBin->setEnabled(FALSE);
        txtWineServerBin->setEnabled(FALSE);
        txtWineBin->setEnabled(FALSE);

        cmdGetWineLibs->setEnabled(FALSE);
        cmdGetWineLoaderBin->setEnabled(FALSE);
        cmdGetWineServerBin->setEnabled(FALSE);
        cmdGetWineBin->setEnabled(FALSE);

        txtWineLibs->setText("");
        txtWineLoaderBin->setText("");
        txtWineServerBin->setText("");
        txtWineBin->setText("");
    } else {
        cmdGetPrefixPath->installEventFilter(this);
        cmdGetWineBin->installEventFilter(this);
        cmdGetWineServerBin->installEventFilter(this);
        cmdGetWineLoaderBin->installEventFilter(this);
        cmdGetWineLibs->installEventFilter(this);

        txtWineLibs->setText(result.value("libs"));
        txtWineLoaderBin->setText(result.value("loader"));
        txtWineServerBin->setText(result.value("server"));
        txtWineBin->setText(result.value("bin"));
    }

    if (!result.value("arch").isEmpty())
        comboArchList->setCurrentIndex(comboArchList->findText(result.value("arch")));
    if (!result.value("mountpoint_windrive").isEmpty()){
        comboWinDrive->setCurrentIndex(comboWinDrive->findText(result.value("mountpoint_windrive")));
    } else {
        this->getWineCdromLetter();
    }

    txtPrefixName->setText(prefix_name);
    txtPrefixPath->setText(result.value("path"));

    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));

       // comboArchList->setEnabled(false);

    cmdGetMountPoint->installEventFilter(this);

    cmdOk->setFocus(Qt::ActiveWindowFocusReason);
    return;
}

PrefixSettings::PrefixSettings(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
    // Setup base UI
    setupUi(this);

    this->setWindowTitle(tr("Add new Prefix"));
    this->lblCaption->setText(tr("Add new Prefix"));
    this->addNew=true;

    // Loading libq4wine-core.so
    libq4wine.setFileName("libq4wine-core");

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->loadThemeIcons();

    cmdGetPrefixPath->installEventFilter(this);
    cmdGetWineBin->installEventFilter(this);
    cmdGetWineServerBin->installEventFilter(this);
    cmdGetWineLoaderBin->installEventFilter(this);
    cmdGetWineLibs->installEventFilter(this);
    cmdGetMountPoint->installEventFilter(this);

    //comboArchList->setEnabled(true);

    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));

    cmdCancel->setFocus(Qt::ActiveWindowFocusReason);
    return;
}

QString PrefixSettings::getPrefixName(){
    return txtPrefixName->text();
}

void PrefixSettings::loadThemeIcons(){
    lblLogo->setPixmap(CoreLib->loadPixmap("data/exec.png"));

    cmdGetWineBin->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWineServerBin->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWineLoaderBin->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetWineLibs->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetMountPoint->setIcon(CoreLib->loadIcon("data/folder.png"));
    cmdGetPrefixPath->setIcon(CoreLib->loadIcon("data/folder.png"));
    return;
}

void PrefixSettings::cmdCancel_Click(){
    reject();
    return;
}

void PrefixSettings::cmdOk_Click(){
    if (txtPrefixName->text().isEmpty()){
         QMessageBox::warning(this, tr("Error"), tr("Enter a prefix name"));
          return;
    }

    if (prefix_name!=txtPrefixName->text()){
        if (db_prefix.isExistsByName(txtPrefixName->text())){
            QMessageBox::warning(this, tr("Error"), tr("A prefix named Sorry, but prefix named %1 already exists.quot;%1Sorry, but prefix named %1 already exists.quot; already exists").arg(txtPrefixName->text()));
            return;
        }
    }

    QString path = txtPrefixPath->text();
    if (path.right(1)=="/"){
        path=path.left(path.length()-1);
        txtPrefixPath->setText(path);
    }

    if (this->addNew){
        if (!db_prefix.addPrefix(txtPrefixName->text(),  txtPrefixPath->text(), txtWineBin->text(), txtWineServerBin->text(), txtWineLoaderBin->text(), txtWineLibs->text(), txtMountPoint->text(), comboArchList->currentText(), this->comboWinDrive->currentText()))
            reject();

        CoreLib->createPrefixDBStructure(txtPrefixName->text());
    } else {
        if (!db_prefix.updatePrefix(txtPrefixName->text(), txtPrefixPath->text(), txtWineBin->text(), txtWineServerBin->text(), txtWineLoaderBin->text(), txtWineLibs->text(), txtMountPoint->text(), this->prefix_name, comboArchList->currentText(), this->comboWinDrive->currentText()))
            reject();
    }

    accept();
    return;
}

bool PrefixSettings::eventFilter(QObject *obj, QEvent *event){
    /*
     * Select folder dialog
     */

    if (event->type() == QEvent::MouseButtonRelease) {

        QString file="";

        if (obj->objectName().right(3)=="Bin"){
#if QT_VERSION >= 0x040500
            QFileDialog::Options options;

            if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
                options = QFileDialog::DontUseNativeDialog;

            file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), "All files (*)", 0, options);
#else
            file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), "All files (*)");
#endif
        } else {
#if QT_VERSION >= 0x040500
            QFileDialog::Options options;

            if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
                options = QFileDialog::DontUseNativeDialog;

            file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), options);
#else
            file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), QFileDialog::DontResolveSymlinks);
#endif
        }

        if (!file.isEmpty()){
            if (file.right(1)=="/"){
                file=file.left(file.length()-1);
            }

            QString a="";

            a.append("txt");
            a.append(obj->objectName().right(obj->objectName().length()-6));

            std::auto_ptr<QLineEdit> lineEdit (findChild<QLineEdit *>(a));

            if (lineEdit.get()){
                lineEdit->setText(file);
            } else {
                qDebug("Error");
            }

            lineEdit.release();

            if (obj->objectName()=="cmdGetPrefixPath"){
                txtPrefixName->setText(txtPrefixPath->text().split("/").last());
            }

            if (obj->objectName()=="cmdGetMountPoint"){
                this->getWineCdromLetter();
            }

        }
    }

    return FALSE;
}

void PrefixSettings::cmdHelp_Click(){
    QString rawurl;
    switch (twbGeneral->currentIndex()){
    case 0:
        rawurl = "10-prefix-settings.html#general";
    break;
    case 1:
        rawurl = "10-prefix-settings.html#winepath";
    break;
    }

    CoreLib->openHelpUrl(rawurl);
}

void PrefixSettings::getWineCdromLetter(){
    if (!txtMountPoint->text().isEmpty()){
        QChar letter = CoreLib->getCdromWineDrive(db_prefix.getPath(prefix_name), txtMountPoint->text());
        if (!letter.isNull()){
            comboWinDrive->setCurrentIndex(comboWinDrive->findText(QString("%1:").arg(letter.toUpper())));
        }
    }
}
