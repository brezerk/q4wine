/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                           *
 *   pzinin@gmail.com                                                     *
 *   Copyright (C) 2010 by Malakhov Alexey                           *
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

#ifndef WINETRICKS_H
#define WINETRICKS_H

#include "config.h"

#include "src/q4wine-gui/ui_winetricks.h"

#include <QStringList>
#include <QProcess>
#include <QMessageBox>
#include <QLibrary>
#include <QKeyEvent>
#include <QEvent>
#include "process.h"

#include "prefix.h"

#include "q4wine-lib.h"

class winetricks : public QDialog, public Ui::winetricks {
    Q_OBJECT
    Q_DISABLE_COPY(winetricks)
public:
    winetricks(QString prefixName, QWidget * parent = 0, Qt::WFlags f = 0);
    QString _prefixName;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void lstMain_itemClicked(QListWidgetItem* item);
    void run_winetricks();
    void install_winetricks();
    bool parse();
private:
    //! This is need for libq4wine-core.so import;
    typedef void *CoreLibPrototype (bool);
        CoreLibPrototype *CoreLibClassPointer;
        std::auto_ptr<corelib> CoreLib;

    QLibrary libq4wine;

    Prefix db_prefix;

    QString console_bin;
    QString console_args;
    QString prefix_name;
    QString winetricks_bin;

    void downloadwinetricks();
    void removefile(QString);

    void addToList (QString arg);

    QStringList names;
    QStringList descs;

};

#endif // WINETRICKS_H
