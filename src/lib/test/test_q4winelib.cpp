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



#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "tesWineObjects"
#include <boost/test/unit_test.hpp>

#include <string>

#include "src/lib/defines.hpp"
#include "src/lib/wineapplication.hpp"
#include "src/lib/wineprefix.hpp"
#include "src/lib/wineversion.hpp"

BOOST_AUTO_TEST_SUITE(testSuiteWineObjects)

BOOST_AUTO_TEST_CASE(testWineVersion) {
    q4wine::lib::WineVersion version = q4wine::lib::WineVersion("binary",
                                  "loader", "server", "libs32", "libs64");
    BOOST_CHECK_EQUAL(version.getId(), uintptr_t(0));
    BOOST_CHECK_EQUAL(version.getBinary(), std::string("binary"));
    BOOST_CHECK_EQUAL(version.getServer(), std::string("server"));
    BOOST_CHECK_EQUAL(version.getLoader(), std::string("loader"));
    BOOST_CHECK_EQUAL(version.getLibs32(), std::string("libs32"));
    BOOST_CHECK_EQUAL(version.getLibs64(), std::string("libs64"));
    BOOST_CHECK_EQUAL(version.getLibs(q4wine::lib::WIN32),
                      std::string("libs32"));
    BOOST_CHECK_EQUAL(version.getLibs(q4wine::lib::WIN64),
                      std::string("libs64"));
}

BOOST_AUTO_TEST_CASE(testWinePrefix) {
    q4wine::lib::WinePrefix* prefix = new q4wine::lib::WinePrefix("test",
                "/root", q4wine::lib::WIN32,
                new q4wine::lib::WineVersion("binary",
                "loader", "server", "libs32", "libs64"),
                "/mnt/cdrom", "D:",
                "some_exec_tempalte", 12);

    // Basic unit tests
    BOOST_CHECK_EQUAL(prefix->getId(), uintptr_t(12));
    BOOST_CHECK_EQUAL(prefix->getName(), std::string("test"));
    BOOST_CHECK_EQUAL(prefix->getPath(), std::string("/root"));
    BOOST_CHECK_EQUAL(prefix->getArch(), q4wine::lib::WIN32);
    BOOST_CHECK_EQUAL(prefix->getMountPoint(), std::string("/mnt/cdrom"));
    BOOST_CHECK_EQUAL(prefix->getVirtualDevice(), std::string("D:"));
    BOOST_CHECK_EQUAL(prefix->getExecutionTemplate(),
                      std::string("some_exec_tempalte"));

    /* Advanced unit tests
    // BOOST_CHECK_EQUAL(prefix->getWineEnv(), std::string("WINEPREFIX='/root' "
"WINESERVER='server' WINELOADER='loader' WINEDLLPATH='libs32' "
"WINEARCH='win32'"));
*/
    delete(prefix);
}

BOOST_AUTO_TEST_CASE(testWineApplication) {
    q4wine::lib::WineApplication application = q4wine::lib::WineApplication(
                "test app",
                "C://test.exe",
                "--make all --quality=good",
                "C://",
                "",
                "Some test app",
                "libgcc.dll=b,n",
                "warn+dll,+heap",
                "600x800",
                "RU_ru",
                true,
                ":1",
                -110,
                "/usr/lib/pre_run.sh",
                "echo done >> /tmp/lol");
    BOOST_CHECK_EQUAL(application.getName(), std::string("test app"));
    BOOST_CHECK_EQUAL(application.getPath(), std::string("C://test.exe"));
    BOOST_CHECK_EQUAL(application.getArgs(),
                      std::string("--make all --quality=good"));
}

BOOST_AUTO_TEST_SUITE_END()  // End of tests
