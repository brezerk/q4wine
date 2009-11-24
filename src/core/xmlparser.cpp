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

XmlParser::XmlParser(QString fileName)
{
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	QDomDocument doc;
	if (doc.setContent(&file)) {
		QDomElement root = doc.documentElement();
		if (root.tagName() != "appdb_export") {
			qDebug()<<"[EE] File is not a q4wine appdb export file";
			return;
		}

		QDomNode node = root.firstChild();
		while (!node.isNull()) {
			if (!parseEntry(node.toElement())){
				file.close();
				return;
			}
			node = node.nextSibling();
		}

	}

	file.close();
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
			_ACTION=node.toText().data().toInt();
		} else if (element.tagName() == "page"){
			if (node.toElement().tagName()=="current"){
				_PAGE_CURRENT=getChildNodeData(node.firstChild()).toInt();
			} else if (node.toElement().tagName()=="count"){
				_PAGE_COUNT=getChildNodeData(node.firstChild()).toInt();
			}
		}

		if (node.toElement().tagName() == "app") {
			parseApp(node.toElement());
		}
		node = node.nextSibling();
	}
	return true;
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

