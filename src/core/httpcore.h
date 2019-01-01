/***************************************************************************
 *   Copyright (C) 2008-2019 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#ifndef HTTPCORE_H
#define HTTPCORE_H

#include "config.h"

#include <memory>

#include <QUrl>
#ifdef QT5
#include <QNetworkAccessManager>
#include <QNetworkReply>
#else
#include <QHttp>
#include <QHttpRequestHeader>
#endif
#include <QNetworkProxy>
#include <QMessageBox>
 #include <QCryptographicHash>

#ifdef DEBUG
#include <QDebug>
#endif

#include "q4wine-lib.h"

class HttpCore : public QObject
{
Q_OBJECT
public:
	HttpCore();
	~HttpCore();

	void getAppDBXMLPage(QString host, short int port, QString page);
	QString getXMLReply();
signals:
	void pageReaded();
	void requestError(QString);
	void updateDataReadProgress(int, int);
	void stateChanged (int);

private slots:
#ifdef QT5
    void readResponseHeader(QNetworkReply* reply);
#else
    void httpRequestFinished(int requestId, bool error);
	void readResponseHeader(const QHttpResponseHeader &responseHeader);
#endif

private:
#ifdef QT5
    std::unique_ptr<QNetworkAccessManager> http;
#else
	std::unique_ptr<QHttp> http;
#endif
	QString user_agent;
	QString xmlreply;
    QString page;

	int getId;
	bool aborted;
    bool getCacheFile(QString page);

    //! This is need for libq4wine-core.so import;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::unique_ptr<corelib> CoreLib;
    QLibrary libq4wine;

};

#endif // HTTPCORE_H
