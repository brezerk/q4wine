/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

PrefixControlWidget::PrefixControlWidget(QWidget *parent) :
    QWidget(parent)
{
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

    this->createActions();

    std::auto_ptr<QToolBar> toolBar (new QToolBar(this));
    toolBar->setIconSize(QSize(24, 24));
    toolBar->addAction(prefixAdd.get());
    toolBar->addSeparator ();
    toolBar->addAction(prefixImport.get());
    toolBar->addAction(prefixExport.get());
    toolBar->addSeparator ();
    toolBar->addAction(prefixDelete.get());
    toolBar->addSeparator ();
    toolBar->addAction(prefixSettings.get());
    toolBar->addSeparator ();
    toolBar->addAction(prefixSetup.get());
    toolBar->addAction(versionsSetup.get());

    prefixTable.reset(new QTableView(this));
    connect(prefixTable.get(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    connect(prefixTable.get(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)));

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

void PrefixControlWidget::setDefaultFocus(QString prefixName){
    if (prefixName.isEmpty())
        return;

    int i=0;
    while (1){
        QModelIndex newIndex  = model->index(i, 0);
        if (!newIndex.isValid())
            break;

        if (newIndex.data().toString()==prefixName){
            prefixTable->setCurrentIndex(newIndex);
            prefixDelete->setEnabled(true);
            prefixImport->setEnabled(true);
            prefixExport->setEnabled(true);
            prefixSettings->setEnabled(true);
            prefixSetup->setEnabled(true);
            break;
        }
        i++;
    }
    return;
}

void PrefixControlWidget::createActions(){
    prefixAdd.reset(new QAction(CoreLib->loadIcon("document-new"), tr("Create new"), this));
    prefixAdd->setStatusTip(tr("Create new prefix"));
    connect(prefixAdd.get(), SIGNAL(triggered()), this, SLOT(prefixAdd_Click()));

    prefixImport.reset(new QAction(CoreLib->loadIcon("document-import"), tr("Import prefix"), this));
    prefixImport->setStatusTip(tr("Import prefix"));
    connect(prefixImport.get(), SIGNAL(triggered()), this, SLOT(prefixImport_Click()));
    prefixImport->setEnabled(false);

    prefixExport.reset(new QAction(CoreLib->loadIcon("document-export"), tr("Export prefix"), this));
    prefixExport->setStatusTip(tr("Export prefix"));
    connect(prefixExport.get(), SIGNAL(triggered()), this, SLOT(prefixExport_Click()));
    prefixExport->setEnabled(false);

    prefixDelete.reset(new QAction(CoreLib->loadIcon("edit-delete"), tr("Delete prefix"), this));
    prefixDelete->setStatusTip(tr("Delete prefix"));
    connect(prefixDelete.get(), SIGNAL(triggered()), this, SLOT(prefixDelete_Click()));
    prefixDelete->setEnabled(false);

    prefixSettings.reset(new QAction(CoreLib->loadIcon("document-edit"), tr("Edit prefix settings"), this));
    prefixSettings->setStatusTip(tr("Edit prefix settings"));
    connect(prefixSettings.get(), SIGNAL(triggered()), this, SLOT(prefixSettings_Click()));
    prefixSettings->setEnabled(false);

    prefixSetup.reset(new QAction(CoreLib->loadIcon("document-properties"), tr("Set up prefix fake drive"), this));
    prefixSetup->setStatusTip(tr("Set up prefix fake drive and applications"));
    connect(prefixSetup.get(), SIGNAL(triggered()), this, SLOT(prefixSetup_Click()));
    prefixSetup->setEnabled(false);

    versionsSetup.reset(new QAction(CoreLib->loadIcon("configure"), tr("Configure wine versions"), this));
    versionsSetup->setStatusTip(tr("Configure wine versions"));
    connect(versionsSetup.get(), SIGNAL(triggered()), this, SLOT(versionsSetup_Click()));
    versionsSetup->setEnabled(true);


    menu.reset(new QMenu(this));
    menu->addAction(prefixAdd.get());
    menu->addSeparator();
    menu->addAction(prefixImport.get());
    menu->addAction(prefixExport.get());
    menu->addSeparator();
    menu->addAction(prefixDelete.get());
    menu->addSeparator();
    menu->addAction(prefixSettings.get());
    menu->addSeparator();
    menu->addAction(prefixSetup.get());
    menu->addAction(versionsSetup.get());

    return;
}

void PrefixControlWidget::customContextMenuRequested(const QPoint &pos){
    if (prefixTable->currentIndex().isValid()){
        prefixDelete->setEnabled(true);
        prefixImport->setEnabled(true);
        prefixExport->setEnabled(true);
        prefixSettings->setEnabled(true);
        prefixSetup->setEnabled(true);
    } else {
        prefixDelete->setEnabled(false);
        prefixImport->setEnabled(false);
        prefixExport->setEnabled(false);
        prefixSettings->setEnabled(false);
        prefixSetup->setEnabled(false);
    }

    menu->exec(QCursor::pos());
    return;
}

void PrefixControlWidget::itemClicked(const QModelIndex &){
    if (prefixTable->currentIndex().isValid()){
        prefixDelete->setEnabled(true);
        prefixImport->setEnabled(true);
        prefixExport->setEnabled(true);
        prefixSettings->setEnabled(true);
        prefixSetup->setEnabled(true);
        emit (prefixIndexChanged(model->index(prefixTable->currentIndex().row(), 0).data().toString()));
    } else {
        prefixDelete->setEnabled(false);
        prefixImport->setEnabled(false);
        prefixExport->setEnabled(false);
        prefixSettings->setEnabled(false);
        prefixSetup->setEnabled(false);
    }

    return;
}

void PrefixControlWidget::prefixAdd_Click(){
    // Prefix creation function

    PrefixSettings settings;
    if (settings.exec()==QDialog::Accepted){
        this->updateTableModel();
        emit(updateDatabaseConnections());
        emit(prefixIndexChanged(settings.getPrefixName()));
    }

    return;
}

void PrefixControlWidget::updateTableModel(){
    model->setQuery("SELECT name, arch, (SELECT name FROM versions WHERE id=version_id), path FROM prefix ORDER BY name");
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Arch"));
    model->setHeaderData(2, Qt::Horizontal, tr("Version"));
    model->setHeaderData(3, Qt::Horizontal, tr("Path"));
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
    QString prefixPath = model->index(prefixTable->currentIndex().row(), 3, QModelIndex()).data().toString();

    if (prefixName.isEmpty())
        return;

    if (prefixName=="Default"){
            QMessageBox::warning(this, tr("Error"), tr("Sorry, you cannot delete the Default prefix."), QMessageBox::Ok);
            return;
    }

    QMessageBox msgBox(this);
    QCheckBox dontPrompt(QObject::tr("Remove prefix directory"), &msgBox);
    dontPrompt.blockSignals(true);

    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("Warning"));
    msgBox.setText(tr("Do you really wish to delete the prefix named \"%1\" and all associated icons?").arg(prefixName));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    msgBox.addButton(&dontPrompt, QMessageBox::ActionRole);

    if(msgBox.exec()==QMessageBox::Ok){
        if (dontPrompt.checkState() == Qt::Checked){
            CoreLib->removeDirectory(prefixPath);
        }
        if (db_icon.delIconsByPrefixName(prefixName))
            if(db_dir.delDir(prefixName)){
                db_prefix.delByName(prefixName);
#ifndef _OS_DARWIN_
                if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool()){
                    sys_menu.remove_dir_info(prefixName);
                    sys_menu.writeXMLSystemMenu();
                }
#endif
            }
        this->updateTableModel();
        emit(updateDatabaseConnections());
    }

    emit (prefixIndexChanged(model->index(prefixTable->currentIndex().row(), 0).data().toString()));

    return;
}

void PrefixControlWidget::prefixImport_Click(){
    /*
        Function for importing preefix from file
    */
    if (!prefixTable->currentIndex().isValid())
        return;

    if(prefixTable->currentIndex().row()<0)
        return;

    QString prefixName = model->index(prefixTable->currentIndex().row(), 0, QModelIndex()).data().toString();
    QString targetDir = model->index(prefixTable->currentIndex().row(), 2, QModelIndex()).data().toString();

    if (prefixName.isEmpty())
        return;

    if (targetDir.isEmpty()){
        targetDir.append(QDir::homePath());
        targetDir.append("/.wine/");
    }

    QString openpath = QDir::homePath();
    openpath.append("/.config/");
    openpath.append(APP_SHORT_NAME);
    openpath.append("/prefixes/");

#if QT_VERSION >= 0x040500
    QFileDialog::Options options;

    if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
        options = QFileDialog::DontUseNativeDialog;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file to import"), openpath , tr("Prefix archive images (*.tbz);;All files (*)"), 0, options);
#else
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file to import"), openpath , tr("Prefix archive images (*.tbz);;All files (*)"));
#endif

    if (!fileName.isEmpty()){
        QDir dir;

        if (dir.exists(targetDir)){
            if(QMessageBox::warning(this, tr("Warning"), tr("Do you really wish to delete all old prefix files?"), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok){
                QStringList args;
                args << "-rf";
                args << targetDir;

                Process exportProcess(args, CoreLib->getWhichOut("rm"), QDir::homePath(), tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));
                if (exportProcess.exec()!=QDialog::Accepted){
                    return;
                }
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
        Process exportProcess(args, CoreLib->getSetting("system", "tar").toString(), QDir::homePath(), tr("Importing prefix.<br>This can take a while..."), tr("Importing prefix"));
        exportProcess.exec();
    }
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
        emit(prefixIndexChanged(settings.getPrefixName()));
    }

    return;
}

void PrefixControlWidget::updateDtabaseItems(){
    this->updateTableModel();
}

void PrefixControlWidget::prefixExport_Click(){
    /*
     * Function for exporting preefix to file
     */
#ifdef DEBUG
    qDebug()<<"[ii] prefixExport_Click()";
#endif
    if (!prefixTable->currentIndex().isValid())
        return;
#ifdef DEBUG
    qDebug()<<"[ii] index is valid";
#endif
    if(prefixTable->currentIndex().row()<0)
        return;
#ifdef DEBUG
    qDebug()<<"[ii] row is valid";
#endif
    QString prefixName = model->index(prefixTable->currentIndex().row(), 0, QModelIndex()).data().toString();
    QString prefixPath = model->index(prefixTable->currentIndex().row(), 3, QModelIndex()).data().toString();

#ifdef DEBUG
    qDebug()<<"[ii] name: " << prefixName << " Path: " << prefixPath;
#endif
    if (prefixName.isEmpty())
        return;

    if (prefixPath.isEmpty()){
        prefixPath.append(QDir::homePath());
        prefixPath.append("/.wine/");
    }

    QString savepath = QDir::homePath();
    savepath.append("/.config/");
    savepath.append(APP_SHORT_NAME);
    savepath.append("/prefixes/");
    savepath.append(prefixName);
    savepath.append("-");
    savepath.append(QDate::currentDate().toString(Qt::ISODate));
    savepath.append(".tbz");

#ifdef DEBUG
    qDebug()<<"[ii] Path: " << prefixPath << " Save Path: " << savepath;
#endif

    //Request user for valid save path
#if QT_VERSION >= 0x040500
#ifdef DEBUG
    qDebug()<<"[ii] QT_VERSION >= 0x040500";
#endif
    QFileDialog::Options options;

    if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
        options = QFileDialog::DontUseNativeDialog;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Select file to export"), savepath , tr("Prefix archive images (*.tbz)"), 0, options);
#else
#ifdef DEBUG
    qDebug()<<"[ii] QT_VERSION <= 0x040500";
#endif
    QString fileName = QFileDialog::getSaveFileName(this, tr("Select file to export"), savepath , tr("Prefix archive images (*.tbz)"));
#endif

    if (!fileName.isEmpty()){
        QStringList args;
        args << "-cjf";
        args << fileName;
        args << "-C";
        args << prefixPath;
        args << "./";

        //Creating process dialog
        Process exportProcess(args, CoreLib->getSetting("system", "tar").toString(), prefixPath, tr("Exporting %1 prefix.<br>This can take a while...").arg(prefixName), tr("Exporting prefix"));
        exportProcess.exec();
    }

    return;
}

void PrefixControlWidget::prefixSetup_Click(){
    emit (setTabIndex (2));
    return;
}

void PrefixControlWidget::versionsSetup_Click(){
    VersionManager* vers = new VersionManager();
    vers->exec();
    delete(vers);
}
