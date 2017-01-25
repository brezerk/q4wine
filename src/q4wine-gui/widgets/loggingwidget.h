/***************************************************************************
 *   Copyright (C) 2008-2017 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#ifndef LOGGINGWIDGET_H
#define LOGGINGWIDGET_H

//System
#include <memory>

//Global config
#include "config.h"

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
#include <QPushButton>

#include "logging.h"
#include "prefix.h"

//q4wine lib
#include "q4wine-lib.h"

#define D_LOGGING_DISABLED 0
#define D_LOGGING_ENABLED 1


class LoggingWidget : public QWidget
{
Q_OBJECT
public:
    explicit LoggingWidget(QWidget *parent = 0);
    ~LoggingWidget();


signals:

public slots:
    void getLogRecords(void);

private slots:
    void logClear_Click(void);
    void logDelete_Click(void);
    void logExport_Click(void);
    void logEnable_Click(void);

    void logCopy_Click(void);
    void logSelectAll_Click(void);

    void treeWidget_itemClicked (QTreeWidgetItem * item, int column);
    void treeWidget_currentItemChanged (QTreeWidgetItem *item, QTreeWidgetItem *);

    void treeWidget_customContextMenuRequested(const QPoint &pos);
    void listWidget_customContextMenuRequested(const QPoint &pos);

private:
    //! This is need for libq4wine-core.so import.
    QLibrary libq4wine;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::auto_ptr<corelib> CoreLib;

    Logging db_log;
    Prefix db_prefix;

    std::auto_ptr<QAction> logClear;
    std::auto_ptr<QAction> logDelete;
    std::auto_ptr<QAction> logExport;

    std::auto_ptr<QAction> logSelectAll;
    std::auto_ptr<QAction> logCopy;

    std::auto_ptr<QAction> logEnable;
    std::auto_ptr<QLabel> logStatus;

    int log_status;

    std::auto_ptr<QTreeWidget> treeWidget;
    std::auto_ptr<QListWidget> listWidget;
    std::auto_ptr<QSplitter> splitter;

    //std::auto_ptr<QMenu> menu;

    void createActions(void);
    void clearLogs(void);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

#endif // LOGGINGWIDGET_H
