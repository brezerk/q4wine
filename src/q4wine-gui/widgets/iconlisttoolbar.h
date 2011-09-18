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

#ifndef ICONLISTTOOLBAR_H
#define ICONLISTTOOLBAR_H

//System
#include <memory>

//Global config
#include "config.h"

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>

//q4wine lib
#include "q4wine-lib.h"

class IconListToolbar : public QWidget
{
Q_OBJECT
public:
    explicit IconListToolbar(QWidget *parent = 0);

signals:
    void searchFilterChange(QString filter);
    void changeView(int action);

public slots:

private:
    //! This is need for libq4wine-core.so import.
    QLibrary libq4wine;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::auto_ptr<corelib> CoreLib;

    void createActions(void);

    std::auto_ptr<QLineEdit> searchField;

    std::auto_ptr<QAction> searchClear;
    std::auto_ptr<QAction> viewIcon;
    std::auto_ptr<QAction> viewList;

    std::auto_ptr<QAction> zoomIn;
    std::auto_ptr<QAction> zoomOut;

    std::auto_ptr<QAction> sortAlpha;
    std::auto_ptr<QAction> sortCreation;

    int sort_order;

private slots:
    void searchClear_Click();
    void viewIcon_Click();
    void viewList_Click();
    void zoomIn_Click();
    void zoomOut_Click();
    void sortAlpha_Click();
    void sortCreation_Click();
};

#endif // ICONLISTTOOLBAR_H
