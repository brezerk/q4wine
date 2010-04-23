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

#include "run.h"

Run::Run(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	// Setup base UI
	setupUi(this);
	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	loadThemeIcons();

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdGetProgramBin, SIGNAL(clicked()), this, SLOT(cmdGetProgram_Click()));
	connect(cmdGetWorkDir, SIGNAL(clicked()), this, SLOT(cmdGetWorkDir_Click()));
	connect(cmdAdd, SIGNAL(clicked()), this, SLOT(cmdAdd_Click()));
	connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
	connect(comboPrefixes, SIGNAL(currentIndexChanged (int)), this, SLOT(comboPrefixes_indexChanged (int)));
	connect(cbUseConsole, SIGNAL(stateChanged(int)), this, SLOT(cbUseConsole_stateChanged(int)));
	connect(twbGeneral, SIGNAL(currentChanged(int)), this, SLOT(ResizeContent(int)));

    QString res = CoreLib->getSetting("advanced", "defaultDesktopSize", false, "").toString();
    if (res.isEmpty()){
        cboxDesktopSize->setCurrentIndex(0);
    } else {
        cboxDesktopSize->setCurrentIndex(cboxDesktopSize->findText(res));
    }

	cmdGetProgramBin->installEventFilter(this);
	cmdGetWorkDir->installEventFilter(this);
	cmdOk->setFocus(Qt::ActiveWindowFocusReason);
	return;
}

void Run::prepare(QString prefix_name, QString wrkdir, QString override, QString winedebug, QString useconsole, QString display, QString cmdargs, QString desktop, int nice, QString exec)
{
	if (!wrkdir.isEmpty())
		txtWorkDir->setText(wrkdir);

	if (!winedebug.isEmpty())
		txtWinedebug->setText(winedebug);

	if (!display.isEmpty())
		txtDisplay->setText(display);

	if (!cmdargs.isEmpty())
		txtCmdArgs->setText(cmdargs);

	txtNice->setValue(nice);

	if (!exec.isEmpty())
		txtProgramBin->setText(exec);

	if (useconsole=="1")
		cbUseConsole->setChecked(TRUE);

	if (!desktop.isEmpty())
		cboxDesktopSize->setCurrentIndex(cboxDesktopSize->findText(desktop));

	if (!override.isEmpty()){
		QStringList overrideS = override.split(";");

		QString overrideorder;

		for (int i=0; i<overrideS.count()-1; i++){

			QStringList list2 = overrideS.at(i).split("=");
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
	}

	this->prefix_name = prefix_name;
	getPrefixes();
	return;
}

void Run::loadThemeIcons(){
	lblLogo->setPixmap(CoreLib->loadPixmap("data/exec.png"));

	cmdGetProgramBin->setIcon(CoreLib->loadIcon("data/folder.png"));
	cmdGetWorkDir->setIcon(CoreLib->loadIcon("data/folder.png"));
	return;
}

void Run::cmdCancel_Click(){
	reject();
	return;
}

void Run::cmdAdd_Click(){
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

void Run::cmdOk_Click(){
	if (txtProgramBin->text().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("No program name specified."));
		return;
	}

	if (!QDir(txtWorkDir->text()).exists()){
        QMessageBox::warning(this, tr("Error"), tr("Selected working directory not exists."));
		return;
	}

	QString override="";
	for (int i=1; i<=twDlls->rowCount(); i++){
		override.append(QString("%1=").arg(twDlls->item(i-1, 0)->text()));
		if (twDlls->item(i-1, 1)->text()==tr("Native"))
			override.append("n;");
		if (twDlls->item(i-1, 1)->text()==tr("Buildin"))
			override.append("b;");
		if (twDlls->item(i-1, 1)->text()==tr("Native, Buildin"))
			override.append("n,b;");
		if (twDlls->item(i-1, 1)->text()==tr("Buildin, Native"))
			override.append("b,n;");
	}

	execObj.execcmd = txtProgramBin->text();
	execObj.prefixid = db_prefix.getId(comboPrefixes->currentText());

	if (cbUseConsole->checkState()==Qt::Checked){
		execObj.useconsole = "1";
	} else {
		execObj.useconsole = "0";
	}
	execObj.cmdargs = txtCmdArgs->text();
	execObj.override = override;
	execObj.winedebug = txtWinedebug->text();
	execObj.display = txtDisplay->text();
	execObj.wrkdir = txtWorkDir->text();
	execObj.nice = txtNice->text();

	if (cboxDesktopSize->currentText()==tr("No virtual desktop")){
		execObj.desktop="";
	} else {
		execObj.desktop=cboxDesktopSize->currentText();
	}

	db_last_run_icon.addIcon(execObj.cmdargs, execObj.execcmd, execObj.override, execObj.winedebug, execObj.useconsole, execObj.display, execObj.wrkdir, execObj.desktop, execObj.nice.toInt());

	accept();
	return;
}

bool Run::eventFilter( QObject *object, QEvent *event )
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

void Run::getWineDlls(QString winelibs_path){
	/*
	 * This function Builds Wine dll list for selected prefix
	 */
	cboxDlls->clear();
	cboxDlls->addItems (CoreLib->getWineDlls(winelibs_path));
	cboxDlls->setMaxVisibleItems (10);

	return;
}

void Run::getPrefixes(){
	/*
	 * Getting prefixes and set default
	 */
    QStringList list = db_prefix.getPrefixList();
    for (int i = 0; i < list.size(); ++i) {
        comboPrefixes->addItem(list.at(i));
        if (list.at(i)==this->prefix_name){
            comboPrefixes->setCurrentIndex ( comboPrefixes->findText(list.at(i)) );
            prefix_dir = db_prefix.getPath(list.at(i));
            getWineDlls(db_prefix.getLibsPath(list.at(i)));
		}
	}

	return;
}

void Run::comboPrefixes_indexChanged (int){
	/*
	 * If user select prefix -- rebuild wine dlls list
	 */
    QHash<QString,QString> result = db_prefix.getByName(comboPrefixes->currentText());

    if (result.value("path").isEmpty()){
		prefix_dir = QDir::homePath();
		prefix_dir.append("/.wine/drive_c/");
	} else {
        prefix_dir = result.value("path");
	}

    getWineDlls(result.value("libs"));

	return;
}

/*************************************************************\
|		CheckBox Events	    					  |
\*************************************************************/

void Run::cbUseConsole_stateChanged(int){
	switch(cbUseConsole->checkState()){
 case Qt::Checked:
		txtWinedebug->setEnabled(TRUE);
		break;
 default:
		txtWinedebug->setEnabled(FALSE);
		break;
	}

	return;
}

void Run::ResizeContent(int TabIndex){
	switch (TabIndex){
 case 1:
		twDlls->resizeRowsToContents();
		twDlls->resizeColumnsToContents();
		twDlls->horizontalHeader()->setStretchLastSection(TRUE);
		break;
	}

	return;
}

void Run::cmdGetProgram_Click(){

	QString fileName="";
	QString searchPath=prefix_dir;

	if (!txtProgramBin->text().isEmpty()){
		if (!txtWorkDir->text().isEmpty()){
			searchPath=txtWorkDir->text();
		} else {
			searchPath=txtProgramBin->text().left(txtProgramBin->text().length() - txtProgramBin->text().split("/").last().length());;
		}
	}

	if ((!QDir(searchPath).exists()) or (searchPath.isEmpty())){
		if (QDir(prefix_dir).exists()){
			searchPath=prefix_dir;
		} else {
			searchPath=QDir::homePath();
		}
	}

	QFileDialog dialog(this);
	dialog.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden);
	dialog.setWindowTitle(tr("Open Exe file"));
	dialog.setDirectory(searchPath);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(tr("Exe files (*.exe)"));
	//dialog.setSidebarUrls(prefix_urls);

#ifdef _QT45_AVALIBLE_
      if (CoreLib->getSetting("advanced", "dontUseNativeFileDialog", false, 0)==1){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

	if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		QStringList list1 = fileName.split("/");
		txtProgramBin->setText(fileName);

		QString wrkDir="";
		wrkDir = fileName.left(fileName.length() - list1.last().length());
		txtWorkDir->setText(wrkDir);
	}

	return;
}

void Run::cmdGetWorkDir_Click(){
	QString searchPath=prefix_dir;
	QString fileName="";

	if ((!txtProgramBin->text().isEmpty()) and (QDir().exists(txtProgramBin->text()))){
		searchPath=txtProgramBin->text().left(txtProgramBin->text().length() - txtProgramBin->text().split("/").last().length());;
	}

	if (!QDir(searchPath).exists()){
		if (QDir(prefix_dir).exists()){
			searchPath=prefix_dir;
		} else {
			searchPath=QDir::homePath();
		}
	}

	QList<QUrl> prefix_urls;
	// Adding side bar urls for FileOpen dialogs
	if (QDir(prefix_dir).exists())
		prefix_urls << QUrl::fromLocalFile(prefix_dir);

	if ((searchPath != prefix_dir) && (QDir(searchPath).exists()))
		prefix_urls << QUrl::fromLocalFile(searchPath);

	prefix_urls << QUrl::fromLocalFile(QDir::homePath());
	prefix_urls << QUrl::fromLocalFile(QDir::rootPath());

	QFileDialog dialog(this);
	dialog.setFilter(QDir::Dirs | QDir::Hidden);

	dialog.setFileMode(QFileDialog::Directory);
	dialog.setWindowTitle(tr("Open Directory"));
	dialog.setDirectory(searchPath);
	// This option wirksonly it qt 4.5. In fact this not works correctly with QDir::Hidden,  so I comment it out for a some  time
	//dialog.setOption(QFileDialog::ShowDirsOnly, true);

#ifdef _QT45_AVALIBLE_
      if (CoreLib->getSetting("advanced", "dontUseNativeFileDialog", false, 0)==1){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

	dialog.setSidebarUrls(prefix_urls);
	if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		txtWorkDir->setText(fileName);
	}
	return;
}

void Run::cmdHelp_Click(){
	QString rawurl="";
	switch (twbGeneral->currentIndex()){
 case 0:
		rawurl = "12-run-dialog.html#general";
		break;
 case 1:
		rawurl = "12-run-dialog.html#override";
		break;
 case 2:
		rawurl = "12-run-dialog.html#advanced";
		break;
 default:
		return;
		break;
	}

	CoreLib->openHelpUrl(rawurl);
}

