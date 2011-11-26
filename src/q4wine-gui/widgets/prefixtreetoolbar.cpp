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

#include "prefixtreetoolbar.h"

PrefixTreeToolbar::PrefixTreeToolbar(QWidget *parent) :
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

    this->tree_state = CoreLib->getSetting("TreeWidget", "State", false, D_TREE_EXPAND).toInt();
    this->createActions();

    std::auto_ptr<QToolBar> toolBar (new QToolBar(this));

    toolBar->setIconSize(QSize(24, 24));
    toolBar->addAction(treeState.get());
    toolBar->addSeparator();
    toolBar->addAction(prefixExport.get());
    toolBar->addAction(prefixImport.get());

    std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(toolBar.release());

    this->setLayout(layout.release());
}

PrefixTreeToolbar::~PrefixTreeToolbar(){
    QSettings settings(APP_SHORT_NAME, "default");
    settings.beginGroup("TreeWidget");
    settings.setValue("State", this->tree_state);
    settings.endGroup();
}

void PrefixTreeToolbar::createActions(){
    treeState.reset(new QAction(this));
    if (this->tree_state == D_TREE_COLLAPSE){
        treeState->setIcon(CoreLib->loadIcon("data/expand.png"));
        treeState->setText(tr("Expand prefix tree"));
        treeState->setStatusTip(tr("Expand prefix tree"));
        emit(collapseTree());
    } else {
        treeState->setIcon(CoreLib->loadIcon("data/collapse.png"));
        treeState->setText(tr("Collapse prefix tree"));
        treeState->setStatusTip(tr("Collapse prefix tree"));
        emit(expandTree());
    }

    connect(treeState.get(), SIGNAL(triggered()), this, SLOT(treeState_Click()));

    prefixImport.reset(new QAction(CoreLib->loadIcon("data/import.png"), tr("Import prefixes"), this));
    prefixImport->setStatusTip(tr("Import prefixes from ~/.local/share/wineprefixes/"));
    connect(prefixImport.get(), SIGNAL(triggered()), this, SLOT(prefixImport_Click()));

    prefixExport.reset(new QAction(CoreLib->loadIcon("data/export.png"), tr("Export prefixes"), this));
    prefixExport->setStatusTip(tr("Export prefixes to ~/.local/share/wineprefixes/"));
    connect(prefixExport.get(), SIGNAL(triggered()), this, SLOT(prefixExport_Click()));

    return;
}

void PrefixTreeToolbar::treeState_Click(){
    if (this->tree_state == D_TREE_EXPAND){
        emit(collapseTree());
        this->tree_state = D_TREE_COLLAPSE;
        treeState->setIcon(CoreLib->loadIcon("data/expand.png"));
        treeState->setText(tr("Expand prefix tree"));
        treeState->setStatusTip(tr("Expand prefix tree"));
    } else {
        emit(expandTree());
        this->tree_state = D_TREE_EXPAND;
        treeState->setIcon(CoreLib->loadIcon("data/collapse.png"));
        treeState->setText(tr("Collapse prefix tree"));
        treeState->setStatusTip(tr("Collapse prefix tree"));
    }
}

void PrefixTreeToolbar::prefixImport_Click(){
    foreach (QString prefixPath, CoreLib->importPrefixesConfiguration()){
        QString path;
        Registry reg(prefixPath);
        QStringList list;
        list << "\"Desktop\"";
        list = reg.readKeys("user", "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", list);


        prefixPath.append("/dosdevices/c:/users/");
        prefixPath.append(getenv("USER"));

        QFileInfo fileinfo(QString("%1/%2").arg(prefixPath).arg(CoreLib->decodeRegString(list.at(0).split("\\\\").last())));
        if (fileinfo.isSymLink()){
            path = fileinfo.symLinkTarget();
        } else {
            path = fileinfo.filePath();
        }

        Progress progress(0, path);
        progress.exec();
    }

    emit(updatePrefixTree());
}

void PrefixTreeToolbar::prefixExport_Click(){
    CoreLib->exportPrefixesConfiguration();
}
