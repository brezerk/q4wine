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
			qDebug()<<"[EE] File is not a q4wine appdb export format";
			return 2;
		}

		if (root.attribute("version") != APPDB_EXPORT_VERSION){
			qDebug()<<QString("[EE] export_version mismatch! Expected \"%1\", but got \"%2\".").arg(APPDB_EXPORT_VERSION).arg(root.attribute("version"));
			return 3;
		}

		action = root.attribute("action").toInt();

		QDomNode node = root.firstChild();
		while (!node.isNull()) {
			switch (action){
				case 1:
					  // Search action
					  parseAppSearchEntry(node.toElement());
				break;
				case 5:
					  parseAppCategoryEntry(node.toElement());
				break;
				case 4:
					  // View test results
					  parseApp(node.toElement());
				break;
				case 3:
					 // View test results
					 parseApp(node.toElement());
				break;

			}
			node = node.nextSibling();
		}
	}

	file.close();
	return 0;
}


int XmlParser::parseIOSream2(QString file){
	clear();

	file = file.trimmed();

	QDomDocument doc;
	if (doc.setContent(file)) {
		QDomElement root = doc.documentElement();
		if (root.tagName() != "appdb_export") {
			qDebug()<<"[EE] File is not a q4wine appdb export format";
			return 2;
		}

		if (root.attribute("version") != APPDB_EXPORT_VERSION){
			qDebug()<<QString("[EE] export_version mismatch! Expected \"%1\", but got \"%2\".").arg(APPDB_EXPORT_VERSION).arg(root.attribute("version"));
			return 3;
		}

		action = root.attribute("action").toInt();

		QDomNode node = root.firstChild();
		while (!node.isNull()) {
			switch (action){
				case 1:
					  // Search action
					  parseAppSearchEntry(node.toElement());
				break;
				case 5:
					  parseAppCategoryEntry(node.toElement());
				break;
				case 4:
					  // View test results
					  parseApp(node.toElement());
				break;
				case 3:
					 // View test results
					 parseApp(node.toElement());
				break;

			}
			node = node.nextSibling();
		}
	}

	return 0;
}

short int XmlParser::getPageCount(void){
	return page_count;
}

short int XmlParser::getPageCurrent(void){
	return page_current;
}

QList<WineAppDBInfo> XmlParser::getAppSearchInfoList(){
	return _APPDB_SEARCH_INFO;
}

WineAppDBInfo XmlParser::getAppSearchInfo(){
	return appdb_appinfo;
}

QList<WineAppDBCategory> XmlParser::getCategorysList(){
	return appdb_categorys;
}

QList<WineAppDBCategory> XmlParser::getSubCategorysList(){
	return appdb_subcategorys;
}

QList<WineAppDBCategory> XmlParser::getAppsList(){
	return appdb_apps;
}

void XmlParser::parseAppSearchEntry(const QDomElement &element){
	if (element.tagName() == "page"){
		parsePages(element);
	} else if (element.tagName() == "app-list"){
		QDomNode node = element.firstChild();
		while (!node.isNull()) {
			parseApp(node.toElement());
			node = node.nextSibling();
		}
	}
	return;
}

void XmlParser::parseAppCategoryEntry(const QDomElement &element){
	if (element.tagName() == "category-list"){
		QDomNode subnode = element.firstChild();
		while (!subnode.isNull()) {
			if (subnode.toElement().tagName()=="category")
				appdb_categorys.append(parseCategory(subnode.toElement()));
			subnode = subnode.nextSibling();
		}
	} else if (element.tagName() == "subcategory-list"){
		QDomNode subnode = element.firstChild();
		while (!subnode.isNull()) {
			if (subnode.toElement().tagName()=="category")
				appdb_subcategorys.append(parseCategory(subnode.toElement()));
			subnode = subnode.nextSibling();
		}
	} else if (element.tagName() == "app-list"){
		QDomNode subnode = element.firstChild();
		while (!subnode.isNull()) {
			if (subnode.toElement().tagName()=="app")
				appdb_apps.append(parseCategory(subnode.toElement()));
			subnode = subnode.nextSibling();
		}
	}
	return;
}

void XmlParser::parsePages(const QDomElement &element){
	QDomNode node = element.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName()=="current"){
			page_current=getChildNodeData(node.firstChild()).toInt();
		} else if (node.toElement().tagName()=="count"){
			page_count=getChildNodeData(node.firstChild()).toInt();
		}
		node = node.nextSibling();
	}
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
			QDomNode subnode = node.toElement().firstChild();
			while (!subnode.isNull()) {
				if (subnode.toElement().tagName()=="version")
					  parseAppVersion(subnode.toElement(), appinfo);
				subnode = subnode.nextSibling();
			}
		} else if (node.toElement().tagName()=="url"){
			appinfo.url = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="app-ver"){
			appinfo.appver = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="rating"){
			appinfo.rating = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="wine-ver"){
			appinfo.winever = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="works"){
			appinfo.works = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="not-works"){
			appinfo.notworks = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="not-tested"){
			appinfo.nottested = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="comment"){
			appinfo.comment = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="license"){
			appinfo.license = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="test-result"){
			parseTestResults(node.toElement(), appinfo);
		} else if (node.toElement().tagName()=="bug-list"){
			QDomNode subnode = node.toElement().firstChild();
			while (!subnode.isNull()) {
				if (subnode.toElement().tagName()=="bug")
					parseBug(subnode.toElement(), appinfo);
				subnode = subnode.nextSibling();
			}
		} else if (node.toElement().tagName()=="category-list"){
			QDomNode subnode = node.toElement().firstChild();
			while (!subnode.isNull()) {
				if (subnode.toElement().tagName()=="category")
					appinfo.categorys.append(parseCategory(subnode.toElement()));
				subnode = subnode.nextSibling();
			}
		} else if (node.toElement().tagName()=="comment-list"){
			QDomNode subnode = node.toElement().firstChild();
			while (!subnode.isNull()) {
				if (subnode.toElement().tagName()=="comment")
					parseComment(subnode.toElement(), appinfo);
				subnode = subnode.nextSibling();
			}
		} else if (node.toElement().tagName()=="test-list"){
			QDomNode subnode = node.toElement().firstChild();
			while (!subnode.isNull()) {
				if (subnode.toElement().tagName()=="test")
					parseTest(subnode.toElement(), appinfo);
				subnode = subnode.nextSibling();
			}
		}
		node = node.nextSibling();
	}
	switch (action){
		case 1:
			  //Add to search struct
			 _APPDB_SEARCH_INFO.append(appinfo);
		break;
		case 3:
			 appdb_appinfo = appinfo;
		break;
		case 4:
			 appdb_appinfo = appinfo;
		break;
	}
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

WineAppDBCategory XmlParser::parseCategory(const QDomElement &element){
	WineAppDBCategory category;
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (node.toElement().tagName()=="name"){
			category.name = getChildNodeData(node.firstChild());
			category.id = element.attribute("id").toInt();
			category.enabled = true;
		} else if (node.toElement().tagName()=="enabled"){
			if (getChildNodeData(node.firstChild())=="0"){
				category.enabled = false;
			} else {
				category.enabled = true;
			}
		} else if (node.toElement().tagName()=="desc"){
			category.desc = getChildNodeData(node.firstChild());
		}
		node = node.nextSibling();
	}

	return (category);
}

void XmlParser::parseTestResults(const QDomElement &element, WineAppDBInfo &appinfo){
	QDomNode node = element.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName()=="rating"){
			appinfo.rating = getChildNodeData(node.firstChild());
			appinfo.test_id = element.attribute("id").toInt();
		} else if (node.toElement().tagName()=="wine-ver"){
			appinfo.winever = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="works"){
			appinfo.works = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="not-works"){
			appinfo.notworks = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="not-tested"){
			appinfo.nottested = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="comment"){
			appinfo.comment = getChildNodeData(node.firstChild());
		}
		node = node.nextSibling();
	}
	return;
}

void XmlParser::parseBug(const QDomElement &element, WineAppDBInfo &appinfo){
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

	appinfo.bugs.append(bug);

	return;
}

void XmlParser::parseTest(const QDomElement &element, WineAppDBInfo &appinfo){
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

	appinfo.tests.append(appresults);

	return;
}

void XmlParser::parseComment(const QDomElement &element, WineAppDBInfo &appinfo){
	WineAppDBComment comment;
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (node.toElement().tagName()=="topic"){
			comment.topic = getChildNodeData(node.firstChild());
			comment.id = element.attribute("id").toInt();
		} else if (node.toElement().tagName()=="date"){
			comment.date = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="autor"){
			comment.autor = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="parent"){
			comment.padent_id = getChildNodeData(node.firstChild()).toInt();
		} else if (node.toElement().tagName()=="message"){
			comment.message = getChildNodeData(node.firstChild());
		}

		node = node.nextSibling();
	}

	appinfo.comments.append(comment);
	return;
}

QString XmlParser::getChildNodeData(const QDomNode &childNode){
	QString data;
	if (childNode.nodeType() == QDomNode::TextNode) {
		data = childNode.toText().data();
	}
	return data;
}

void XmlParser::clear(){
	action=0;
	page_current=0;
	page_count=0;

	_APPDB_SEARCH_INFO.clear();

	appdb_appinfo.appver="";
	appdb_appinfo.bugs.clear();
	appdb_appinfo.comment="";
	appdb_appinfo.desc="";
	appdb_appinfo.id=0;
	appdb_appinfo.license="";
	appdb_appinfo.name="";
	appdb_appinfo.nottested="";
	appdb_appinfo.notworks="";
	appdb_appinfo.rating="";
	appdb_appinfo.tests.clear();
	appdb_appinfo.categorys.clear();
	appdb_appinfo.url="";
	appdb_appinfo.winever="";
	appdb_appinfo.works="";
	appdb_appinfo.comments.clear();
	appdb_appinfo.versions.clear();

	appdb_categorys.clear();
	appdb_apps.clear();
	appdb_subcategorys.clear();

	return;
}



