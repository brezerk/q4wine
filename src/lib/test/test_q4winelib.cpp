/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include <QtTest/QtTest>

#include "src/lib/defines.hpp"
#include "src/lib/wineprefix.hpp"
#include "src/lib/wineversion.hpp"

class test_Q4WineLib: public QObject {
    Q_OBJECT
 private slots:
    void testWineVersion();
    void testWinePrefix();
};

void test_Q4WineLib::testWineVersion() {
    q4wine::lib::WineVersion version = q4wine::lib::WineVersion("binary",
                                  "loader", "server", "libs32", "libs64");
    QCOMPARE(version.getBinary(), QString("binary"));
    QCOMPARE(version.getServer(), QString("server"));
    QCOMPARE(version.getLoader(), QString("loader"));
    QCOMPARE(version.getLibs32(), QString("libs32"));
    QCOMPARE(version.getLibs64(), QString("libs64"));
    QCOMPARE(version.getLibs(q4wine::lib::WIN32), QString("libs32"));
    QCOMPARE(version.getLibs(q4wine::lib::WIN64), QString("libs64"));
}

void test_Q4WineLib::testWinePrefix() {
    q4wine::lib::WineVersion version = q4wine::lib::WineVersion("binary",
                                  "loader", "server", "libs32", "libs64");
    q4wine::lib::WinePrefix prefix = q4wine::lib::WinePrefix("test", "/root",
                             q4wine::lib::WIN32, version, "/mnt/cdrom", "D:",
                             "some_exec_tempalte");
    QCOMPARE(prefix.getName(), QString("test"));
    QCOMPARE(prefix.getPath(), QString("/root"));
    QCOMPARE(prefix.getArch(), q4wine::lib::WIN32);
    QCOMPARE(prefix.getMountPoint(), QString("/mnt/cdrom"));
    QCOMPARE(prefix.getVirtualDevice(), QString("D:"));
    QCOMPARE(prefix.getExecutionTemplate(), QString("some_exec_tempalte"));
}

QTEST_MAIN(test_Q4WineLib)
#include "test_q4winelib.moc"

