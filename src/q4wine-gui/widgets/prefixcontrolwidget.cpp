/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                            *
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

PrefixControlWidget::PrefixControlWidget(QWidget *parent) :
    QWidget(parent)
{
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
    prefixAdd.reset(new QAction(CoreLib->loadIcon("data/wizard.png"), tr("Create new"), this));
    prefixAdd->setStatusTip(tr("Create new prefix"));
    connect(prefixAdd.get(), SIGNAL(triggered()), this, SLOT(prefixAdd_Click()));

    prefixImport.reset(new QAction(CoreLib->loadIcon("data/down.png"), tr("Import prefix"), this));
    prefixImport->setStatusTip(tr("Import prefix"));
    connect(prefixImport.get(), SIGNAL(triggered()), this, SLOT(prefixImport_Click()));
    prefixImport->setEnabled(FALSE);

    prefixExport.reset(new QAction(CoreLib->loadIcon("data/up.png"), tr("Export prefix"), this));
    prefixExport->setStatusTip(tr("Export prefix"));
    connect(prefixExport.get(), SIGNAL(triggered()), this, SLOT(prefixExport_Click()));
    prefixExport->setEnabled(FALSE);

    prefixDelete.reset(new QAction(CoreLib->loadIcon("data/kill.png"), tr("Delete prefix"), this));
    prefixDelete->setStatusTip(tr("Delete prefix"));
    connect(prefixDelete.get(), SIGNAL(triggered()), this, SLOT(prefixDelete_Click()));
    prefixDelete->setEnabled(FALSE);

    prefixSettings.reset(new QAction(CoreLib->loadIcon("data/configure.png"), tr("Edit prefix settings"), this));
    prefixSettings->setStatusTip(tr("Edit prefix settings"));
    connect(prefixSettings.get(), SIGNAL(triggered()), this, SLOT(prefixSettings_Click()));
    prefixSettings->setEnabled(FALSE);

    prefixSetup.reset(new QAction(CoreLib->loadIcon("data/eject.png"), tr("Set up prefix fake drive"), this));
    prefixSetup->setStatusTip(tr("Set up prefix fake drive and applications"));
    connect(prefixSetup.get(), SIGNAL(triggered()), this, SLOT(prefixSetup_Click()));
    prefixSetup->setEnabled(FALSE);

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
    model->setQuery("SELECT name, arch, path FROM prefix ORDER BY name");
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Architecture"));
    model->setHeaderData(2, Qt::Horizontal, tr("Path"));
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

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file to import"), openpath , tr("Prefix archive images (*.tbz)"), 0, options);
#else
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file to import"), openpath , tr("Prefix archive images (*.tbz)"));
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
    if (!prefixTable->currentIndex().isValid())
        return;

    if(prefixTable->currentIndex().row()<0)
        return;

    QString prefixName = model->index(prefixTable->currentIndex().row(), 0, QModelIndex()).data().toString();
    QString prefixPath = model->index(prefixTable->currentIndex().row(), 2, QModelIndex()).data().toString();

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

    //Request user for valid save path
#if QT_VERSION >= 0x040500
    QFileDialog::Options options;

    if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
        options = QFileDialog::DontUseNativeDialog;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Select file to export"), savepath , tr("Prefix archive images (*.tbz)"), 0, options);
#else
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
