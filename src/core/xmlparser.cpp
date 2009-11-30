/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#include "xmlparser.h"

XmlParser::XmlParser(void)
{
	return;
}


int XmlParser::parseIOSream(QString fileName){
	clear();
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly)) {
		return 1;
	}

	QDomDocument doc;
	if (doc.setContent(&file)) {
		QDomElement root = doc.documentElement();
		if (root.tagName() != "appdb_export") {
			qDebug()<<"[EE] File is not a q4wine appdb export file";
			return 2;
		}

		QDomNode node = root.firstChild();
		while (!node.isNull()) {
			if (!parseEntry(node.toElement())){
				file.close();
				return 3;
			}
			node = node.nextSibling();
		}

	}

	file.close();
	return 0;
}

bool XmlParser::parseEntry(const QDomElement &element){
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (element.tagName() == "export_version"){
			if (node.toText().data() != APPDB_EXPORT_VERSION){
				qDebug()<<QString("[EE] export_version mismatch! Expected %1, but got %2.").arg(APPDB_EXPORT_VERSION).arg(node.toText().data());
				return false;
			}
		} else if (element.tagName() == "export_action"){
			_ACTION=node.toText().data().toInt();
		} else if (element.tagName() == "page"){
			if (node.toElement().tagName()=="current"){
				_PAGE_CURRENT=getChildNodeData(node.firstChild()).toInt();
			} else if (node.toElement().tagName()=="count"){
				_PAGE_COUNT=getChildNodeData(node.firstChild()).toInt();
			}
		}

		if (element.tagName()=="app-list"){
			if (node.toElement().tagName() == "app") {
				parseApp(node.toElement());
			}
		} else if (element.tagName()=="app") {
			if (node.toElement().tagName()=="name"){
				_APPDB_TEST_INFO.name = getChildNodeData(node.firstChild());
				_APPDB_TEST_INFO.id = element.attribute("id").toInt();
			} else if (node.toElement().tagName()=="desc"){
				_APPDB_TEST_INFO.desc = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="category"){
				_APPDB_TEST_INFO.category = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="url"){
				_APPDB_TEST_INFO.url = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="app-ver"){
				_APPDB_TEST_INFO.appver = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="rating"){
				_APPDB_TEST_INFO.rating = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="wine-ver"){
				_APPDB_TEST_INFO.winever = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="works"){
				_APPDB_TEST_INFO.works = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="not-works"){
				_APPDB_TEST_INFO.notworks = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="not-tested"){
				_APPDB_TEST_INFO.nottested = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="comment"){
				_APPDB_TEST_INFO.comment = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="license"){
				_APPDB_TEST_INFO.license = getChildNodeData(node.firstChild());
			} else if (node.toElement().tagName()=="test-list"){
				parseTestList(node.toElement());
			} else if (node.toElement().tagName()=="bug-list"){
				parseBugList(node.toElement());
			}
		}

		node = node.nextSibling();
	}
	return true;
}

void XmlParser::clear(){
	_ACTION=0;
	_PAGE_CURRENT=0;
	_PAGE_COUNT=0;
	_APPDB_SEARCH_INFO.clear();
	_APPDB_TEST_INFO.appver="";
	_APPDB_TEST_INFO.bugs.clear();
	_APPDB_TEST_INFO.category="";
	_APPDB_TEST_INFO.comment="";
	_APPDB_TEST_INFO.desc="";
	_APPDB_TEST_INFO.id=0;
	_APPDB_TEST_INFO.license="";
	_APPDB_TEST_INFO.name="";
	_APPDB_TEST_INFO.nottested="";
	_APPDB_TEST_INFO.notworks="";
	_APPDB_TEST_INFO.rating="";
	_APPDB_TEST_INFO.tests.clear();
	_APPDB_TEST_INFO.url="";
	_APPDB_TEST_INFO.winever="";
	_APPDB_TEST_INFO.works="";

	return;
}

void XmlParser::parseBugList(const QDomElement &element){
	QDomNode node = element.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName()=="bug"){
			parseBug(node.toElement());
		}
		node = node.nextSibling();
	}
	return;
}

void XmlParser::parseBug(const QDomElement &element){
	WineAppDBBug bug;
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (node.toElement().tagName()=="desc"){
			bug.desc = getChildNodeData(node.firstChild());
			bug.id = element.attribute("id").toInt();
		} else if (node.toElement().tagName()=="status"){
				bug.status = getChildNodeData(node.firstChild()).toInt();
		} else if (node.toElement().tagName()=="resolution"){
				bug.resolution = getChildNodeData(node.firstChild()).toInt();
		}

		node = node.nextSibling();
	}

	_APPDB_TEST_INFO.bugs.append(bug);

	return;
}


void XmlParser::parseTestList(const QDomElement &element){
	QDomNode node = element.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName()=="test"){
			parseTest(node.toElement());
		}
		node = node.nextSibling();
	}
	return;
}

void XmlParser::parseTest(const QDomElement &element){
	WineAppDBTestResult appresults;
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (node.toElement().tagName()=="current"){
			if (getChildNodeData(node.firstChild()).toInt()==1){
				appresults.current = true;
			} else {
				appresults.current = false;
			}
			appresults.id = element.attribute("id").toInt();
		} else if (node.toElement().tagName()=="distrib"){
				appresults.distrib = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="date"){
				appresults.date = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="wine"){
				appresults.winever = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="install"){
			if (getChildNodeData(node.firstChild()).toInt()==1){
				appresults.install = true;
			} else {
				appresults.install = false;
			}
		} else if (node.toElement().tagName()=="run"){
			if (getChildNodeData(node.firstChild()).toInt()==1){
				appresults.run = true;
			} else {
				appresults.run = false;
			}
		} else if (node.toElement().tagName()=="rating"){
				appresults.rating = getChildNodeData(node.firstChild()).toInt();
		}

		node = node.nextSibling();
	}

	_APPDB_TEST_INFO.tests.append(appresults);

	return;
}

void XmlParser::parseApp(const QDomElement &element){
	QDomNode node = element.firstChild();
	WineAppDBInfo appinfo;

	while (!node.isNull()) {
		if (node.toElement().tagName()=="name"){
			appinfo.name = getChildNodeData(node.firstChild());
			appinfo.id = element.attribute("id").toInt();
		} else if (node.toElement().tagName()=="desc"){
			appinfo.desc = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="category"){
			appinfo.category = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="version-list"){
			parseAppVersionsList(node.toElement(), appinfo);
		} else if (node.toElement().tagName()=="bug-list"){
			parseAppVersionsList(node.toElement(), appinfo);
		}
		node = node.nextSibling();
	}

	this->_APPDB_SEARCH_INFO.append(appinfo);
}

void XmlParser::parseAppVersionsList(const QDomElement &element, WineAppDBInfo &appinfo){
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (node.toElement().tagName()=="version"){
			parseAppVersion(node.toElement(), appinfo);
		}
		node = node.nextSibling();
	}
	return;
}

void XmlParser::parseAppVersion(const QDomElement &element, WineAppDBInfo &appinfo){
	QDomNode node = element.firstChild();
	WineAppDBVersionInfo versioninfo;

	while (!node.isNull()) {
		if (node.toElement().tagName()=="app-ver"){
			versioninfo.appver=getChildNodeData(node.firstChild());
			versioninfo.id = element.attribute("id").toInt();
		} else if (node.toElement().tagName()=="rating"){
			versioninfo.rating=getChildNodeData(node.firstChild()).toInt();
		} else if (node.toElement().tagName()=="wine-ver"){
			versioninfo.winever=getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="url"){
			versioninfo.url=getChildNodeData(node.firstChild());
		}
		node = node.nextSibling();
	}
	appinfo.versions.append(versioninfo);
	return;
}

QString XmlParser::getChildNodeData(const QDomNode &childNode){
	QString data;

	if (childNode.nodeType() == QDomNode::TextNode) {
		data = childNode.toText().data();
	}
	return data;
}

