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

#include "run.h"

Run::Run(QString prefix_name, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);


	this->prefix_name = prefix_name;
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

	loadThemeIcons(CoreLib->getSetting("app", "theme", false).toString());

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdGetProgramBin, SIGNAL(clicked()), this, SLOT(cmdGetProgram_Click()));
	connect(cmdGetWorkDir, SIGNAL(clicked()), this, SLOT(cmdGetWorkDir_Click()));
	connect(cmdAdd, SIGNAL(clicked()), this, SLOT(cmdAdd_Click()));
	connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));

	connect(comboPrefixes, SIGNAL(currentIndexChanged (int)), this, SLOT(comboPrefixes_indexChanged (int)));
	connect(cbUseConsole, SIGNAL(stateChanged(int)), this, SLOT(cbUseConsole_stateChanged(int)));

	connect(twbGeneral, SIGNAL(currentChanged(int)), this, SLOT(ResizeContent(int)));

	cmdGetProgramBin->installEventFilter(this);
	cmdGetWorkDir->installEventFilter(this);

	getPrefixes();

	return;
}



void Run::loadThemeIcons(QString themePath){
	QPixmap pixmap;

	if (!pixmap.load(tr("%1/data/exec.png").arg(themePath))){
		pixmap.load(":data/exec.png");
	}

	lblLogo->setPixmap(pixmap);

	cmdGetProgramBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWorkDir->setIcon(loadIcon("data/folder.png", themePath));

	return;
}


QIcon Run::loadIcon(QString iconName, QString themePath){
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


void Run::cmdCancel_Click(){
	reject();
	return;
}

void Run::cmdAdd_Click(){
	if (!cboxDlls->currentText().isEmpty()){
		twDlls->insertRow (0);
		QTableWidgetItem *newItem = new QTableWidgetItem(cboxDlls->currentText());
		twDlls->setItem(0, 0, newItem);
		newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
		newItem = new QTableWidgetItem(cboxOveride->currentText());
		twDlls->setItem(0, 1, newItem);
		newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
	}

	twDlls->resizeRowsToContents();
	twDlls->resizeColumnsToContents();
	twDlls->horizontalHeader()->setStretchLastSection(TRUE);
	return;
}

void Run::cmdOk_Click(){

	QString override;
	for (int i=1; i<=twDlls->rowCount(); i++){
		override.append(tr("%1=").arg(twDlls->item(i-1, 0)->text()));
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

	QSqlQuery query;
	//FIXME: Remove it please.
	query.prepare("SELECT id FROM prefix WHERE name=:name");
	query.bindValue(":name", comboPrefixes->currentText());
	query.exec();
	query.first();

	execObj.prefixid = query.value(0).toString();

	query.clear();


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




	accept();
	return;
}

bool Run::eventFilter( QObject *object, QEvent *event )
{
   //  firstly, check whether the object is the QTableWidget and if it's a mouse press event
	if (object == twDlls)
		if (event->type() == QEvent::KeyPress)
	{   // if yes, we need to cast the event
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent->key()==Qt::Key_Delete)
			twDlls->removeRow(twDlls->currentRow());

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


	QList<QStringList> result = db_prefix->getFields();
	for (int i = 0; i < result.size(); ++i) {
		comboPrefixes->addItem(result.at(i).at(1));
		if (result.at(i).at(1)==this->prefix_name){
			comboPrefixes->setCurrentIndex ( comboPrefixes->findText(result.at(i).at(1)) );
			if (result.at(i).at(2).isEmpty()){
				prefix_dir = QDir::homePath();
				prefix_dir.append("/.wine/drive_c/");
			} else {
				prefix_dir = result.at(i).at(2);
			}
			getWineDlls(result.at(i).at(3));
		}
	}

	return;
}

void Run::comboPrefixes_indexChanged (int){
	/*
	 * If user select prefix -- rebuild wine dlls list
	 */
	QStringList result = db_prefix->getFieldsByPrefixName(comboPrefixes->currentText());

	if (result.at(1).isEmpty()){
		prefix_dir = QDir::homePath();
		prefix_dir.append("/.wine/drive_c/");
	} else {
		prefix_dir = result.at(1);
	}

	getWineDlls(result.at(2));

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

	QString fileName;
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

	/*
	QList<QUrl> prefix_urls;
	// Adding side bar urls for FileOpen dialogs
	if (QDir(prefix_dir).exists())
	   prefix_urls << QUrl::fromLocalFile(prefix_dir);

	if ((searchPath != prefix_dir) && (QDir(searchPath).exists()))
		prefix_urls << QUrl::fromLocalFile(searchPath);

	prefix_urls << QUrl::fromLocalFile(QDir::homePath());
	prefix_urls << QUrl::fromLocalFile(QDir::rootPath());
	*/

	QFileDialog dialog(this);
	  dialog.setFilter(QDir::Dirs | QDir::Files );
	  dialog.setWindowTitle(tr("Open Exe file"));
	  dialog.setDirectory(searchPath);
	  dialog.setFileMode(QFileDialog::ExistingFile);
	  dialog.setNameFilter(tr("Exe files (*.exe)"));
	  //dialog.setSidebarUrls(prefix_urls);

	 if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		QStringList list1 = fileName.split("/");
		txtProgramBin->setText(fileName);

		QString wrkDir;
		wrkDir = fileName.left(fileName.length() - list1.last().length());
		txtWorkDir->setText(wrkDir);
	}

	return;
}

void Run::cmdGetWorkDir_Click(){
	QString searchPath=prefix_dir;
	QString fileName;

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

	  dialog.setSidebarUrls(prefix_urls);
	 if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		txtWorkDir->setText(fileName);
	}
	return;
}

void Run::cmdHelp_Click(){
	QString rawurl;
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
	}

	CoreLib->openHelpUrl(rawurl);
}

