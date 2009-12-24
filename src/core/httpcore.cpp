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
 ***************************************************************************/

#include "httpcore.h"

HttpCore::HttpCore()
{
	http = new QHttp(this);
	if (!http)
		return;

	connect(http, SIGNAL(done(bool)), this, SLOT(readPage()));
	//requestFinished

	//Create user_agent string
	user_agent=QString("%1/%2 (X11; %3 %4) xmlparser/%5").arg(APP_SHORT_NAME).arg(APP_VERS).arg(APP_HOST).arg(APP_ARCH).arg(APPDB_EXPORT_VERSION);
}

HttpCore::~HttpCore(){
	delete http;
}

void HttpCore::getAppDBXMLPage(QString host, short int port, QString page, QString params)
{
	http->setHost(host, QHttp::ConnectionModeHttp, port);

	QHttpRequestHeader header("POST", page);
	header.setValue("Host", host);
	header.setValue("Port", "80");
	header.setContentType("application/x-www-form-urlencoded");
	header.setValue("Accept-Encoding", "deflate");
	header.setContentLength( params.toUtf8().length() );
	header.setValue("User-Agent", user_agent);

#ifdef DEBUG
	qDebug()<<"[ii] Connecting to"<<host<<":"<<port<<" reuested page is: "<<page<<params.toUtf8();
#endif

	http->request(header, params.toUtf8());
}

QString HttpCore::getXMLReply(){
	return xmlreply;
}

void HttpCore::readPage()
{
	xmlreply=http->readAll();

#ifdef DEBUG
	//qDebug()<<"[ii] Recived page:"<<xmlreply;
#endif

	emit(pageReaded());
}
