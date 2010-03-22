/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                                 *
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
 case 0:
		// Prefix creation
		lblPicture->setPixmap(CoreLib->loadPixmap("data/prefixc.png"));

		cmdGetWineBin->setIcon(CoreLib->loadIcon("data/folder.png"));
		cmdGetWineServerBin->setIcon(CoreLib->loadIcon("data/folder.png"));
		cmdGetWineLoaderBin->setIcon(CoreLib->loadIcon("data/folder.png"));
		cmdGetWineDllPath->setIcon(CoreLib->loadIcon("data/folder.png"));
		cmdGetPrefixPath->setIcon(CoreLib->loadIcon("data/folder.png"));
		cmdGetMountPoint->setIcon(CoreLib->loadIcon("data/folder.png"));

		break;
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

#ifndef WITH_ICOUTILS
		cmdGetWrestoolBin->setEnabled(false);
		cmdGetIcotoolBin->setEnabled(false);
		txtWrestoolBin->setEnabled(false);
		txtIcotoolBin->setEnabled(false);
#endif

		connect(radioDefault, SIGNAL(toggled(bool)), this, SLOT(radioDefault_toggled(bool)));
		connect(radioDefaultGui, SIGNAL(toggled(bool)), this, SLOT(radioDefaultGui_toggled(bool)));
		connect(radioFuse, SIGNAL(toggled(bool)), this, SLOT(radioFuse_toggled(bool)));
		connect(radioEmbedded, SIGNAL(toggled(bool)), this, SLOT(radioEmbedded_toggled(bool)));

#ifdef _OS_FREEBSD_
		radioFuse->setEnabled(FALSE);
		radioEmbedded->setEnabled(FALSE);
#endif

		if (CoreLib->getWhichOut("fuseiso", false).isEmpty()){
#ifdef WITH_EMBEDDED_FUSEISO
			radioEmbedded->setChecked(true);
#else
			radioDefaultGui->setChecked(true);
			radioDefault->setChecked(true);
#endif
		} else {
			radioFuse->setChecked(true);
		}

		break;
	case 2:
		// Fake drive create
		lblPicture->setPixmap(CoreLib->loadPixmap("data/firstc.png"));

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

		break;
	case 3:
		// Fake drive update
		lblPicture->setPixmap(CoreLib->loadPixmap("data/firstc.png"));

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
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	widgetCreatePrefix0->setVisible(FALSE);

	Scena=WizardType;
	Page=1;
	QString console_w;

	loadThemeIcons(Scena);

	QString pic="", line="", prefixPath="";
	std::auto_ptr<DriveListWidgetItem> item;

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

		txtGuiSudoBin->setText(CoreLib->getWhichOut("kdesu", FALSE));
		if (txtGuiSudoBin->text().isEmpty()){
			txtGuiSudoBin->setText(CoreLib->getWhichOut("gksudo", FALSE));
			if (txtGuiSudoBin->text().isEmpty()){
				txtGuiSudoBin->setText(CoreLib->getWhichOut("gksu", FALSE));
				if (txtGuiSudoBin->text().isEmpty()){
					txtGuiSudoBin->setText(CoreLib->getWhichOut("sudo", FALSE));
				}
			}
		}

		txtNiceBin->setText(CoreLib->getWhichOut("nice"));
		txtReniceBin->setText(CoreLib->getWhichOut("renice"));
		txtShBin->setText(CoreLib->getWhichOut("sh"));

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

#ifdef WITH_ICOUTILS
		txtWrestoolBin->setText(CoreLib->getWhichOut("wrestool"));
		txtIcotoolBin->setText(CoreLib->getWhichOut("icotool"));
		cmdGetWrestoolBin->installEventFilter(this);
		cmdGetIcotoolBin->installEventFilter(this);
#endif

		break;
	case 2:
		TotalPage=8;
		this->var1=var1;
		setWindowTitle(tr("Fake drive creation wizard"));
		lblCaption->setText(tr("<b>Fake drive creation wizard</b>"));
		lblStep->setText(tr("<b>Step %1 of %2</b>").arg(Page).arg(TotalPage));
		lblWizardInfo->setText(Wizard::tr("<p>Welcome to fake drive creation wizard.</p><p>This wizard helps you to make all necessary steps for successful fake drive creation.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>"));

		cmdGetWineDesktop->installEventFilter(this);
		cmdGetWineDesktopDoc->installEventFilter(this);
		cmdGetWineDesktopPic->installEventFilter(this);
		cmdGetWineDesktopMus->installEventFilter(this);
		cmdGetWineDesktopVid->installEventFilter(this);

		item.reset(new DriveListWidgetItem(listWineDrives));
		item->setDrive("C:", "../drive_c", "auto");
		listWineDrives->addItem(item.release());

        if (!db_prefix.getMountPoint(var1).isEmpty()){
			item.reset(new DriveListWidgetItem(listWineDrives));
            item->setDrive("D:", db_prefix.getMountPoint(var1), "cdrom");
			listWineDrives->addItem(item.release());
		}

		item.reset(new DriveListWidgetItem(listWineDrives));
		item->setDrive("Z:", "/", "auto");
		listWineDrives->addItem(item.release());

		item.reset(new DriveListWidgetItem(listWineDrives));
		item->setDrive("H:", QString("%1/.config/q4wine/tmp").arg(QDir::homePath()), "auto");
		listWineDrives->addItem(item.release());

                txtWineDesktop->setText(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
		txtWineDesktopDoc->setText(QDir::homePath());
		txtWineDesktopMus->setText(QDir::homePath());
		txtWineDesktopPic->setText(QDir::homePath());
		txtWineDesktopVid->setText(QDir::homePath());

		break;
	case 3:
		TotalPage=10;
		this->var1=var1;

		prefixPath = db_prefix.getPath(var1);
		if (prefixPath.isEmpty()){
			qDebug()<<" [EE] Cant get prefix path: "<<var1;
			return;
		}

		Registry reg(prefixPath);

		QStringList list;
		list << "\"RegisteredOrganization\"" << "\"RegisteredOwner\"";
		list = reg.readKeys("system", "Software\\Microsoft\\Windows NT\\CurrentVersion", list);
		//HKEY_CURRENT_USER\\Software\\Wine]\n\"Version

		if (list.count()>0){
			txtOrganization->setText(list.at(0));
			txtOwner->setText(list.at(1));
		}

		list.clear();
		list << "\"Browsers\"" << "\"Mailers\"";
		list = reg.readKeys("user", "Software\\Wine\\WineBrowser", list);

		if (list.count()>0){
			txtFakeBrowsers->setText(list.at(0));
			txtFakeMailers->setText(list.at(1));
		}

		list.clear();
		list << "\"Multisampling\"" << "\"DirectDrawRenderer\"" << "\"RenderTargetLockMode\"" << "\"OffscreenRenderingMode\"" << "\"UseGLSL\"" << "\"VideoMemorySize\"" << "\"VideoDescription\"" << "\"VideoDriver\"" << "\"SoftwareEmulation\"" << "\"PixelShaderMode\"" << "\"VertexShaderMode\"";
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
			QFileInfo fileinfo(QString("%1/Desktop").arg(prefixPath));
			if (fileinfo.isSymLink()){
				txtWineDesktop->setText(fileinfo.symLinkTarget());
			} else {
				txtWineDesktop->setText(fileinfo.filePath());
			}

			fileinfo.setFile(QString("%1/My Documents").arg(prefixPath));
			if (fileinfo.isSymLink()){
				txtWineDesktopDoc->setText(fileinfo.symLinkTarget());
			} else {
				txtWineDesktopDoc->setText(fileinfo.filePath());
			}

			fileinfo.setFile(QString("%1/My Music").arg(prefixPath));
			if (fileinfo.isSymLink()){
				txtWineDesktopMus->setText(fileinfo.symLinkTarget());
			} else {
				txtWineDesktopMus->setText(fileinfo.filePath());
			}

			fileinfo.setFile(QString("%1/My Pictures").arg(prefixPath));
			if (fileinfo.isSymLink()){
				txtWineDesktopPic->setText(fileinfo.symLinkTarget());
			} else {
				txtWineDesktopPic->setText(fileinfo.filePath());
			}

			fileinfo.setFile(QString("%1/My Videos").arg(prefixPath));
			if (fileinfo.isSymLink()){
				txtWineDesktopVid->setText(fileinfo.symLinkTarget());
			} else {
				txtWineDesktopVid->setText(fileinfo.filePath());
			}
		}

		setWindowTitle(tr("Fake drive update wizard"));
		lblCaption->setText(tr("<b>Fake drive update wizard</b>"));
		lblStep->setText(tr("<b>Step %1 of %2</b>").arg(Page).arg(TotalPage));
		lblWizardInfo->setText(Wizard::tr("<p>Welcome to fake drive update wizard.</p><p>This wizard helps you to make all necessary steps for successful fake drive creation.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>"));
		Scena=2;

		cmdGetWineDesktop->installEventFilter(this);
		cmdGetWineDesktopDoc->installEventFilter(this);
		cmdGetWineDesktopPic->installEventFilter(this);
		cmdGetWineDesktopMus->installEventFilter(this);
		cmdGetWineDesktopVid->installEventFilter(this);
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
	widgetCreateFakeDrive4->setVisible(FALSE);
	widgetCreateFakeDrive5->setVisible(FALSE);
	widgetCreateFakeDrive6->setVisible(FALSE);
	widgetCreateFakeDrive7->setVisible(FALSE);


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


QString Wizard::getPrefixName(){
	return txtPrefixName->text();
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
		this->widgetCreateFakeDrive0->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive1->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive2->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive3->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive4->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive5->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive6->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreateFakeDrive7->resize(this->widgetFrame->width(), this->widgetFrame->height());
		this->widgetCreatePrefix0->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreatePrefix1->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetCreatePrefix2->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup0->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup1->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup2->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup3->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		this->widgetFirstStartup4->resize(this->widgetFrame->width()+10, this->widgetFrame->height());
		return FALSE;
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
	QRegExp rx("^\".*\"=\".*\"$");
	QList<QListWidgetItem *> listItems = listJoystickAxesMappings->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
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
			if (radioDefault->isChecked()){
				settings.setValue("type", 0);
				if (txtMountString->text().isEmpty()){
					txtMountString->setText(CoreLib->getMountString(0));
				}

				if (txtMountImageString->text().isEmpty()){
					txtMountImageString->setText(CoreLib->getMountImageString(0));
				}

				if (txtUmountString->text().isEmpty()){
					txtUmountString->setText(CoreLib->getUmountString(0));
				}

			}

			if (radioDefaultGui->isChecked()){
				settings.setValue("type", 1);
				if (txtMountString->text().isEmpty()){
					txtMountString->setText(CoreLib->getMountString(1));
				}

				if (txtMountImageString->text().isEmpty()){
					txtMountImageString->setText(CoreLib->getMountImageString(1));
				}

				if (txtUmountString->text().isEmpty()){
					txtUmountString->setText(CoreLib->getUmountString(1));
				}
			}

			if (radioFuse->isChecked()){
				settings.setValue("type", 2);
				QString format;
				if (txtMountString->text().isEmpty()){
					txtMountString->setText(CoreLib->getMountString(2));
				}

				if (txtMountImageString->text().isEmpty()){
					txtMountImageString->setText(CoreLib->getMountImageString(2));
				}

				if (txtUmountString->text().isEmpty()){
					txtUmountString->setText(CoreLib->getUmountString(2));
				}
			}
			if (radioEmbedded->isChecked()){
				QString format;
				settings.setValue("type", 3);
				if (txtMountString->text().isEmpty()){
					txtMountString->setText(CoreLib->getMountString(3));
				}

				if (txtMountImageString->text().isEmpty()){
					txtMountImageString->setText(CoreLib->getMountImageString(3));
				}

				if (txtUmountString->text().isEmpty()){
					txtUmountString->setText(CoreLib->getUmountString(3));
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
				if (db_prefix.isExistsByName(txtPrefixName->text())){
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
			if (!db_prefix.addPrefix(txtPrefixName->text(), txtPrefixPath->text(), txtWineBin->text(), txtWineServerBin->text(), txtWineLoaderBin->text(), txtWineDllPath->text(), txtMountPoint->text(), combSourceDevice->currentText()))
				reject();
			if (cbCreafeFake->checkState()==Qt::Checked){
				Wizard createFakeDriveWizard(2, txtPrefixName->text());
				createFakeDriveWizard.exec();
			}
			accept();
			break;
		}
		break;
	case 2:
		switch (Page){
  case 6:
			for (int i=0; i < listItems.count(); i++){
				if (rx.indexIn(listItems.at(i)->text())!=0){
					QMessageBox::warning(this, tr("Error"), tr("Error in string:\n\n%1\n\nJoystick axes mappings might be defined as:\n\"Joystick name\"=\"axes mapping\"\n\nFor example:\n\"Logitech Logitech Dual Action\"=\"X,Y,Rz,Slider1,POV1\"\n\nSee help for details.").arg(listItems.at(i)->text()));
					return;
				}
			}
			break;
			case 8:
			if (listWineDrives->count()>0){
				QString tmppath=QDir::homePath();
				bool tmpexists=FALSE;
				tmppath.append("/.config/q4wine/tmp");
				for (int i=0; i<listWineDrives->count(); i++){
					QString path = listWineDrives->item(i)->text().split("\n").at(0).split(":").at(1).trimmed();

					if (path==tmppath){
						tmpexists=TRUE;
						break;
					}
				}
				if (!tmpexists){
					QMessageBox::warning(this, tr("Warning"), tr("Can't find Wine Drive which is point to:\n\"%1\"\n\nMake shure wine can access %2 temp directory.").arg(tmppath).arg(APP_SHORT_NAME));
				}
			}
			break;
		case 9:
			if (!checkEntry(txtWineDesktop->text(), "Desktop", FALSE))
				return;

			if (!checkEntry(txtWineDesktopDoc->text(), "My Documents", FALSE))
				return;

			if (!checkEntry(txtWineDesktopPic->text(), "My Pictures", FALSE))
				return;

			if (!checkEntry(txtWineDesktopMus->text(), "My Music", FALSE))
				return;

			if (!checkEntry(txtWineDesktopVid->text(), "My Videos", FALSE))
				return;

			break;
			case 10:
			QApplication::setOverrideCursor( Qt::BusyCursor );

			QString prefix_name = var1;
			QString prefix_id="";

			//Getting versions
			QString version;
            if (comboFakeVersion->currentText()=="Windows XP"){
				version = "winxp";
            } else if (comboFakeVersion->currentText()=="Windows 2008"){
				version = "win2008";
            } else if (comboFakeVersion->currentText()=="Windows 7"){
				version = "vista";
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
            }

            ExecObject execObj;
            execObj.cmdargs = "-u -i";
            execObj.execcmd = "wineboot";

            if (!CoreLib->runWineBinary(execObj, prefix_name, false)){
                QApplication::restoreOverrideCursor();
                reject();
                return;
            }

			QString sh_cmd = "";
			QStringList sh_line;

			QDir wineDriveDir;
			wineDriveDir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot  );

			QString prefixPath = db_prefix.getPath(var1);
			prefixPath.append("/dosdevices/");

			if (!wineDriveDir.cd(prefixPath)){
				qDebug()<<"Cannot cd to prefix directory: "<<prefixPath;
			} else {
				QFileInfoList drivelist = wineDriveDir.entryInfoList();
				for (int i = 0; i < drivelist.size(); ++i) {
					QFileInfo fileInfo = drivelist.at(i);
					if (fileInfo.isSymLink()){
						sh_cmd.clear();
						sh_cmd.append(CoreLib->getWhichOut("rm"));
						sh_cmd.append(" -fr '");
						sh_cmd.append(fileInfo.filePath());
						sh_cmd.append("'");
						sh_line.append(sh_cmd);
					}
				}
			}

			sh_cmd.clear();
            sh_cmd.append("cd \'");
			sh_cmd.append(prefixPath);
            sh_cmd.append("\'");
			sh_line.append(sh_cmd);

			if (listWineDrives->count()>0){
				for (int i=0; i<listWineDrives->count(); i++){
					std::auto_ptr<DriveListWidgetItem> item (dynamic_cast<DriveListWidgetItem*>(listWineDrives->item(i)));

					if (item.get()){
                        sh_cmd.clear();
                        sh_cmd.append(CoreLib->getWhichOut("rm"));
                        sh_cmd.append(" -f '");
                        sh_cmd.append(item->getLetter().toLower());
                        sh_cmd.append("'");
                        sh_line.append(sh_cmd);

						sh_cmd.clear();
						sh_cmd.append(CoreLib->getWhichOut("ln"));
						sh_cmd.append(" -s '");
						sh_cmd.append(item->getPath());
						sh_cmd.append("' '");
						sh_cmd.append(item->getLetter().toLower());
						sh_cmd.append("'");
						sh_line.append(sh_cmd);
					}
					item.release();
				}
			}

			prefixPath.append("c:/users/");
			prefixPath.append(getenv("USER"));

			sh_cmd.clear();
            sh_cmd.append("mkdir -p \'");
			sh_cmd.append(prefixPath);
            sh_cmd.append("\'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("rm"));
			sh_cmd.append(" -fr '");
			sh_cmd.append(QString("%1/Desktop").arg(prefixPath));
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("rm"));
			sh_cmd.append(" -fr '");
			sh_cmd.append(QString("%1/My Documents").arg(prefixPath));
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("rm"));
			sh_cmd.append(" -fr '");
			sh_cmd.append(QString("%1/My Music").arg(prefixPath));
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("rm"));
			sh_cmd.append(" -fr '");
			sh_cmd.append(QString("%1/My Pictures").arg(prefixPath));
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("rm"));
			sh_cmd.append(" -fr '");
			sh_cmd.append(QString("%1/My Videos").arg(prefixPath));
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
            sh_cmd.append("cd \'");
			sh_cmd.append(prefixPath);
            sh_cmd.append("\'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("ln"));
			sh_cmd.append(" -s '");
			sh_cmd.append(txtWineDesktop->text());
			sh_cmd.append("' '");
			sh_cmd.append("Desktop");
			sh_cmd.append("'");
            sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("ln"));
			sh_cmd.append(" -s '");
			sh_cmd.append(txtWineDesktopDoc->text());
			sh_cmd.append("' '");
			sh_cmd.append("My Documents");
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("ln"));
			sh_cmd.append(" -s '");
			sh_cmd.append(txtWineDesktopMus->text());
			sh_cmd.append("' '");
			sh_cmd.append("My Music");
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("ln"));
			sh_cmd.append(" -s '");
			sh_cmd.append(txtWineDesktopPic->text());
			sh_cmd.append("' '");
			sh_cmd.append("My Pictures");
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();
			sh_cmd.append(CoreLib->getWhichOut("ln"));
			sh_cmd.append(" -s '");
			sh_cmd.append(txtWineDesktopVid->text());
			sh_cmd.append("' '");
			sh_cmd.append("My Videos");
			sh_cmd.append("'");
			sh_line.append(sh_cmd);

			sh_cmd.clear();

#ifdef DEBUG
			qDebug()<<"[ii] Wizard::Creating sh_line";
#endif
			for (int i=0; i<sh_line.count(); i++){
				sh_cmd.append(sh_line.at(i));
				if (i!=(sh_line.count()-1))
					sh_cmd.append(" && ");
			}

			QStringList args;
			args<<"-c"<<sh_cmd;

#ifdef DEBUG
			qDebug()<<"[ii] Wizard::Updateing wine dosdrives";
#endif


			Process proc(args, CoreLib->getWhichOut("sh"), QDir::homePath(), tr("Updateing wine dosdrives"), tr("Updateing wine dosdrives"), true);
			proc.exec();

			// ---- End of Creating Dos drives ----

			Registry registry;

			if (registry.init()){
				registry.set("Software\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOrganization", txtOrganization->text(), "HKEY_LOCAL_MACHINE");
				registry.set("Software\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOwner", txtOwner->text(), "HKEY_LOCAL_MACHINE");

				registry.set("Software\\Wine", "Version", version);

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

				if (registry.exec(this, prefix_name)){

#ifdef DEBUG
			qDebug()<<"[ii] Wizard::creating icons";
#endif

					QString dir_id;
					//Is settings directory exists?
					if (!db_dir.isExistsByName(prefix_name, "system")){
						db_dir.addDir(prefix_name, "system");
						//Adding icons
						db_icon.addIcon("", "winecfg.exe", "winecfg", "Configure the general settings for Wine", prefix_name, "system", "winecfg");
						db_icon.addIcon("--backend=user cmd", "wineconsole", "wineconsole", "Wineconsole is similar to wine command wcmd", prefix_name, "system", "console");
						db_icon.addIcon("", "uninstaller.exe", "uninstaller", "Uninstall Windows programs under Wine properly", prefix_name, "system", "uninstaller");
						db_icon.addIcon("", "regedit.exe", "regedit", "Wine registry editor", prefix_name, "system", "regedit");
						db_icon.addIcon("", "explorer.exe", "explorer", "Browse the files in the virtual Wine drive", prefix_name, "system", "explorer");
						db_icon.addIcon("", "eject.exe", "eject", "Wine CD eject tool", prefix_name, "system", "eject");
						db_icon.addIcon("", "wordpad.exe", "wordpad", "Wine wordpad text editor", prefix_name, "system", "wordpad");
					}

					if (!db_dir.isExistsByName(prefix_name, "autostart"))
						db_dir.addDir(prefix_name, "autostart");

					QApplication::restoreOverrideCursor();

#ifdef DEBUG
			qDebug()<<"[ii] Wizard::done";
#endif

					accept();
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
  case 7:
			rawurl = "03-first-startup-wizard.html#qmount";
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
			rawurl = "07-fake-drive-wizard.html#video";
			break;
  case 6:
			rawurl = "07-fake-drive-wizard.html#input";
			break;
  case 7:
			rawurl = "07-fake-drive-wizard.html#x11";
			break;
  case 8:
			rawurl = "07-fake-drive-wizard.html#winedrive";
			break;
  case 9:
			rawurl = "07-fake-drive-wizard.html#desktop";
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
			break;
  case 3:
			widgetCreatePrefix0->setVisible(FALSE);
			widgetCreatePrefix1->setVisible(TRUE);
			widgetCreatePrefix2->setVisible(FALSE);
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

			QString info = tr("<p>Please check parameters listed below before clicking <b>Next</b>:</p><p><b>Prefix name:</b> %1<br><b>Prefix path:</b> %2</p>").arg(txtPrefixName->text()).arg(txtPrefixPath->text());
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
				*	New fake drive creation
				*/
		switch (Page){
  case 1:
			lblWizardInfo->setText(tr("<p>Welcome to fake drive creation wizard.</p><p>This wizard helps you to make all necessary steps for successful fake drive creation.</p><p>Please, press the <b>Next</b> button to go to the next wizard's page. Or press <b>Back</b> button for return.</p>"));
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
			widgetCreateFakeDrive4->setVisible(FALSE);
			break;
  case 6:
			widgetCreateFakeDrive3->setVisible(FALSE);
			widgetCreateFakeDrive4->setVisible(TRUE);
			widgetCreateFakeDrive5->setVisible(FALSE);
			break;
  case 7:
			widgetCreateFakeDrive4->setVisible(FALSE);
			widgetCreateFakeDrive5->setVisible(TRUE);
			widgetCreateFakeDrive6->setVisible(FALSE);
			break;
  case 8:
			widgetCreateFakeDrive5->setVisible(FALSE);
			widgetCreateFakeDrive6->setVisible(TRUE);
			widgetCreateFakeDrive7->setVisible(FALSE);
			break;
  case 9:
			widgetCreateFakeDrive6->setVisible(FALSE);
			widgetCreateFakeDrive7->setVisible(TRUE);
			widgetInfo->setVisible(FALSE);
			cmdNext->setText(tr("Next >"));
			break;
  case 10:
			lblWizardInfo->setText(tr("<p>All ready for fake drive creation. </p><p>Please, press the <b>Finish</b> button to create facke drive. Or press <b>Back</b> button for return.</p>"));
			widgetInfo->setVisible(TRUE);
			widgetCreateFakeDrive6->setVisible(FALSE);
			widgetCreateFakeDrive7->setVisible(FALSE);
			cmdNext->setText(tr("Finish"));
			break;
		}
		break;
	}

	cmdNext->setFocus(Qt::ActiveWindowFocusReason);

	return;
}

void Wizard::radioDefault_toggled(bool state){
	if (!state)
		return;

	txtMountString->setText(CoreLib->getMountString(0));
	txtMountImageString->setText(CoreLib->getMountImageString(0));
	txtUmountString->setText(CoreLib->getUmountString(0));
	return;
}

void Wizard::radioDefaultGui_toggled(bool state){
	if (!state)
		return;

	txtMountString->setText(CoreLib->getMountString(1));
	txtMountImageString->setText(CoreLib->getMountImageString(1));
	txtUmountString->setText(CoreLib->getUmountString(1));
	return;
}

void Wizard::radioFuse_toggled(bool state){
	if (!state)
		return;

	if (CoreLib->getWhichOut("fusermount").isEmpty()){
		radioDefault->setChecked(true);
		return;
	}
	if (CoreLib->getWhichOut("fuseiso").isEmpty()){
		radioDefault->setChecked(true);
		return;
	}

	txtMountString->setText(CoreLib->getMountString(2));
	txtMountImageString->setText(CoreLib->getMountImageString(2));
	txtUmountString->setText(CoreLib->getUmountString(2));
	return;
}

void Wizard::radioEmbedded_toggled(bool state){
	if (!state)
		return;

#ifdef WITH_EMBEDDED_FUSEISO
	if (CoreLib->getWhichOut("fusermount").isEmpty()){
		radioDefault->setChecked(true);
		return;
	}

	txtMountString->setText(CoreLib->getMountString(3));
	txtMountImageString->setText(CoreLib->getMountImageString(3));
	txtUmountString->setText(CoreLib->getUmountString(3));
#else
	QMessageBox::warning(this, tr("Warning"), tr("<p>q4wine was compiled without embedded FuseIso.</p><p>If you wish to compile q4wine with embedded FuseIso add:</p><p> \"-WITH_EMBEDDED_FUSEISO=ON\" to cmake arguments.</p>"));
	radioDefault->setChecked(true);
#endif
	return;
}

void Wizard::cmdJoystickEdit_Click(){
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

void Wizard::cmdJoystickAdd_Click(){
	bool ok;
	QString text = QInputDialog::getText(this, tr("Joystick Axes Mappings"), tr("Joystick axes mappings might be defined as:\n\"Joystick name\"=\"axes mapping\"\n\nFor example:\n\"Logitech Logitech Dual Action\"=\"X,Y,Rz,Slider1,POV1\"\n\nSee help for details."), QLineEdit::Normal, "", &ok);
	if (ok && !text.isEmpty()){
		listJoystickAxesMappings->addItem(text);
	}
	return;
}

void Wizard::cmdJoystickDel_Click(){
	std::auto_ptr<QListWidgetItem> item (listJoystickAxesMappings->currentItem());
	if (!item.get())
		return;

	delete item.release();
	return;
}

void Wizard::cmdWineDriveEdit_Click(){
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

void Wizard::cmdWineDriveAdd_Click(){
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

void Wizard::cmdWineDriveDel_Click(){
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
