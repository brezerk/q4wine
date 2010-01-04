/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                            *
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

#include "prefixcontrolwidget.h"

PrefixControlWidget::PrefixControlWidget(QString themeName, QWidget *parent) :
	QWidget(parent)
{
	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		  libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	this->themeName=themeName;
	this->createActions();

	std::auto_ptr<QToolBar> toolBar (new QToolBar(this));
	toolBar->addAction(prefixAdd.get());
	toolBar->addSeparator ();
	toolBar->addAction(prefixImport.get());
	toolBar->addAction(prefixExport.get());
	toolBar->addSeparator ();
	toolBar->addAction(prefixSettings.get());
	toolBar->addSeparator ();
	toolBar->addAction(prefixDelete.get());

	prefixTable.reset(new QTableView(this));
	connect(prefixTable.get(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
	//connect(prefixTable.get(), SIGNAL(rowCountChanged (int, int)), this, SLOT(tableRowCountChanged (int, int)));

	prefixTable->setContextMenuPolicy(Qt::CustomContextMenu);

	std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(toolBar.release());
	layout->addWidget(prefixTable.get());

	model.reset(new QSqlQueryModel());

	prefixTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	prefixTable->setSelectionMode(QAbstractItemView::SingleSelection);

	this->setLayout(layout.release());

	this->updateTableModel();
	return;
}

void PrefixControlWidget::createActions(){
	prefixAdd.reset(new QAction(loadIcon("data/wizard.png"), tr("Create new"), this));
	prefixAdd->setStatusTip(tr("Create new prefix"));
	connect(prefixAdd.get(), SIGNAL(triggered()), this, SLOT(prefixAdd_Click()));

	prefixImport.reset(new QAction(loadIcon("data/down.png"), tr("Import prefix"), this));
	prefixImport->setStatusTip(tr("Import prefix"));
	connect(prefixImport.get(), SIGNAL(triggered()), this, SLOT(prefixImport_Click()));
	prefixImport->setEnabled(FALSE);

	prefixExport.reset(new QAction(loadIcon("data/up.png"), tr("Export prefix"), this));
	prefixExport->setStatusTip(tr("Export prefix"));
	connect(prefixExport.get(), SIGNAL(triggered()), this, SLOT(prefixExport_Click()));
	prefixExport->setEnabled(FALSE);

	prefixDelete.reset(new QAction(loadIcon("data/kill.png"), tr("Delete prefix"), this));
	prefixDelete->setStatusTip(tr("Delete prefix"));
	connect(prefixDelete.get(), SIGNAL(triggered()), this, SLOT(prefixDelete_Click()));
	prefixDelete->setEnabled(FALSE);

	prefixSettings.reset(new QAction(loadIcon("data/configure.png"), tr("Edit prefix settings"), this));
	prefixSettings->setStatusTip(tr("Edit prefix settings"));
	connect(prefixSettings.get(), SIGNAL(triggered()), this, SLOT(prefixSettings_Click()));
	prefixSettings->setEnabled(FALSE);

	menu.reset(new QMenu(this));
	menu->addAction(prefixAdd.get());
	menu->addSeparator();
	menu->addAction(prefixImport.get());
	menu->addAction(prefixExport.get());
	menu->addSeparator();
	menu->addAction(prefixDelete.get());
	menu->addSeparator();
	menu->addAction(prefixSettings.get());

	return;
}

QIcon PrefixControlWidget::loadIcon(QString iconName){
	  // Function tryes to load icon image from theme dir
	  // If it fails -> load default from rsource file
	  QIcon icon;
	  if ((!this->themeName.isEmpty()) and (this->themeName!="Default")){
			icon.addFile(QString("%1/%2").arg(this->themeName).arg(iconName));
			if (icon.isNull()){
				  icon.addFile(QString(":/%1").arg(iconName));
			}
	  } else {
			icon.addFile(QString(":/%1").arg(iconName));
	  }
	  return icon;
}

void PrefixControlWidget::customContextMenuRequested(const QPoint &pos){
	if (prefixTable->currentIndex().isValid()){
		prefixDelete->setEnabled(true);
		prefixImport->setEnabled(true);
		prefixExport->setEnabled(true);
		prefixSettings->setEnabled(true);
	} else {
		prefixDelete->setEnabled(false);
		prefixImport->setEnabled(false);
		prefixExport->setEnabled(false);
		prefixSettings->setEnabled(false);
	}

	menu->exec(QCursor::pos());
	return;
}

void PrefixControlWidget::prefixAdd_Click(){
	// Prefix creation function

	Wizard createPrefixWizard(0);
	if (createPrefixWizard.exec()==QDialog::Accepted){
		this->updateTableModel();
	}

	return;
}

void PrefixControlWidget::updateTableModel(){
	model->setQuery("SELECT name, path FROM prefix");
	model->setHeaderData(0, Qt::Horizontal, tr("Name"));
	model->setHeaderData(1, Qt::Horizontal, tr("Path"));
	prefixTable->setModel(model.get());
	prefixTable->resizeColumnsToContents();
	prefixTable->resizeRowsToContents();
	prefixTable->horizontalHeader()->setStretchLastSection(true);
	return;
}

void PrefixControlWidget::prefixDelete_Click(){
	if (!prefixTable->currentIndex().isValid())
		return;

	if(prefixTable->currentIndex().row()<0)
		return;

	QString prefixName = model->index(prefixTable->currentIndex().row(), 0, QModelIndex()).data().toString();

	if (prefixName.isEmpty())
		return;

	if (prefixName=="Default"){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, you can't delete Default prefix."), QMessageBox::Ok);
			return;
	}

	if(QMessageBox::warning(this, tr("Warning"),	tr("Do you really wish to delete prefix named \"%1\" and all associated icons?").arg(prefixName), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok){
		if (db_icon.delIconsByPrefixName(prefixName))
			if(db_dir.delDir(prefixName))
				db_prefix.delByName(prefixName);

		this->updateTableModel();
		emit(updateDatabaseConnections());
	}

	return;
}

void PrefixControlWidget::prefixImport_Click(){
	/*
		Function for importing preefix from file
	*/
/*
	QString openpath;

	//Getting user selected path for prefix store, if not -- use default
	if (!PREFIX_EI_PATH.isEmpty()){
		openpath.append(PREFIX_EI_PATH);
	} else {
		openpath.append(QDir::homePath());
		openpath.append("/.config/");
		openpath.append(APP_SHORT_NAME);
		openpath.append("/prefixes/");
	}

	QString fileName = QFileDialog::getOpenFileName(this, tr("Select file to import"), openpath , tr("Prefix archive images (*.tbz)"));

	if (!fileName.isEmpty()){
		QDir dir;
		QString targetDir;

		if (tablePrefix->item(tablePrefix->currentRow(), 1)->text().isEmpty()){
			targetDir.clear();
			targetDir.append(HOME_PATH);
			targetDir.append("/.wine/");
		} else {
			targetDir.clear();
			targetDir.append(tablePrefix->item(tablePrefix->currentRow(), 1)->text());
		}

		if (dir.exists(targetDir)){
			if(QMessageBox::warning(this, tr("Warning"), tr("Do you really wish to delete all old prefix files?"), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok){
				QStringList args;
				args << "-rdf";
				args << targetDir;

				Process *exportProcess = new Process(args, CoreLib->getWhichOut("rm"), HOME_PATH, tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));
				if (exportProcess->exec()!=QDialog::Accepted){
					return;
				}
				delete(exportProcess);
			} else {
				return;
			}
		}

		dir.mkdir(targetDir);
		QStringList args;
		args << "-xjf";
		args << fileName;
		args << "-C" << targetDir;

		//Creating process dialog
		Process *exportProcess = new Process(args, TAR_BIN, HOME_PATH, tr("Importing prefix.<br>This can take a while..."), tr("Importing prefix"));
		exportProcess->show();
	}*/
	return;
}

void PrefixControlWidget::prefixSettings_Click(){
	if (!prefixTable->currentIndex().isValid())
		return;

	if(prefixTable->currentIndex().row()<0)
		return;

	QString prefixName = model->index(prefixTable->currentIndex().row(), 0, QModelIndex()).data().toString();

	if (prefixName.isEmpty())
		return;

	PrefixSettings settings(prefixName);
	if (settings.exec()==QDialog::Accepted){
		this->updateTableModel();
		emit(updateDatabaseConnections());
	}
	return;
}

void PrefixControlWidget::prefixExport_Click(){
	/*
	 * Function for exporting preefix to file
	 */
/*
	QString savepath;

	//Getting user selected path for prefix store, if not -- use default
	if (!PREFIX_EI_PATH.isEmpty()){
		savepath.append(PREFIX_EI_PATH);
	} else {
		savepath.append(QDir::homePath());
		savepath.append("/.config/");
		savepath.append(APP_SHORT_NAME);
		savepath.append("/prefixes/");
	}

	//Creating save path
	savepath.append(tablePrefix->item(tablePrefix->currentRow(), 0)->text());
	savepath.append("-");
	savepath.append(QDate::currentDate().toString(Qt::ISODate));
	savepath.append(".tbz");

	//Request user for valid save path
	QString fileName = QFileDialog::getSaveFileName(this, tr("Select file to export"), savepath , tr("Prefix archive images (*.tbz)"));

	if (!fileName.isEmpty()){
		QStringList args;

		args << "-cjf";
		args << fileName;


		QString prefix_path = tablePrefix->item(tablePrefix->currentRow(), 1)->text();

		if (prefix_path.isEmpty()){
			prefix_path.clear();
			prefix_path.append(HOME_PATH);
			prefix_path.append("/.wine/");
		}

		args << "-C";
		args << prefix_path;
		args << "./";

		//Creating process dialog
		Process *exportProcess = new Process(args, TAR_BIN, prefix_path, tr("Exporting %1 prefix.<br>This can take a while...").arg(tablePrefix->item(tablePrefix->currentRow(), 0)->text()), tr("Exporting prefix"));

		exportProcess->show();
	}

	*/
	return;
}
