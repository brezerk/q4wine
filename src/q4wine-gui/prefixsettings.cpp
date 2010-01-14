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

#include "prefixsettings.h"

PrefixSettings::PrefixSettings(QString prefix_name, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	// Setup base UI
	setupUi(this);

	// Setting class prefix name
	this->prefix_name=prefix_name;

	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	this->loadThemeIcons(this->CoreLib->getSetting("app", "theme", false).toString());

	QStringList result = db_prefix.getFieldsByPrefixName(prefix_name);
	if (result.at(0) == "-1")
		return;

	prefix_id=result.at(0);

	txtWineLibs->setText(result.at(2));
	txtWineLoaderBin->setText(result.at(3));
	txtWineServerBin->setText(result.at(4));
	txtWineBin->setText(result.at(5));

	//comboDeviceList
	txtMountPoint->setText(result.at(6));

	if (prefix_name=="Default"){
		txtPrefixName->setEnabled(FALSE);
		txtPrefixPath->setEnabled(FALSE);
		cmdGetPrefixPath->setEnabled(FALSE);
	} else {
		cmdGetPrefixPath->installEventFilter(this);
	}

	txtPrefixName->setText(prefix_name);
	txtPrefixPath->setText(result.at(1));

	comboDeviceList->addItems(CoreLib->getCdromDevices());
	if (!result.at(7).isEmpty()){
		comboDeviceList->setCurrentIndex (comboDeviceList->findText(result.at(7)));
		if (comboDeviceList->currentText().isEmpty())
			comboDeviceList->setCurrentIndex (0);
	} else {
		comboDeviceList->setCurrentIndex (0);
	}

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));

	cmdGetWineBin->installEventFilter(this);
	cmdGetWineServerBin->installEventFilter(this);
	cmdGetWineLoaderBin->installEventFilter(this);
	cmdGetWineLibs->installEventFilter(this);

	cmdGetMountPoint->installEventFilter(this);

	cmdOk->setFocus(Qt::ActiveWindowFocusReason);
	return;
}

QString PrefixSettings::getPrefixName(){
	return txtPrefixName->text();
}

void PrefixSettings::loadThemeIcons(QString themePath){
	QPixmap pixmap;

	if (!pixmap.load(QString("%1/data/exec.png").arg(themePath))){
		pixmap.load(":data/exec.png");
	}

	lblLogo->setPixmap(pixmap);

	cmdGetWineBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineServerBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineLoaderBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineLibs->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetMountPoint->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetPrefixPath->setIcon(loadIcon("data/folder.png", themePath));

	return;
}


QIcon PrefixSettings::loadIcon(QString iconName, QString themePath){
	// Function tryes to load icon image from theme dir
	// If it fails -> load default from resource file

	QIcon icon;

	if ((!themePath.isEmpty()) and (themePath!="Default")){
		icon.addFile(QString("%1/%2").arg(themePath).arg(iconName));
		if (icon.isNull()){
			icon.addFile(QString(":/%1").arg(iconName));
		}
	} else {
		icon.addFile(QString(":/%1").arg(iconName));
	}

	return icon;
}

void PrefixSettings::cmdCancel_Click(){
	reject();
	return;
}

void PrefixSettings::cmdOk_Click(){
	if (txtPrefixName->text().isEmpty()){
		 QMessageBox::warning(this, tr("Error"), tr("Please, enter prefix name"));
		  return;
	}

	if (prefix_name!=txtPrefixName->text()){
		if (db_prefix.isExistsByName(txtPrefixName->text())){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, but prefix named %1 already exists.").arg(txtPrefixName->text()));
			return;
		}
	}

	if (!db_prefix.updatePrefix(txtPrefixName->text(), txtPrefixPath->text(), txtWineBin->text(), txtWineServerBin->text(), txtWineLoaderBin->text(), txtWineLibs->text(), txtMountPoint->text(), comboDeviceList->currentText(), this->prefix_name))
		reject();

	accept();
	return;
}

bool PrefixSettings::eventFilter(QObject *obj, QEvent *event){
	/*
	 * Select folder dialog
	 */

	if (event->type() == QEvent::MouseButtonPress) {

		QString file="";

		if (obj->objectName().right(3)=="Bin"){
			file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),   "All files (*.*)");
		} else {
			file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),   QFileDialog::DontResolveSymlinks);
		}

		if (!file.isEmpty()){
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

