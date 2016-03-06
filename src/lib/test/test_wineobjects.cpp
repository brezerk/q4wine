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
#define BOOST_TEST_MODULE "testWineObjects"
#include <boost/test/unit_test.hpp>

#include <string>

#include "src/lib/defines.hpp"
#include "src/lib/db.hpp"
#include "src/lib/wineapplication.hpp"
#include "src/lib/wineprefix.hpp"
#include "src/lib/wineversion.hpp"

BOOST_AUTO_TEST_SUITE(testSuiteWineObjects)

BOOST_AUTO_TEST_CASE(testWineVersion) {
    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    db->open(":memory:");
    db->init();
    std::unique_ptr<q4wine::lib::WineVersion> version(
                                  new q4wine::lib::WineVersion(
                                  "test",
                                  "/usr/bin/wine",
                                  "/usr/bin/wineserver",
                                  "/usr/bin/wine",
                                  "/home/develop/win32",
                                  "/home/develop/win64"));
    BOOST_CHECK_EQUAL(version->getId(), uintptr_t(0));
    BOOST_CHECK_EQUAL(version->getBinary(),
                      std::string("/usr/bin/wine"));
    BOOST_CHECK_EQUAL(version->getServer(),
                      std::string("/usr/bin/wineserver"));
    BOOST_CHECK_EQUAL(version->getLoader(),
                      std::string("/usr/bin/wine"));
    BOOST_CHECK_EQUAL(version->getLibs32(),
                      std::string("/home/develop/win32"));
    BOOST_CHECK_EQUAL(version->getLibs64(),
                      std::string("/home/develop/win64"));
    BOOST_CHECK_EQUAL(version->getLibs(q4wine::lib::WIN32),
                      std::string("/home/develop/win32"));
    BOOST_CHECK_EQUAL(version->getLibs(q4wine::lib::WIN64),
                      std::string("/home/develop/win64"));
    version->save();
    BOOST_CHECK_EQUAL(version->getId(), uintptr_t(1));
}

BOOST_AUTO_TEST_CASE(testWineApplication) {
    std::unique_ptr<q4wine::lib::WineApplication> application(
                new q4wine::lib::WineApplication(
                "test app",
                "winecfg",
                "--make all --quality=good",
                "/home/brezerk",
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
                "echo done >> /tmp/lol"));
    BOOST_CHECK_EQUAL(application->getName(), std::string("test app"));
    BOOST_CHECK_EQUAL(application->getPath(), std::string("winecfg"));
    BOOST_CHECK_EQUAL(application->getArgs(),
                      std::string("--make all --quality=good"));
}

BOOST_AUTO_TEST_CASE(testWinePrefix) {
    std::shared_ptr<q4wine::lib::WinePrefix> prefix(
                new q4wine::lib::WinePrefix("test",
                "/home/brezerk/.wine", q4wine::lib::WIN32,
                q4wine::lib::WineVersion::getInstance(1),
                "/mnt/cdrom", "D:",
                q4wine::lib::DEFAULT_EXEC_TEMPLATE,
                12));

    // Basic unit tests
    BOOST_CHECK_EQUAL(prefix->getId(), uintptr_t(12));
    BOOST_CHECK_EQUAL(prefix->getName(), std::string("test"));
    BOOST_CHECK_EQUAL(prefix->getPath(), std::string("/home/brezerk/.wine"));
    BOOST_CHECK_EQUAL(prefix->getArch(), q4wine::lib::WIN32);
    BOOST_CHECK_EQUAL(prefix->getMountPoint(), std::string("/mnt/cdrom"));
    BOOST_CHECK_EQUAL(prefix->getVirtualDevice(), std::string("D:"));
    BOOST_CHECK_EQUAL(prefix->getExecutionTemplate(),
                      q4wine::lib::DEFAULT_EXEC_TEMPLATE);

    std::shared_ptr<q4wine::lib::WineApplication> application (new
            q4wine::lib::WineApplication(
                "test app",
                "winecfg",
                "--make all --quality=good",
                "/home/brezerk",
                "",
                "Some test app",
                "libgcc.dll=b,n",
                "",
                "600x800",
                "RU_ru",
                true,
                ":1",
                -110,
                "/usr/lib/pre_run.sh",
                "echo done >> /tmp/lol"));

    // Advanced unit tests
    BOOST_CHECK_EQUAL(prefix->getEnvVariables(application.get()),
                      std::string(" LANG='RU_ru' DISPLAY=':1' "
                      "WINEDLLOVERRIDES='libgcc.dll=b,n' "
                      "WINEPREFIX='/home/brezerk/.wine' "
                      "WINESERVER='/usr/bin/wineserver' "
                      "WINELOADER='/usr/bin/wine' "
                      "WINEDLLPATH='/home/develop/win32' "
                      "WINEARCH='win32'"));

    BOOST_CHECK_EQUAL(prefix->getExecutionString(application.get()),
                      std::string("/usr/bin/konsole --noclose -e /usr/bin/env "
                      " LANG='RU_ru' DISPLAY=':1' "
                      "WINEDLLOVERRIDES='libgcc.dll=b,n' "
                      "WINEPREFIX='/home/brezerk/.wine' "
                      "WINESERVER='/usr/bin/wineserver' "
                      "WINELOADER='/usr/bin/wine' "
                      "WINEDLLPATH='/home/develop/win32' "
                      "WINEARCH='win32' "
                      " /usr/bin/wine explorer.exe /desktop=test.app,600x800 "
                      "winecfg --make all --quality=good 2>&1"));

    q4wine::lib::WineVersion* vers = q4wine::lib::WineVersion::getInstance(1);
    vers->setLoader("");
    vers->setServer("");
    vers->setLibs32("");
    vers->setLibs64("");
    vers->save();
    delete(vers);

    prefix->setVersion(q4wine::lib::WineVersion::getInstance(1));

    BOOST_CHECK_EQUAL(prefix->getExecutionString(application.get()),
                      std::string("/usr/bin/konsole --noclose -e /usr/bin/env "
                      " LANG='RU_ru' DISPLAY=':1' "
                      "WINEDLLOVERRIDES='libgcc.dll=b,n' "
                      "WINEPREFIX='/home/brezerk/.wine' "
                      "WINEARCH='win32' "
                      " /usr/bin/wine explorer.exe /desktop=test.app,600x800 "
                      "winecfg --make all --quality=good 2>&1"));

    application->setPath("installer.msi");
    application->setName("installer.msi");

    BOOST_CHECK_EQUAL(prefix->getExecutionString(application.get()),
                      std::string("/usr/bin/konsole --noclose -e /usr/bin/env "
                      " LANG='RU_ru' DISPLAY=':1' "
                      "WINEDLLOVERRIDES='libgcc.dll=b,n' "
                      "WINEPREFIX='/home/brezerk/.wine' "
                      "WINEARCH='win32' "
                      " /usr/bin/wine explorer.exe "
                      "/desktop=installer.msi,600x800 "
                      "msiexec /i installer.msi --make all --quality=good "
                      "2>&1"));

    application->setPath("script.bat");
    application->setName("MySQL RESET");
    application->setVirtualDesktop("");

    BOOST_CHECK_EQUAL(prefix->getExecutionString(application.get()),
                      std::string("/usr/bin/konsole --noclose -e /usr/bin/env "
                      " LANG='RU_ru' DISPLAY=':1' "
                      "WINEDLLOVERRIDES='libgcc.dll=b,n' "
                      "WINEPREFIX='/home/brezerk/.wine' "
                      "WINEARCH='win32' "
                      " /usr/bin/wine  "
                      "wineconsole script.bat --make all --quality=good "
                      "2>&1"));

    application->setUseTerminal(false);
    application->setArgs("--ololo");
    application->setDisplay("");
    application->setPriority(10);
    application->setWineDebug("fixme-all,warn+cursor,+relay");

    BOOST_CHECK_EQUAL(prefix->getExecutionString(application.get()),
                      std::string("  /usr/bin/env  LANG='RU_ru' "
                      "WINEDLLOVERRIDES='libgcc.dll=b,n' "
                      "WINEDEBUG='fixme-all,warn+cursor,+relay' "
                      "WINEPREFIX='/home/brezerk/.wine' "
                      "WINEARCH='win32' "
                      "/usr/bin/nice -n 10"
                      " /usr/bin/wine  wineconsole script.bat "
                      "--ololo 2>&1"));

    std::cout << prefix->getExecutionString(application.get());
}

BOOST_AUTO_TEST_CASE(testWineVersion2) {
    q4wine::lib::WineVersion* vers = q4wine::lib::WineVersion::getInstance(999);
    BOOST_CHECK(vers == NULL);
}

BOOST_AUTO_TEST_SUITE_END()  // End of tests
