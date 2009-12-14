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

		if (root.attribute("version") != APPDB_EXPORT_VERSION){
			qDebug()<<QString("[EE] export_version mismatch! Expected %1, but got %2.").arg(APPDB_EXPORT_VERSION).arg(root.attribute("version"));
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
				case 4:
					  // View test results
					  parseAppTestResultsEntry(node.toElement());
				break;

			}
			node = node.nextSibling();
		}
	}

	file.close();
	return 0;
}

short int XmlParser::getPageCount(void){
	return page_count;
}

short int XmlParser::getPageCurrent(void){
	return page_current;
}

QList<WineAppDBInfo> XmlParser::getAppSearchInfo(){
	return _APPDB_SEARCH_INFO;
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

 void XmlParser::parseAppTestResultsEntry(const QDomElement &element){
	 qDebug()<<element.tagName();
	/* if (element.tagName() == "page"){
		 parsePages(element);
	 } else if (element.tagName() == "app-list"){
		 QDomNode node = element.firstChild();
		 while (!node.isNull()) {
			 parseApp(node.toElement());
			 node = node.nextSibling();
		 }
	 }*/
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
	//QDomNode subnode;
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
				parseAppVersion(subnode.toElement(), appinfo);
				subnode = subnode.nextSibling();
			}
		} else if (node.toElement().tagName()=="bug-list"){
			//FIXME: need bug parse function
			QDomNode subnode = node.toElement().firstChild();
			while (!subnode.isNull()) {
				parseAppVersion(subnode.toElement(), appinfo);
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



bool XmlParser::parseEntry(const QDomElement &element){
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (element.tagName() == "export_version"){
			if (node.toText().data() != APPDB_EXPORT_VERSION){
				qDebug()<<QString("[EE] export_version mismatch! Expected %1, but got %2.").arg(APPDB_EXPORT_VERSION).arg(node.toText().data());
				return false;
			}
		} else if (element.tagName() == "export_action"){
//			_ACTION=node.toText().data().toInt();
		} else if (element.tagName() == "page"){

		}

		if (element.tagName()=="app-list"){
			if (node.toElement().tagName() == "app") {
				parseApp(node.toElement());
			}
		} else if (element.tagName()=="app") {
			if (node.toElement().tagName()=="name"){
				_APPDB_TEST_INFO.name = getChildNodeData(node.firstChild());
				_APPDB_TEST_INFO.id = element.attribute("id").toInt();
				_APPDB_TEST_INFO.ver_id = element.attribute("verid").toInt();
			} else if (node.toElement().tagName()=="desc"){
				_APPDB_TEST_INFO.desc = getChildNodeData(node.firstChild());
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
			} else if (node.toElement().tagName()=="category-list"){
				parseCategoryList(node.toElement());
			} else if (node.toElement().tagName()=="comment-list"){
				parseCommentList(node.toElement());
			} else if (node.toElement().tagName()=="test-result"){
				parseCurrTestResults(node.toElement());
			}
		}

		node = node.nextSibling();
	}
	return true;
}

void XmlParser::clear(){
	action=0;
	page_current=0;
	page_count=0;
	_APPDB_SEARCH_INFO.clear();
	_APPDB_TEST_INFO.appver="";
	_APPDB_TEST_INFO.bugs.clear();
	_APPDB_TEST_INFO.comment="";
	_APPDB_TEST_INFO.desc="";
	_APPDB_TEST_INFO.id=0;
	_APPDB_TEST_INFO.license="";
	_APPDB_TEST_INFO.name="";
	_APPDB_TEST_INFO.nottested="";
	_APPDB_TEST_INFO.notworks="";
	_APPDB_TEST_INFO.rating="";
	_APPDB_TEST_INFO.tests.clear();
	_APPDB_TEST_INFO.category.clear();
	_APPDB_TEST_INFO.url="";
	_APPDB_TEST_INFO.winever="";
	_APPDB_TEST_INFO.works="";
	_APPDB_TEST_INFO.comments.clear();

	return;
}

void XmlParser::parseCurrTestResults(const QDomElement &element){
	QDomNode node = element.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName()=="rating"){
			_APPDB_TEST_INFO.rating = getChildNodeData(node.firstChild());
			_APPDB_TEST_INFO.test_id = element.attribute("id").toInt();
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
		}
		node = node.nextSibling();
	}
	return;
}

void XmlParser::parseCommentList(const QDomElement &element){
	QDomNode node = element.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName()=="comment"){
			parseComment(node.toElement());
		}
		node = node.nextSibling();
	}
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

void XmlParser::parseCategoryList(const QDomElement &element){

	QDomNode node = element.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName()=="category"){
			parseCategory(node.toElement());
		}
		node = node.nextSibling();
	}
	return;
}

void XmlParser::parseComment(const QDomElement &element){
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

	_APPDB_TEST_INFO.comments.append(comment);
	return;
}

void XmlParser::parseCategory(const QDomElement &element){
	WineAppDBCategory category;
	QDomNode node = element.firstChild();

	while (!node.isNull()) {
		if (node.toElement().tagName()=="name"){
			category.name = getChildNodeData(node.firstChild());
			category.id = element.attribute("id").toInt();
		}
		node = node.nextSibling();
	}

	_APPDB_TEST_INFO.category.append(category);

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


QString XmlParser::getChildNodeData(const QDomNode &childNode){
	QString data;

	if (childNode.nodeType() == QDomNode::TextNode) {
		data = childNode.toText().data();
	}
	return data;
}

