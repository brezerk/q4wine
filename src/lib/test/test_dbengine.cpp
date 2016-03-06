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
#define BOOST_TEST_MODULE "testDBEngine"
#include <boost/test/unit_test.hpp>

#include <string>

#include "src/lib/defines.hpp"
#include "src/lib/db.hpp"

BOOST_AUTO_TEST_SUITE(testSuiteDBEngine)

BOOST_AUTO_TEST_CASE(testDBEngine) {
    std::unique_ptr<q4wine::lib::DBEngine> db = \
    std::unique_ptr<q4wine::lib::DBEngine>(
                q4wine::lib::DBEngine::getInstance());
    // db->open("/tmp/q4wine.db");
    db->open(":memory:");
    db->init();
    db->exec(std::string("INSERT INTO test (foo, bar) VALUES (?,?)"),
             { std::string("lol"),
             std::string("kek") });

    db->exec(std::string("INSERT INTO test (foo, bar) VALUES (?,?)"),
             { std::string("wow"),
             std::string("zap") });

    q4wine::lib::rows rows = db->select(std::string("SELECT * FROM test"));

    BOOST_CHECK_EQUAL(rows.size(), intptr_t(2));
    BOOST_CHECK_EQUAL(rows[0]["id"], std::string("1"));
    BOOST_CHECK_EQUAL(rows[0]["foo"], std::string("lol"));
    BOOST_CHECK_EQUAL(rows[0]["bar"], std::string("kek"));

    BOOST_CHECK_EQUAL(rows[1]["id"], std::string("2"));
    BOOST_CHECK_EQUAL(rows[1]["foo"], std::string("wow"));
    BOOST_CHECK_EQUAL(rows[1]["bar"], std::string("zap"));

    db->exec(std::string("UPDATE test SET foo = ?, bar = ? WHERE id = ?"),
             { std::string("lol2"),
             std::string("kek2"),
             std::string("1") });

    q4wine::lib::result res = db->select_one(
                std::string("SELECT * FROM test"));

    BOOST_CHECK_EQUAL(res["id"], std::string("1"));
    BOOST_CHECK_EQUAL(res["foo"], std::string("lol2"));
    BOOST_CHECK_EQUAL(res["bar"], std::string("kek2"));
}

BOOST_AUTO_TEST_SUITE_END()  // End of tests
