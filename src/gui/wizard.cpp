/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#include "wizard.h"

void Wizard::loadThemeIcons(QString themePath, int Scene){
	QPixmap pixmap;

	switch (Scene){
	case 0:
		// Prefix creation
		if (!pixmap.load(tr("%1/data/prefixc.png").arg(themePath))){
			pixmap.load(":data/prefixc.png");
		}

		lblPicture->setPixmap(pixmap);


		cmdGetWineBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetWineServerBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetWineLoaderBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetWineDllPath->setIcon(loadIcon("data/folder.png", themePath));

		cmdGetPrefixPath->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetMountPoint->setIcon(loadIcon("data/folder.png", themePath));

		break;
	case 1:
		// First startup
		if (!pixmap.load(tr("%1/data/firstc.png").arg(themePath))){
			pixmap.load(":data/firstc.png");
		}

		lblPicture->setPixmap(pixmap);

		cmdGetWineBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetWineServerBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetWineLoaderBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetWineDllPath->setIcon(loadIcon("data/folder.png", themePath));

		cmdGetTarBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetMountBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetUmountBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetSudoBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetGuiSudoBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetNiceBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetReniceBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetShBin->setIcon(loadIcon("data/folder.png", themePath));

		cmdGetConsoleBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetWrestoolBin->setIcon(loadIcon("data/folder.png", themePath));
		cmdGetIcotoolBin->setIcon(loadIcon("data/folder.png", themePath));

#ifdef WITHOUT_ICOTOOLS
		cmdGetWrestoolBin->setEnabled(false);
		cmdGetIcotoolBin->setEnabled(false);
		txtWrestoolBin->setEnabled(false);
		txtIcotoolBin->setEnabled(false);
#endif

		connect(radioDefault, SIGNAL(toggled(bool)), this, SLOT(radioDefault_toggled(bool)));
		connect(radioDefaultGui, SIGNAL(toggled(bool)), this, SLOT(radioDefaultGui_toggled(bool)));
		connect(radioFuse, SIGNAL(toggled(bool)), this, SLOT(radioFuse_toggled(bool)));
		connect(radioEmbedded, SIGNAL(toggled(bool)), this, SLOT(radioEmbedded_toggled(bool)));

		radioDefaultGui->setChecked(true);
		radioDefault->setChecked(true);

		break;
	case 2:
		// Fake drive
		if (!pixmap.load(tr("%1/data/firstc.png").arg(themePath))){
			pixmap.load(":data/firstc.png");
		}
		lblPicture->setPixmap(pixmap);
		break;
	case 3:
		// Fake drive
		if (!pixmap.load(tr("%1/data/firstc.png").arg(themePath))){
			pixmap.load(":data/firstc.png");
		}
		lblPicture->setPixmap(pixmap);
		break;
	}
	return;
}


QIcon Wizard::loadIcon(QString iconName, QString themePath){
	// Function tryes to load icon image from theme dir
	// If it fails -> load default from rsource file

	QIcon icon;

	if ((!themePath.isEmpty()) and (themePath!="Default")){
		icon.addFile(tr("%1/%2").arg(themePath).arg(iconName));
		if (icon.isNull()){
			icon.addFile(tr(":/%1").arg(iconName));
		}
	} else {
		icon.addFile(tr(":/%1").arg(iconName));
	}

	return icon;
}



Wizard::Wizard(int WizardType, QString var1, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	/*
		Note: var1 is optional data, used for different scene actions ;)
	*/

	setupUi(this);

	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib = (corelib *)CoreLibClassPointer(true);

	// Creating database classes
	db_prefix = new Prefix();
	db_icon = new Icon();
	db_dir = new Dir();

	widgetCreatePrefix0->setVisible(FALSE);

	Scena=WizardType;
	Page=1;
	QString console_w;

	loadThemeIcons(CoreLib->getSetting("app", "theme", false).toString(), Scena);


	switch (Scena){
	case 0:
		TotalPage=5;

		setWindowTitle(tr("New prefix creation wizard"));
		lblCaption->setText(tr("<b>Prefix creation wizard</b>"));
		lblStep->setText(tr("<b>Step %1 of %2</b>").arg(Page).arg(TotalPage));

		connect (cbCreafeFake, SIGNAL(stateChanged (int)), this, SLOT(changeBoxState(int)));

		cmdGetPrefixPath->installEventFilter(this);
		cmdGetWineBin->installEventFilter(this);
		cmdGetWineServerBin->installEventFilter(this);
		cmdGetWineLoaderBin->installEventFilter(this);
		cmdGetWineDllPath->installEventFilter(this);
		cmdGetMountPoint->installEventFilter(this);

		combSourceDevice->addItems(CoreLib->getCdromDevices());
		break;
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
		cmdGetWrestoolBin->installEventFilter(this);
		cmdGetIcotoolBin->installEventFilter(this);

		txtWineBin->setText(CoreLib->getWhichOut("wine"));
		txtWineServerBin->setText(CoreLib->getWhichOut("wineserver"));
		txtWineLoaderBin->setText(CoreLib->getWhichOut("wine"));

		if (QDir("/usr/lib/wine").exists())
			txtWineDllPath->setText("/usr/lib/wine");

		if (QDir("/local/usr/lib/wine").exists())
			txtWineDllPath->setText("/local/usr/lib/wine");

		if (QDir("/usr/local/lib/wine").exists())
			txtWineDllPath->setText("/usr/local/lib/wine");

		txtTarBin->setText(CoreLib->getWhichOut("tar"));
		txtMountBin->setText(CoreLib->getWhichOut("mount"));
		txtUmountBin->setText(CoreLib->getWhichOut("umount"));
		txtSudoBin->setText(CoreLib->getWhichOut("sudo"));

		txtGuiSudoBin->setText(CoreLib->getWhichOut("kdesu"));
		if (txtGuiSudoBin->text().isEmpty()){
			txtGuiSudoBin->setText(CoreLib->getWhichOut("gksu"));
		} else {
			if (txtGuiSudoBin->text().isEmpty()){
				txtGuiSudoBin->setText(CoreLib->getWhichOut("sudo"));
			}
		}

		txtNiceBin->setText(CoreLib->getWhichOut("nice"));
		txtReniceBin->setText(CoreLib->getWhichOut("renice"));
		txtShBin->setText(CoreLib->getWhichOut("sh"));

		console_w = CoreLib->getWhichOut("konsole");
		if (!console_w.isEmpty()){
			txtConsoleBin->setText(console_w);
			txtConsoleArgs->setText("--noclose -e");
		} else {
			console_w = CoreLib->getWhichOut("xterm");
			if (!console_w.isEmpty()){
				txtConsoleBin->setText(console_w);
				txtConsoleArgs->setText("-e");
			}
		}

#ifndef WITHOUT_ICOTOOLS
		txtWrestoolBin->setText(CoreLib->getWhichOut("wrestool"));
		txtIcotoolBin->setText(CoreLib->getWhichOut("icotool"));
#endif

		break;
	case 2:
		TotalPage=6;
		this->var1=var1;
		setWindowTitle(tr("Fake drive creation wizard"));
		lblCaption->setText(tr("<b>Fake drive creation wizard</b>"));
		lblStep->setText(tr("<b>Step %1 of %2</b>").arg(Page).arg(TotalPage));
		lblWizardInfo->setText(Wizard::tr("<p>Welcome to fake drive creation wizard.</p><p>This wizard helps you to make all necessary steps for successful fake drive creation.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>"));
		break;
	case 3:
		TotalPage=6;
		this->var1=var1;
		setWindowTitle(tr("Fake drive update wizard"));
		lblCaption->setText(tr("<b>Fake drive update wizard</b>"));
		lblStep->setText(tr("<b>Step %1 of %2</b>").arg(Page).arg(TotalPage));
		lblWizardInfo->setText(Wizard::tr("<p>Welcome to fake drive update wizard.</p><p>This wizard helps you to make all necessary steps for successful fake drive creation.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>"));
		Scena=2;
		break;
	}

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(reject ()));
	connect(cmdNext, SIGNAL(clicked()), this, SLOT(nextWizardPage()));
	connect(cmdBack, SIGNAL(clicked()), this, SLOT(previosWizardPage()));
	connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
	connect(comboProxyType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboProxyType_indexChanged(QString)));

	widgetCreatePrefix0->setVisible(FALSE);
	widgetCreatePrefix1->setVisible(FALSE);
	widgetCreatePrefix2->setVisible(FALSE);

	widgetCreateFakeDrive0->setVisible(FALSE);
	widgetCreateFakeDrive1->setVisible(FALSE);
	widgetCreateFakeDrive2->setVisible(FALSE);
	widgetCreateFakeDrive3->setVisible(FALSE);

	widgetFirstStartup0->setVisible(FALSE);
	widgetFirstStartup1->setVisible(FALSE);
	widgetFirstStartup2->setVisible(FALSE);
	widgetFirstStartup3->setVisible(FALSE);
	widgetFirstStartup4->setVisible(FALSE);

	widgetInfo->setVisible(FALSE);

	updateScena();

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
	if (this->widgetFrame->width()<=100)
		return FALSE;

	if (event->type() == QEvent::Paint){
		this->widgetInfo->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive0->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreateFakeDrive1->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreateFakeDrive2->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreateFakeDrive3->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreatePrefix0->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreatePrefix1->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreatePrefix2->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup0->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup1->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup2->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup3->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup4->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		return TRUE;
	}

	if (obj->objectName()== "Wizard")
		return FALSE;

	if (event->type() == QEvent::MouseButtonPress) {
		QString file;
		if (obj->objectName().right(3)=="Bin"){
			file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),   "All files (*)");
		} else {
			file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),   QFileDialog::DontResolveSymlinks);
		}

		if (!file.isEmpty()){
			QString a;
			a.append("txt");
			a.append(obj->objectName().right(obj->objectName().length()-6));

			QLineEdit *lineEdit = findChild<QLineEdit *>(a);
			if (lineEdit){
				lineEdit->setText(file);
			} else {
				qDebug("Error");
			}
			if (obj==cmdGetWineBin){
				QString wrkDir;
				QStringList list1 = file.split("/");
				wrkDir = file.left(file.length() - list1.last().length());
				txtWineServerBin->setText(tr("%1wineserver").arg(wrkDir));
				txtWineLoaderBin->setText(tr("%1wine").arg(wrkDir));
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
#ifndef WITHOUT_ICOTOOLS
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
	  if (radioDefault->isChecked()){
		  settings.setValue("type", 0);
		  if (txtMountString->text().isEmpty()){
			  #ifdef _OS_LINUX_
				  txtMountString->setText("%SUDO% %MOUNT_BIN% %MOUNT_DRIVE% %MOUNT_POINT%");
			  #endif
			  #ifdef _OS_FREEBSD_
				  txtMountString->setText("%SUDO% %MOUNT_BIN% -t cd9660 %MOUNT_DRIVE% %MOUNT_POINT%");
			  #endif
		  }

		  if (txtMountImageString->text().isEmpty()){
			  #ifdef _OS_LINUX_
				  txtMountImageString->setText("%SUDO% %MOUNT_BIN% %MOUNT_OPTIONS% %MOUNT_IMAGE% %MOUNT_POINT%");
			  #endif
			  #ifdef _OS_FREEBSD_
				  txtMountString->setText("%SUDO% %MOUNT_BIN% -t cd9660 /dev/`%MDCONFIG_BIN% -f %%MOUNT_IMAGE%` %MOUNT_POINT%");
			  #endif
		  }

		  if (txtUmountString->text().isEmpty()){
			  #ifdef _OS_LINUX_
				  txtUmountString->setText("%SUDO% %UMOUNT_BIN% %MOUNT_POINT%");
			  #endif
			  #ifdef _OS_FREEBSD_
				  QString umount="%SUDO% ";
				  umount.append(APP_PREF);
				  umount.append("/share/q4wine/scripts/umount_freebsd.sh");
				  umount.append(" %UMOUNT_BIN% %MOUNT_POINT%");
				  txtUmountString->setText(umount);
			  #endif
		  }

	  }

	  if (radioDefaultGui->isChecked()){
		  settings.setValue("type", 1);
		  if (txtMountString->text().isEmpty()){
			  #ifdef _OS_LINUX_
				  txtMountString->setText("%GUI_SUDO% \"%MOUNT_BIN% %MOUNT_DRIVE% %MOUNT_POINT%\"");
			  #endif
			  #ifdef _OS_FREEBSD_
				  txtMountString->setText("%GUI_SUDO% \"%MOUNT_BIN% -t cd9660 %MOUNT_DRIVE% %MOUNT_POINT%\"");
			  #endif
		  }

		  if (txtMountImageString->text().isEmpty()){
			  #ifdef _OS_LINUX_
				  txtMountImageString->setText("%GUI_SUDO% \"%MOUNT_BIN% %MOUNT_OPTIONS% %MOUNT_IMAGE% %MOUNT_POINT%\"");
			  #endif
			  #ifdef _OS_FREEBSD_
				  txtMountString->setText("%GUI_SUDO% \"%MOUNT_BIN% -t cd9660 /dev/`%MDCONFIG_BIN% -f %%MOUNT_IMAGE%` %MOUNT_POINT%\"");
			  #endif
		  }

		  if (txtUmountString->text().isEmpty()){
			  #ifdef _OS_LINUX_
				  txtUmountString->setText("%GUI_SUDO% %UMOUNT_BIN% %MOUNT_POINT%");
			  #endif
			  #ifdef _OS_FREEBSD_
				  QString umount="%GUI_SUDO% ";
				  umount.append(APP_PREF);
				  umount.append("/share/q4wine/scripts/umount_freebsd.sh");
				  umount.append(" %UMOUNT_BIN% %MOUNT_POINT%");
				  txtUmountString->setText(umount);
			  #endif
		  }
	  }
	  if (radioFuse->isChecked()){
		  settings.setValue("type", 2);
		  QString format;
		  if (txtMountString->text().isEmpty()){
			  format=CoreLib->getWhichOut("fuseiso");
			  format.append(" %MOUNT_DRIVE% %MOUNT_POINT%");
			  txtMountString->setText(format);
		  }
		  if (txtMountImageString->text().isEmpty()){
			  format=CoreLib->getWhichOut("fuseiso");
			  format.append(" %MOUNT_IMAGE% %MOUNT_POINT%");
			  txtMountImageString->setText(format);
		  }
		  if (txtUmountString->text().isEmpty()){
			  format=CoreLib->getWhichOut("fusermount");
			  format.append(" -u %MOUNT_POINT%");
			  txtUmountString->setText(format);
		  }
	  }
	   if (radioEmbedded->isChecked()){
		   QString format;
		   settings.setValue("type", 3);
		   if (txtMountString->text().isEmpty()){
			   format=APP_PREF;
			   format.append("/bin/q4wine-mount %MOUNT_DRIVE% %MOUNT_POINT%");
			   txtMountString->setText(format);
		   }
		   if (txtMountImageString->text().isEmpty()){
			   format=APP_PREF;
			   format.append("/bin/q4wine-mount %MOUNT_IMAGE% %MOUNT_POINT%");
			   txtMountImageString->setText(format);
		   }
		   if (txtUmountString->text().isEmpty()){
			   format=CoreLib->getWhichOut("fusermount");
			   format.append(" -u %MOUNT_POINT%");
			   txtUmountString->setText(format);
		   }
	   }

	  settings.setValue("mount_drive_string", txtMountString->text());
	  settings.setValue("mount_image_string", txtMountImageString->text());
	  settings.setValue("umount_string", txtUmountString->text());

	  settings.endGroup();

			accept();
			break;
		}
		break;
	case 0:
		switch (Page){
				case 2:
			if (txtPrefixName->text().isEmpty()){
				QMessageBox::warning(this, tr("Error"), tr("Enter prefix name first."));
				return;
			} else {
				if (db_prefix->isExistsByName(txtPrefixName->text())){
					QMessageBox::warning(this, tr("Error"), tr("Sorry. There is other prefix with same name."));
					return;
				}
			}
			if (!txtPrefixPath->text().isEmpty()){
				if (!QDir(txtPrefixPath->text()).exists()){
					QMessageBox::warning(this, tr("Error"), tr("Sorry, prefix path is wrong! Directory not exists."));
					return;
				}
			} else {
				QMessageBox::warning(this, tr("Error"), tr("Select prefix directory first."));
				return;
			}
			break;
				case 4:
			if ((txtMountPoint->text().isEmpty()) and (combSourceDevice->currentText()!=tr("<none>"))){
				QMessageBox::warning(this, tr("Error"), tr("Sorry, specify mount point directory."));
				return;
			}
			if ((!txtMountPoint->text().isEmpty()) and (combSourceDevice->currentText()==tr("<none>"))){
				QMessageBox::warning(this, tr("Error"), tr("Sorry, specify source device."));
				return;
			}
			break;
				case 5:
			if (!db_prefix->addPrefix(txtPrefixName->text(), txtPrefixPath->text(), txtWineBin->text(), txtWineServerBin->text(), txtWineLoaderBin->text(), txtWineDllPath->text(), txtMountPoint->text(), combSourceDevice->currentText()))
				reject();
			if (cbCreafeFake->checkState()==Qt::Checked){
				Wizard *createFakeDriveWizard = new Wizard(2, txtPrefixName->text());
				createFakeDriveWizard->exec();
			}
			accept();
			break;
		}
		break;
	case 2:
		switch (Page){
				case 6:
			QApplication::setOverrideCursor( Qt::BusyCursor );

			//Set variables
			QSqlQuery query;
			QString prefix_name = var1;
			QString prefix_id;

			//Getting versions
			QString version;
			if (comboFakeVersion->currentText()=="Windows XP")
				version = "winxp";
			if (comboFakeVersion->currentText()=="Windows 2008")
				version = "win2008";
			if (comboFakeVersion->currentText()=="Windows Vista")
				version = "vista";
			if (comboFakeVersion->currentText()=="Windows 2003")
				version = "win2003";
			if (comboFakeVersion->currentText()=="Windows 2000")
				version = "win2k";
			if (comboFakeVersion->currentText()=="Windows ME")
				version = "winme";
			if (comboFakeVersion->currentText()=="Windows 98")
				version = "win98";
			if (comboFakeVersion->currentText()=="Windows 95")
				version = "win95";
			if (comboFakeVersion->currentText()=="Windows NT 4.0")
				version = "nt40";
			if (comboFakeVersion->currentText()=="Windows NT 3.0")
				version = "nt351";
			if (comboFakeVersion->currentText()=="Windows 3.1")
				version = "win31";
			if (comboFakeVersion->currentText()=="Windows 3.0")
				version = "win30";
			if (comboFakeVersion->currentText()=="Windows 2.0")
				version = "win20";

			WineBinLauncher *launcher;
			launcher = new WineBinLauncher(prefix_name);
			launcher->appendWineExe("prefixcreate");
			launcher->run_exec(this, "", "-w", TRUE);
			if (launcher->exec()!=QDialog::Accepted){
				QApplication::restoreOverrideCursor();
				qDebug()<<"Error while run WineExe: ";
				reject();
				return;
			}

			Registry registry;

			if (registry.init()){
				registry.append(tr("[HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows NT\\CurrentVersion]\n\"RegisteredOrganization\"=\"%1\"\n\"RegisteredOwner\"=\"%2\"\n\n[HKEY_CURRENT_USER\\Software\\Wine]\n\"Version\"=\"%3\"").arg(txtOrganization->text()).arg(txtOwner->text()).arg(version));

				if (!txtFakeBrowsers->text().isEmpty())
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\WineBrowser]\n\"Browsers\"=\"%1\"").arg(txtFakeBrowsers->text()));

				if (!txtFakeMailers->text().isEmpty())
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\WineBrowser]\n\"Mailers\"=\"%1\"").arg(txtFakeMailers->text()));

				if (comboFakeD3D_Multi->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\Direct3D]\n\"Multisampling\"=\"%1\"").arg(comboFakeD3D_Multi->currentText()));

				if (comboFakeD3D_Render->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\Direct3D]\n\"DirectDrawRenderer\"=\"%1\"").arg(comboFakeD3D_Render->currentText()));

				if (comboFakeD3D_LMode->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\Direct3D]\n\"RenderTargetLockMode\"=\"%1\"").arg(comboFakeD3D_LMode->currentText()));

				if (comboFakeD3D_Offscreen->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\Direct3D]\n\"OffscreenRenderingMode\"=\"%1\"").arg(comboFakeD3D_Offscreen->currentText()));

				if (comboFakeD3D_GLSL->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\Direct3D]\n\"UseGLSL\"=\"%1\"").arg(comboFakeD3D_GLSL->currentText()));

				if (!txtFakeD3D_VMem->text().isEmpty())
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\Direct3D]\n\"VideoMemorySize\"=\"%1\"").arg(txtFakeD3D_VMem->text()));

				if (comboFakeX11_WR->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\X11 Driver]\n\"ClientSideWithRender\"=\"%1\"").arg(comboFakeX11_WR->currentText()));

				if (comboFakeX11_AAR->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\X11 Driver]\n\"ClientSideAntiAliasWithRender\"=\"%1\"").arg(comboFakeX11_AAR->currentText()));

				if (comboFakeX11_AAC->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\X11 Driver]\n\"ClientSideAntiAliasWithCore\"=\"%1\"").arg(comboFakeX11_AAC->currentText()));

				if (comboFakeX11_XRandr->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\X11 Driver]\n\"UseXRandR\"=\"%1\"").arg(comboFakeX11_XRandr->currentText()));

				if (comboFakeX11_XVid->currentText()!="default")
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Software\\Wine\\X11 Driver]\n\"UseXVidMode\"=\"%1\"").arg(comboFakeX11_XVid->currentText()));

				if (cbUseQtColors->checkState()==Qt::Checked){
					registry.append(tr("\n\n[HKEY_CURRENT_USER\\Control Panel\\Colors]\n"));

					QColor color;
					QPalette cur_palette;

					cur_palette = qApp->palette();
					color = cur_palette.color(QPalette::Base);
					registry.append(tr("\"Window\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::Window);
					registry.append(tr("\"ActiveBorder\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"InactiveBorder\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"AppWorkSpace\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"Menu\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"MenuBar\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"Scrollbar\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"MenuHilight\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"ButtonFace\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::AlternateBase);
					registry.append(tr("\"ButtonAlternateFace\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::Dark);
					registry.append(tr("\"ButtonDkShadow\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"ButtonShadow\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"GrayText\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::Light);
					registry.append(tr("\"ButtonHilight\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::ButtonText);
					registry.append(tr("\"ButtonText\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::WindowText);
					registry.append(tr("\"MenuText\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"WindowFrame\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
					registry.append(tr("\"WindowText\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::Highlight);
					registry.append(tr("\"Hilight\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::HighlightedText);
					registry.append(tr("\"HilightText\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::ToolTipBase);
					registry.append(tr("\"InfoWindow\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));

					color = cur_palette.color(QPalette::ToolTipText);
					registry.append(tr("\"InfoText\"=\"%1 %2 %3\"\n").arg(QString::number ( color.red())).arg(QString::number ( color.green())).arg(QString::number ( color.blue())));
				}

				if (registry.exec(this, prefix_name)){
					QString dir_id;
					//Is settings directory exists?
					if (!db_dir->isExistsByName(prefix_name, "system")){
						db_dir->addDir(prefix_name, "system");
						//Adding icons
						db_icon->addIcon("", "winecfg.exe", "winecfg", "Configure the general settings for Wine", prefix_name, "system", "winecfg");
						db_icon->addIcon("--backend=user cmd", "wineconsole", "wineconsole", "Wineconsole is similar to wine command wcmd", prefix_name, "system", "console");
						db_icon->addIcon("", "uninstaller.exe", "uninstaller", "Uninstall Windows programs under Wine properly", prefix_name, "system", "uninstaller");
						db_icon->addIcon("", "regedit.exe", "regedit", "Wine registry editor", prefix_name, "system", "regedit");
						db_icon->addIcon("", "explorer.exe", "explorer", "Browse the files in the virtual Wine drive", prefix_name, "system", "explorer");
						db_icon->addIcon("", "eject.exe", "eject", "Wine CD eject tool", prefix_name, "system", "eject");
						db_icon->addIcon("", "wordpad.exe", "wordpad", "Wine wordpad text editor", prefix_name, "system", "wordpad");
					}
					if (!db_dir->isExistsByName(prefix_name, "autostart"))
						db_dir->addDir(prefix_name, "autostart");

					QApplication::restoreOverrideCursor();
					accept() ;
				} else {
					QApplication::restoreOverrideCursor();
					reject();
				}
			}
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
		default:
			rawurl = "03-first-startup-wizard.html";
			break;
		}
		break;
		case 2:
		switch (Page){
		case 2:
			rawurl = "07-fake-drive-wizard.html#general";
			break;
		case 3:
			rawurl = "07-fake-drive-wizard.html#winebrowsers";
			break;
		case 4:
			rawurl = "07-fake-drive-wizard.html#direct";
			break;
		case 5:
			rawurl = "07-fake-drive-wizard.html#x11";
			break;
		default:
			rawurl = "07-fake-drive-wizard.html";
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
				New prefix creation
				*/
		switch (Page){
					case 1:
			lblCaption->setText(Wizard::tr("<b>First startup wizrd</b>"));
			lblWizardInfo->setText(Wizard::tr("<p>Welcome to first startup wizard.</p><p>This wizard helps you to make all necessary steps for successful %1 setup.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>").arg(APP_NAME));
			widgetFirstStartup0->setVisible(FALSE);
			widgetInfo->setVisible(TRUE);
			cmdNext->setEnabled(TRUE);
			cmdBack->setEnabled(FALSE);
			break;
					case 2:
			txtInfo->setText(tr("<p><b><span style='font-weight:600; color:#6495ed;'>%1</span></b> was initially written by Malakhov Alexey aka John Brezerk  [<a href='mailto:brezerk@gmail.com'>brezerk@gmail.com</a>]</p><p>General ideia comes from <b><span style='font-weight:600; color:#6495ed;'>WineTools</span></b> scripts witch was initially written by Frank Hendriksen [<a href='mailto:frank@frankscorner.org'>frank@frankscorner.org</a>]</p><p>It is licensed under the <b><span style='font-weight:600; color:#6495ed;'>GPL v3</span></b>.</p><p>Send comments, bugreports, etc. to [<a href='mailto:brezerk@gmail.com'>brezerk@gmail.com</a>]</p><p><b><span style='font-weight:600; color:#6495ed;'>Note</span></b>: This software comes with absolutely no warranty. You will <b><span style='font-weight:600; color:#7D1D10;'>NOT</span></b> get any support or help for WineTools, Wine, software installations, Linux or Microsoft Windows from the author.</p><p>If you <span style='font-weight:600; color:#6495ed;'>need help</span>, ask the mailing lists at <a href='http://www.winehq.org/site/forums'>http://www.winehq.org/site/forums</a>.</p><p>If you <span style='font-weight:600; color:#6495ed;'>want support</span>, buy the commercial versions of wine: CodeWeavers CrossOver Office (<a href='http://www.codeweavers.com'>http://www.codeweavers.com</a>) for Desktop Applications</p>").arg(APP_NAME));
			widgetCreatePrefix1->setVisible(FALSE);
			widgetInfo->setVisible(FALSE);
			widgetFirstStartup0->setVisible(TRUE);
			cmdNext->setEnabled(TRUE);
			cmdBack->setEnabled(TRUE);
			break;
					case 3:
			widgetFirstStartup0->setVisible(FALSE);
			widgetFirstStartup1->setVisible(FALSE);
			widgetCreatePrefix1->setVisible(TRUE);
			lblInfoPrefix1->setText(tr("<p>Please enter default wine settings.</p>"));

			break;
					case 4:
			widgetCreatePrefix1->setVisible(FALSE);
			widgetFirstStartup2->setVisible(FALSE);
			widgetFirstStartup1->setVisible(TRUE);
			break;
					case 5:
			widgetFirstStartup1->setVisible(FALSE);
			widgetFirstStartup3->setVisible(FALSE);
			widgetFirstStartup2->setVisible(TRUE);
			break;
					case 6:
			widgetFirstStartup1->setVisible(FALSE);
			widgetFirstStartup2->setVisible(FALSE);
			widgetFirstStartup3->setVisible(TRUE);
			widgetFirstStartup4->setVisible(FALSE);
			cmdNext->setText(tr("Next >"));
			widgetInfo->setVisible(FALSE);
			break;
					case 7:
			widgetFirstStartup3->setVisible(FALSE);
			widgetFirstStartup4->setVisible(TRUE);
			cmdNext->setText(tr("Next >"));
			widgetInfo->setVisible(FALSE);
			break;
								case 8:
			lblWizardInfo->setText(tr("<p>All ready for finishing %1 setup. </p><p>Please, press the <b>Finish</b> button to create finish setup process. Or press <b>Back</b> button for return.</p>").arg(APP_NAME));
			widgetFirstStartup4->setVisible(FALSE);
			widgetInfo->setVisible(TRUE);
			cmdNext->setText(tr("Finish"));
			break;
		}
		break;
			case 0:
		/*
					New prefix creation
				*/
		switch (Page){
					case 1:
			lblCaption->setText(tr("<b>Prefix creation wizard</b>"));
			lblWizardInfo->setText(tr("<p>Welcome to prefix creation wizard.</p><p>This wizard helps you to make all necessary steps for successful Wine prefix creation.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>"));
			widgetCreatePrefix0->setVisible(FALSE);
			widgetCreatePrefix1->setVisible(FALSE);
			widgetCreatePrefix2->setVisible(FALSE);
			widgetInfo->setVisible(TRUE);
			cmdNext->setEnabled(TRUE);
			cmdBack->setEnabled(FALSE);
			break;
					case 2:
			widgetInfo->setVisible(FALSE);
			widgetCreatePrefix0->setVisible(TRUE);
			widgetCreatePrefix1->setVisible(FALSE);
			widgetCreatePrefix2->setVisible(FALSE);
			cmdBack->setEnabled(TRUE);
			//cmdNext->setText(tr("Next >"));
			break;
					case 3:
			widgetCreatePrefix0->setVisible(FALSE);
			widgetCreatePrefix1->setVisible(TRUE);
			widgetCreatePrefix2->setVisible(FALSE);
			//cmdNext->setText(tr("Next >"));
			widgetInfo->setVisible(FALSE);
			break;
					case 4:
			widgetCreatePrefix0->setVisible(FALSE);
			widgetCreatePrefix1->setVisible(FALSE);
			widgetCreatePrefix2->setVisible(TRUE);
			cmdNext->setText(tr("Next >"));
			widgetInfo->setVisible(FALSE);
			break;
					case 5:
			QString info;
			info = tr("<p>Please check parameters listed below before clicking <b>Next</b>:</p><p><b>Prefix name:</b> %1<br><b>Prefix path:</b> %2</p>").arg(txtPrefixName->text()).arg(txtPrefixPath->text());
			if (cbCreafeFake->checkState()==Qt::Checked){
				info.append(tr("<p>Wine fake drive will be created.</p>"));
			}
			if (!txtWineBin->text().isEmpty()){
				info.append(tr("<p><b>Wine Bin</b>: %1").arg(txtWineBin->text()));
			} else {
				info.append(tr("<p><b>Wine Bin</b>: Default"));
			}
			if (!txtWineServerBin->text().isEmpty()){
				info.append(tr("<br><b>Wine Server</b>: %1").arg(txtWineServerBin->text()));
			} else {
				info.append(tr("<br><b>Wine Server</b>: Default"));
			}
			if (!txtWineLoaderBin->text().isEmpty()){
				info.append(tr("<br><b>Wine Loader</b>: %1").arg(txtWineLoaderBin->text()));
			} else {
				info.append(tr("<br><b>Wine Loader</b>: Default"));
			}
			if (!txtWineDllPath->text().isEmpty()){
				info.append(tr("<br><b>Wine Dll Path</b>: %1").arg(txtWineDllPath->text()));
			} else {
				info.append(tr("<br><b>Wine Dll Path</b>: Default"));
			}
			if (!txtMountPoint->text().isEmpty()){
				info.append(tr("<p><b>Quick mount options</b> <br><b>Device</b>: %1<br><b>Mount point</b>: %2</p>").arg(txtMountPoint->text()) .arg(combSourceDevice->currentText()));
			}
			lblWizardInfo->setText(info);
			widgetInfo->setVisible(TRUE);
			widgetCreatePrefix0->setVisible(FALSE);
			widgetCreatePrefix1->setVisible(FALSE);
			widgetCreatePrefix2->setVisible(FALSE);
			cmdNext->setText(tr("Finish"));
			break;
		}
		break;
			case 2:
		/*
					New fake drive creation
				*/
		switch (Page){
					case 1:
			widgetCreateFakeDrive0->setVisible(FALSE);
			widgetInfo->setVisible(TRUE);
			cmdNext->setEnabled(TRUE);
			cmdBack->setEnabled(FALSE);
			break;
					case 2:
			widgetCreateFakeDrive0->setVisible(TRUE);
			widgetCreateFakeDrive1->setVisible(FALSE);
			widgetInfo->setVisible(FALSE);
			cmdNext->setEnabled(TRUE);
			cmdBack->setEnabled(TRUE);
			cmdNext->setText(tr("Next >"));
			break;
					case 3:
			widgetCreateFakeDrive0->setVisible(FALSE);
			widgetCreateFakeDrive1->setVisible(TRUE);
			widgetCreateFakeDrive2->setVisible(FALSE);
			break;
					case 4:
			widgetCreateFakeDrive1->setVisible(FALSE);
			widgetCreateFakeDrive3->setVisible(FALSE);
			widgetCreateFakeDrive2->setVisible(TRUE);
			break;
					case 5:
			widgetCreateFakeDrive2->setVisible(FALSE);
			widgetCreateFakeDrive3->setVisible(TRUE);
			widgetInfo->setVisible(FALSE);
			cmdNext->setText(tr("Next >"));
			break;
					case 6:
			QString info;
			info = tr("<p>All ready for fake drive creation. </p><p>Please, press the <b>Finish</b> button to create facke drive. Or press <b>Back</b> button for return.</p>");
			lblWizardInfo->setText(info);
			widgetInfo->setVisible(TRUE);
			widgetCreateFakeDrive2->setVisible(FALSE);
			widgetCreateFakeDrive3->setVisible(FALSE);
			cmdNext->setText(tr("Finish"));
			break;
		}
		break;
	}
	return;
}


void Wizard::radioDefault_toggled(bool state){
	if (!state)
		return;

#ifdef _OS_LINUX_
	txtMountString->setText("%SUDO% %MOUNT_BIN% %MOUNT_DRIVE% %MOUNT_POINT%");
#endif
#ifdef _OS_FREEBSD_
	txtMountString->setText("%SUDO% %MOUNT_BIN% -t cd9660 %MOUNT_DRIVE% %MOUNT_POINT%");
#endif

#ifdef _OS_LINUX_
	txtMountImageString->setText("%SUDO% %MOUNT_BIN% %MOUNT_OPTIONS% %MOUNT_IMAGE% %MOUNT_POINT%");
#endif
#ifdef _OS_FREEBSD_
	txtMountString->setText("%SUDO% %MOUNT_BIN% -t cd9660 /dev/`%MDCONFIG_BIN% -f %%MOUNT_IMAGE%` %MOUNT_POINT%");
#endif

#ifdef _OS_LINUX_
	txtUmountString->setText("%SUDO% %UMOUNT_BIN% %MOUNT_POINT%");
#endif
#ifdef _OS_FREEBSD_
	QString umount="%SUDO% ";
	umount.append(APP_PREF);
	umount.append("/share/q4wine/scripts/umount_freebsd.sh");
	umount.append(" %UMOUNT_BIN% %MOUNT_POINT%");
	txtUmountString->setText(umount);
#endif

	return;
}

void Wizard::radioDefaultGui_toggled(bool state){
	if (!state)
		return;

#ifdef _OS_LINUX_
	txtMountString->setText("%GUI_SUDO% \"%MOUNT_BIN% %MOUNT_DRIVE% %MOUNT_POINT%\"");
#endif
#ifdef _OS_FREEBSD_
	txtMountString->setText("%GUI_SUDO% \"%MOUNT_BIN% -t cd9660 %MOUNT_DRIVE% %MOUNT_POINT%\"");
#endif

#ifdef _OS_LINUX_
	txtMountImageString->setText("%GUI_SUDO% \"%MOUNT_BIN% %MOUNT_OPTIONS% %MOUNT_IMAGE% %MOUNT_POINT%\"");
#endif
#ifdef _OS_FREEBSD_
	txtMountString->setText("%GUI_SUDO% \"%MOUNT_BIN% -t cd9660 /dev/`%MDCONFIG_BIN% -f %%MOUNT_IMAGE%` %MOUNT_POINT%\"");
#endif

#ifdef _OS_LINUX_
	txtUmountString->setText("%GUI_SUDO% %UMOUNT_BIN% %MOUNT_POINT%");
#endif
#ifdef _OS_FREEBSD_
	QString umount="%GUI_SUDO% ";
	umount.append(APP_PREF);
	umount.append("/share/q4wine/scripts/umount_freebsd.sh");
	umount.append(" %UMOUNT_BIN% %MOUNT_POINT%");
	txtUmountString->setText(umount);
#endif

	return;
}

void Wizard::radioFuse_toggled(bool state){
	if (!state)
		return;

	QString format;
	format=CoreLib->getWhichOut("fuseiso");
	if (format.isEmpty()){
		radioDefault->setChecked(true);
		return;
	}
	format.append(" %MOUNT_DRIVE% %MOUNT_POINT%");
	txtMountString->setText(format);
	format=CoreLib->getWhichOut("fuseiso");
	if (format.isEmpty()){
		radioDefault->setChecked(true);
		return;
	}
	format.append(" %MOUNT_IMAGE% %MOUNT_POINT%");
	txtMountImageString->setText(format);
	format=CoreLib->getWhichOut("fusermount");
	if (format.isEmpty()){
		radioDefault->setChecked(true);
		return;
	}
	format.append(" -u %MOUNT_POINT%");
	txtUmountString->setText(format);

	return;
}

void Wizard::radioEmbedded_toggled(bool state){
	if (!state)
		return;

#ifdef WITH_EMBEDDED_FUSEISO
	QString format;
	format=APP_PREF;
	format.append("/bin/q4wine-mount %MOUNT_DRIVE% %MOUNT_POINT%");
	txtMountString->setText(format);
	format=APP_PREF;
	format.append("/bin/q4wine-mount %MOUNT_IMAGE% %MOUNT_POINT%");
	txtMountImageString->setText(format);
	format=CoreLib->getWhichOut("fusermount");
	if (format.isEmpty()){
		radioDefault->setChecked(true);
		return;
	}
	format.append(" -u %MOUNT_POINT%");
	txtUmountString->setText(format);
#else
	QMessageBox::warning(this, tr("Warning"), tr("<p>q4wine was compiled without embedded FuseIso.</p><p>If you wish to compile q4wine with embedded FuseIso add:</p><p> \"-WITH_EMBEDDED_FUSEISO=ON\" to cmake arguments.</p>"));
	radioDefault->setChecked(true);
#endif
	return;
}

