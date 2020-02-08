/***************************************************************************
 *   Copyright (C) 2008-2020 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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
    std::unique_ptr<corelib> CoreLib;

    void createActions(void);

    std::unique_ptr<QLineEdit> searchField;

    std::unique_ptr<QAction> searchClear;
    std::unique_ptr<QAction> viewMode;

    std::unique_ptr<QAction> zoomIn;
    std::unique_ptr<QAction> zoomOut;

    std::unique_ptr<QAction> sortAlpha;
    std::unique_ptr<QAction> sortCreation;

    int sort_order;
    int view_mode;

private slots:
    void searchClear_Click();
    void viewMode_Click();
    void zoomIn_Click();
    void zoomOut_Click();
    void sortAlpha_Click();
    void sortCreation_Click();
};

#endif // ICONLISTTOOLBAR_H
