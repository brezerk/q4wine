/***************************************************************************
 *   Copyright (C) 2008-2013 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include "xmlparser.h"

XmlParser::XmlParser(void)
{
	return;
}

int XmlParser::parseIOSream(QString file){
	clear();

	file = file.trimmed();

	QDomDocument doc;
	if (doc.setContent(file)) {
		QDomElement root = doc.documentElement();

		if (root.tagName() != "appdb_export") {
#ifdef DEBUG
			qDebug()<<"[EE] File does not have Q4Wine AppDB export format";
#endif
			return 2;
		}

		if (root.attribute("version") != APPDB_EXPORT_VERSION){
#ifdef DEBUG
			qDebug()<<QString("[EE] export_version mismatch! Expected \"%1\", but got \"%2\".").arg(APPDB_EXPORT_VERSION).arg(root.attribute("version"));
#endif
			return 3;
		}

		action = root.attribute("action");

		QDomNode node = root.firstChild();
		while (!node.isNull()) {
			if (action=="searchApp"){
				// Search action
				parseAppSearchEntry(node.toElement());
			} else if (action=="viewApp") {
				// View app
				parseApp(node.toElement());
			} else if (action=="viewTest") {
				// View test results
				parseApp(node.toElement());
			} else if (action=="viewCategory") {
				parseAppCategoryEntry(node.toElement());
			} else if (action=="viewError") {
				return 6;
			} else if (action=="viewMessage") {
				return parseMessageEntry(node.toElement());
			}
			node = node.nextSibling();
		}
	} else {
		return 2;
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

int XmlParser::parseMessageEntry(const QDomElement &element){
	QDomNode node = element.firstChild();

	if (element.tagName() == "code"){
		return getChildNodeData(element.firstChild()).toInt();
	}

	return 6;
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
			if (element.attribute("verid").toInt()>0){
				appinfo.ver_id=element.attribute("verid").toInt();
			} else {
				appinfo.ver_id=0;
			}
		} else if (node.toElement().tagName()=="desc"){
			appinfo.desc = getChildNodeData(node.firstChild());
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

	if (action=="searchApp"){
		// Search action
		_APPDB_SEARCH_INFO.append(appinfo);
	} else if (action=="viewApp") {
		// View app
		appdb_appinfo = appinfo;
	} else if (action=="viewTest") {
		appdb_appinfo = appinfo;
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
			versioninfo.rating=getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="wine-ver"){
			versioninfo.winever=getChildNodeData(node.firstChild());
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
				bug.status = getChildNodeData(node.firstChild());
		} else if (node.toElement().tagName()=="resolution"){
				bug.resolution = getChildNodeData(node.firstChild());
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
				appresults.rating = getChildNodeData(node.firstChild());
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
			comment.parent_id = getChildNodeData(node.firstChild()).toInt();
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
		data.replace(QRegExp("(<br />\\s*)+"), "<br /><br />");
		data.replace(QRegExp("&nbsp;"), "");
		data.replace(QRegExp("(<br />)+\\s*$"), "");
		data.replace(QRegExp("^\\s*(<br />)+"), "");
	}
	return data;
}

void XmlParser::clear(){
	action="";
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



