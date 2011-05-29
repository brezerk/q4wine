/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                     *
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

#include "wineprocesswidget.h"

WineProcessWidget::WineProcessWidget(QWidget *parent) : QWidget(parent)
{
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

    this->createActions();

    std::auto_ptr<QToolBar> toolBar (new QToolBar(this));
    toolBar->addAction(procKillSelected.get());
    toolBar->addAction(procKillWine.get());
    toolBar->addSeparator();
    toolBar->addAction(procRefresh.get());

    procTable.reset(new QTableView(this));

    model.reset(new QStandardItemModel());
    model->setColumnCount(4);
    model->setHeaderData(0, Qt::Horizontal, tr("PID"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, tr("Name"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, tr("Ni"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, tr("Prefix path"), Qt::DisplayRole);

    procTable->setContextMenuPolicy(Qt::CustomContextMenu);
    procTable->setModel(model.get());

    connect(procTable.get(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    connect(procTable.get(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)));

    procTable->resizeColumnsToContents();
    procTable->resizeRowsToContents();
    procTable->horizontalHeader()->setStretchLastSection(true);
    procTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    procTable->setSelectionMode(QAbstractItemView::ExtendedSelection);

    lblInfo.reset(new QLabel(tr("Total process count: %1").arg(0), this));
    lblInfo->setContentsMargins(2,2,2,2);


    std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(toolBar.release());
    layout->addWidget(procTable.get());
    layout->addWidget(lblInfo.get());

    this->setLayout(layout.release());

    timer.reset(new QTimer());
    timer->start(1000);

    // Connecting signals and slots
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(getWineProcesssInfo()));
    return;
}

void WineProcessWidget::stopTimer(void){
    timer->stop();
    return;
}

void WineProcessWidget::startTimer(void){
    this->getWineProcesssInfo();
    timer->start();
    return;
}

void WineProcessWidget::createActions(){
    procKillSelected.reset(new QAction(CoreLib->loadIcon("data/kill.png"), tr("Stop current"), this));
    procKillSelected->setStatusTip(tr("Send TERM signal to selected process"));
    procKillSelected->setEnabled(false);
    connect(procKillSelected.get(), SIGNAL(triggered()), this, SLOT(procKillSelected_Click()));

    procKillWine.reset(new QAction(CoreLib->loadIcon("data/stop.png"), tr("Stop wine"), this));
    procKillWine->setStatusTip(tr("Send TERM signal to main wine process"));
    procKillWine->setEnabled(false);
    connect(procKillWine.get(), SIGNAL(triggered()), this, SLOT(procKillWine_Click()));

    procRefresh.reset(new QAction(CoreLib->loadIcon("data/reload.png"), tr("Refresh list"),this));
    procRefresh->setStatusTip(tr("Refresh process list"));
    connect(procRefresh.get(), SIGNAL(triggered()), this, SLOT(getWineProcesssInfo()));

    procRenice.reset(new QAction(tr("Renice"), this));
    procRenice->setStatusTip(tr("Set process priority"));
    procRenice->setEnabled(false);
    connect(procRenice.get(), SIGNAL(triggered()), this, SLOT(procRenice_Click()));

    menu.reset(new QMenu(this));
    menu->addAction(procKillSelected.get());
    menu->addAction(procKillWine.get());
    menu->addSeparator();
    menu->addAction(procRenice.get());
    menu->addSeparator();
    menu->addAction(procRefresh.get());

    return;
}

void WineProcessWidget::getWineProcesssInfo(void){
    QList<QStringList> proclist = CoreLib->getWineProcessList();

    if (proclist.count()<=0){
        procKillSelected->setEnabled(false);
        procKillWine->setEnabled(false);
        procRenice->setEnabled(false);
        model->setRowCount(0);
        lblInfo->setText(tr("Total process count: %1").arg(0));
        return;
    }

    model->setRowCount(proclist.size());

    for (int i = 0; i < proclist.size(); ++i) {
        model->setData(model->index(i, 0, QModelIndex()), proclist.at(i).at(0), Qt::DisplayRole);
        model->setData(model->index(i, 1, QModelIndex()), proclist.at(i).at(1), Qt::DisplayRole);
        model->setData(model->index(i, 2, QModelIndex()), proclist.at(i).at(2), Qt::DisplayRole);
        model->setData(model->index(i, 3, QModelIndex()), proclist.at(i).at(3), Qt::DisplayRole);
    }

    lblInfo->setText(tr("Total process count: %1").arg(proclist.size()));

    procTable->resizeColumnsToContents();
    procTable->resizeRowsToContents();
    procTable->horizontalHeader()->setStretchLastSection(true);

    if (procTable->currentIndex().isValid()){
        procKillSelected->setEnabled(true);
        procKillWine->setEnabled(true);
        procRenice->setEnabled(true);
    } else {
        procKillSelected->setEnabled(false);
        procKillWine->setEnabled(false);
        procRenice->setEnabled(false);
    }

    return;
}

void WineProcessWidget::customContextMenuRequested(const QPoint &pos){
    if (procTable->currentIndex().isValid()){
        procKillSelected->setEnabled(true);
        procKillWine->setEnabled(true);
        procRenice->setEnabled(true);
    } else {
        procKillSelected->setEnabled(false);
        procKillWine->setEnabled(false);
        procRenice->setEnabled(false);
    }

    menu->exec(QCursor::pos());
    return;
}

void WineProcessWidget::itemClicked(const QModelIndex &){
    if (procTable->currentIndex().isValid()){
        procKillSelected->setEnabled(true);
        procKillWine->setEnabled(true);
        procRenice->setEnabled(true);
    } else {
        procKillSelected->setEnabled(false);
        procKillWine->setEnabled(false);
        procRenice->setEnabled(false);
    }
    return;
}

void WineProcessWidget::procKillSelected_Click(void){
    if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a KILL(-9) signal to selected processes<br><br>It is HIGH risk to damage wine normal state.<br><br>Do you really want to proceed?"), QMessageBox::Yes, QMessageBox::No)==QMessageBox::No)
        return;

    QItemSelectionModel *selectionModel = procTable->selectionModel();

    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    QList<int> procList;

    foreach(index, indexes) {
        if (index.column()==0)
            procList.append(model->index(index.row(), 0, QModelIndex()).data().toInt());
    }

    if (procList.count()<=0)
        return;

    for (int i=0; i<procList.count(); i++){
        QString cmd = QString("kill -9 %1").arg(procList.at(i));

        if (system(cmd.toAscii().data())==-1){
            QMessageBox::warning(this, tr("Error"), tr("Can't run: %1").arg(cmd.toAscii().data()), QMessageBox::Ok);
#ifdef DEBUG
            qDebug()<<"[EE] Fail to run: "<<cmd;
#endif
        } else {
            emit(changeStatusText(tr("It seems process %1 killed successfully.").arg(procList.at(i))));
#ifdef DEBUG
            qDebug()<<"[ii] Run OK: "<<cmd;
#endif
        }
    }

    return;
}

void WineProcessWidget::procKillWine_Click(void){
    if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a KILL(-9) signal to all wine processes for selected prefixes<br><br>Do you really want to proceed?"), QMessageBox::Yes, QMessageBox::No)==QMessageBox::No)
        return;

    QItemSelectionModel *selectionModel = procTable->selectionModel();

    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    QList<QString> prefixList;

    foreach(index, indexes) {
        if (index.column()==3){
            QString path = model->index(index.row(), 3, QModelIndex()).data().toString();
            if (prefixList.indexOf(path, 0)<0)
                prefixList.append(path);
        }
    }

    if (prefixList.count()<=0)
        return;

    for (int i=0; i<prefixList.count(); i++){
        CoreLib->killWineServer(prefixList.at(i));
    }

    return;
}

void WineProcessWidget::procRenice_Click(void){
    bool ok=false;
    int newNice = QInputDialog::getInteger(this, tr("Select process priority"), tr("<p>Priority value can be in<br>the range from PRIO_MIN (-20)<br>to PRIO_MAX (20).</p><p>See \"man renice\" for details.</p>"), 0, -20, 20, 1, &ok);
    if (!ok)
        return;

    QItemSelectionModel *selectionModel = procTable->selectionModel();

    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    QList<int> procList;

    foreach(index, indexes) {
        if (index.column()==0)
            procList.append(model->index(index.row(), 0, QModelIndex()).data().toInt());
    }

    if (procList.count()<=0)
        return;

    for (int i=0; i<procList.count(); i++){
        if (CoreLib->reniceProcess(procList.at(i), newNice)){
            emit(changeStatusText(tr("It seems process %1 renice to %2 end successfully.").arg(procList.at(i)).arg(newNice)));
        }
    }

    return;
}
