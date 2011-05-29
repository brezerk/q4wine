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

#ifndef PREFIXTREETOOLBAR_H
#define PREFIXTREETOOLBAR_H

//System
#include <memory>

//Global config
#include "config.h"
#include "progress.h"

#include <QWidget>
#include <QToolBar>
#include <QAction>

//q4wine lib
#include "q4wine-lib.h"

#include "src/core/registry.h"

class PrefixTreeToolbar : public QWidget
{
Q_OBJECT
public:
    explicit PrefixTreeToolbar(QWidget *parent = 0);

signals:
    void updatePrefixTree(void);
    void expandTree(void);
    void collapseTree(void);

public slots:

private:
    //! This is need for libq4wine-core.so import.
    QLibrary libq4wine;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::auto_ptr<corelib> CoreLib;

    void createActions(void);

    std::auto_ptr<QAction> treeExpand;
    std::auto_ptr<QAction> treeCollapse;

    std::auto_ptr<QAction> prefixImport;
    std::auto_ptr<QAction> prefixExport;

private slots:
    void treeExpand_Click();
    void treeCollapse_Click();
    void prefixImport_Click();
    void prefixExport_Click();

};

#endif // PREFIXTREETOOLBAR_H
