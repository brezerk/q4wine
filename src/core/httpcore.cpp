/***************************************************************************
 *   Copyright (C) 2008-2021 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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
    http.reset(new QNetworkAccessManager(this));

    connect(http.get(), SIGNAL(finished(QNetworkReply*)),
            this, SLOT(readResponseHeader(QNetworkReply*)));
    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib class pointer
    CoreLibClassPointer = (CoreLibPrototype*) libq4wine.resolve("createCoreLib");
    CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

    int type = CoreLib->getSetting("network", "type", false).toInt();

    if (type!=0){
        QString proxyUser, proxyPass, proxyHost;
        int proxyPort;

        proxyHost = CoreLib->getSetting("network", "host", false).toString();
        proxyPort = CoreLib->getSetting("network", "port", false).toInt();
        proxyUser = CoreLib->getSetting("network", "user", false).toString();
        proxyPass = CoreLib->getSetting("network", "pass", false).toString();
        QNetworkProxy proxy;
        if (type == 1){
            proxy.setType(QNetworkProxy::HttpProxy);
        } else if (type == 2){
            proxy.setType(QNetworkProxy::Socks5Proxy);
        }
        proxy.setHostName(proxyHost);
        proxy.setPort(proxyPort);
        proxy.setUser(proxyUser);
        proxy.setPassword(proxyPass);
        http->setProxy(proxy);
    }

    //Create user_agent string
    user_agent=QString("%1/%2 (X11; %3 %4) xmlparser/%5").arg(APP_SHORT_NAME).arg(APP_VERS).arg(APP_HOST).arg(APP_ARCH).arg(APPDB_EXPORT_VERSION);
}

HttpCore::~HttpCore(){
    //Nothing but...
}

void HttpCore::getAppDBXMLPage(QString host, short int port, QString page)
{
    this->page = page;
    if (!this->getCacheFile(page)){
        QString url = QString("https://%1:%2/%3").arg(host, QString::number(port), page);
        QNetworkRequest request = QNetworkRequest(QUrl(url));
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::UserAgentHeader, user_agent);
        http->get(request);
#ifdef DEBUG
        qDebug()<<"[ii] Reuested page is: "<<QUrl(url);
#endif

        this->xmlreply="";
    } else {
#ifdef DEBUG
        qDebug()<<"[ii] Cache hit";
#endif
        emit(pageReaded());
    }
}

bool HttpCore::getCacheFile(QString page){
    QString cache_file = corelib::getAppCacheLocation();
    cache_file.append(QCryptographicHash::hash(page.toUtf8().constData(), QCryptographicHash::Md4).toHex());

    QFile file(cache_file);
    if (file.exists()){
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        QTextStream in(&file);
        this->xmlreply.clear();
        while (!in.atEnd()) {
            xmlreply.append(in.readLine());
        }
        return true;
    }

    return false;
}

QString HttpCore::getXMLReply(){
    QString cache_file = corelib::getAppCacheLocation();
    cache_file.append(QCryptographicHash::hash(this->page.toUtf8().constData(), QCryptographicHash::Md4).toHex());

    QFile file(cache_file);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
         QTextStream out(&file);
         out << xmlreply;
     }
    file.close();
    return xmlreply;
}

void HttpCore::readResponseHeader(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status_code == 301 || status_code == 302) {
            QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            QNetworkRequest request = QNetworkRequest(QUrl(possibleRedirectUrl.toString()));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            request.setHeader(QNetworkRequest::UserAgentHeader, user_agent);
            http->get(request);
        }
        xmlreply = reply->readAll();
#ifdef DEBUG
        qDebug()<<"[ii] Received page:"<<xmlreply;
#endif
    }



    emit(pageReaded());
}

