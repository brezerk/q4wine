/***************************************************************************
 *   Copyright (C) 2008-2016 by Oleksii S. Malakhov <brezerk@gmail.com>     *
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

#ifndef APPDBHEADERWIDGET_H
#define APPDBHEADERWIDGET_H

//System
#include <memory>

//Global config
#include "config.h"

//Qt inc
#include <QFrame>
#include <QBoxLayout>
#include <QVariant>

#ifdef DEBUG
#include <QDebug>
#endif

//Structs
#include "appdbstructs.h"

//Widgets
#include "linkitemwidget.h"

class AppDBHeaderWidget : public QFrame
{
    Q_OBJECT
public:
    AppDBHeaderWidget(QWidget * parent = 0);
    void addLabel(QString info);
    void addLink(QString text, bool enabled = true, short int action = 0, QString search = "", int value = 0);
    void setLayout(short int direction);
    void insertStretch(void);
    void createPagesList(short int count, short int current, QString search);
    void createCategoryList(const QList<WineAppDBCategory> category);
    void clear(void);
    void hideAll(void);
    void updateFirstLabelText(QString text);

signals:
    void itemTrigged(short int, QString, int, int, int);

private:
    std::auto_ptr<QBoxLayout> contentLayout;
    short int pages_len;

};

#endif // APPDBHEADERWIDGET_H
