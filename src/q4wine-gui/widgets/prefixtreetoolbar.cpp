/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                     *
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
    toolBar->addAction(treeExpand.get());
    toolBar->addAction(treeCollapse.get());
    toolBar->addSeparator();
    toolBar->addAction(prefixExport.get());
    toolBar->addAction(prefixImport.get());

    std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(toolBar.release());

    this->setLayout(layout.release());
}

void PrefixTreeToolbar::createActions(){
    treeExpand.reset(new QAction(CoreLib->loadIcon("data/expand.png"), tr("Expand prefix tree"), this));
    treeExpand->setStatusTip(tr("Expand prefix tree"));
    connect(treeExpand.get(), SIGNAL(triggered()), this, SLOT(treeExpand_Click()));

    treeCollapse.reset(new QAction(CoreLib->loadIcon("data/collapse.png"), tr("Collapse prefix tree"), this));
    treeCollapse->setStatusTip(tr("Collapse prefix tree"));
    connect(treeCollapse.get(), SIGNAL(triggered()), this, SLOT(treeCollapse_Click()));

    prefixImport.reset(new QAction(CoreLib->loadIcon("data/import.png"), tr("Import prefixes"), this));
    prefixImport->setStatusTip(tr("Import prefixes from ~/.local/share/wineprefixes/"));
    connect(prefixImport.get(), SIGNAL(triggered()), this, SLOT(prefixImport_Click()));

    prefixExport.reset(new QAction(CoreLib->loadIcon("data/export.png"), tr("Export prefixes"), this));
    prefixExport->setStatusTip(tr("Export prefixes to ~/.local/share/wineprefixes/"));
    connect(prefixExport.get(), SIGNAL(triggered()), this, SLOT(prefixExport_Click()));

    return;
}

void PrefixTreeToolbar::treeExpand_Click(){
    emit(expandTree());
}

void PrefixTreeToolbar::treeCollapse_Click(){
    emit(collapseTree());
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
