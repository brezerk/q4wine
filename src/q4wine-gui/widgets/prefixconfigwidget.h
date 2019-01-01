/***************************************************************************
 *   Copyright (C) 2008-2019 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

//System
#include <memory>

//Global config
#include "config.h"

#include "winetricks.h"
#include "sysconfig.h"
#include "fakedrivesettings.h"

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QSplitter>
#include <QLineEdit>
#include <QDateTime>
#include <QBrush>
#include <QPalette>
#include <QMenu>
#include <QKeyEvent>
#include <QClipboard>
#include <QComboBox>
#include <QProgressDialog>

#include "prefix.h"

//q4wine lib
#include "q4wine-lib.h"

class PrefixConfigWidget : public QWidget
{
Q_OBJECT
public:
    explicit PrefixConfigWidget(QWidget *parent = 0);
    ~PrefixConfigWidget();


signals:

public slots:
    void getPrefixes(void);
    void setPrefix(QString prefix);

signals:
      void prefixIndexChanged(QString);
      void updateDatabaseConnections();
      void setTabIndex(int);

private slots:
    void treeWidget_itemClicked (QTreeWidgetItem * item, int column);
    void treeWidget_currentItemChanged (QTreeWidgetItem *item, QTreeWidgetItem *);
    void itemClicked (QListWidgetItem *item);
    void itemDoubleClicked (QListWidgetItem *item);
    void currentItemChanged (QListWidgetItem *item, QListWidgetItem *);
    void keyPressEvent ( QKeyEvent * event );
    void searchClear_Click();
    void sortAlpha_Click();
    void zoomIn_Click();
    void zoomOut_Click();
    void searchFilterChange(QString);
    void prefixManage_Click();

private:
    //! This is need for libq4wine-core.so import.
    QLibrary libq4wine;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::unique_ptr<corelib> CoreLib;

    Prefix db_prefix;
    Sysconfig db_sysconfig;

    std::unique_ptr<QAction> prefixManage;
    std::unique_ptr<QAction> searchClear;
    std::unique_ptr<QAction> sortAlpha;
    std::unique_ptr<QLineEdit> searchField;
    std::unique_ptr<QAction> zoomIn;
    std::unique_ptr<QAction> zoomOut;

    std::unique_ptr<QComboBox> cbPrefixes;
    std::unique_ptr<QLabel> infoName;

    QString provider;
    QString subtype;

    int sort_order;

    std::unique_ptr<QTreeWidget> treeWidget;
    std::unique_ptr<QListWidget> listWidget;
    std::unique_ptr<QSplitter> splitter;

    //std::unique_ptr<QMenu> menu;

    void createActions(void);
    void createTree(void);

    void get_icons(void);

};

#endif // CONFIGWIDGET_H
