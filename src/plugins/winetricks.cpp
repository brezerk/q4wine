/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                              *
 *   pzinin@gmail.com                                                     *
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

#include "winetricks.h"

winetricks::winetricks(QString prefixName) : QWidget()
{
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
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

    this->winetricks_bin = QDir::homePath();
    this->winetricks_bin.append("/.config/");
    this->winetricks_bin.append(APP_SHORT_NAME);
    this->winetricks_bin.append("/winetricks");

    if (!check_script(false)){
        this->winetricks_bin = CoreLib->getWhichOut("winetricks", false);
    }

    this->prefix_name = prefixName;
    this->console_bin = CoreLib->getSetting("console", "bin").toString();
    this->console_args = CoreLib->getSetting("console", "args", false).toString();
}

QStringList winetricks::get_installed() {

    QStringList installed_verbs;
    qstring_map name_val_settings;
    QString winver = nullptr;
    QHash<QString, QString> prefix_info = db_prefix.getByName(this->prefix_name);
    QString logfile = QString("%1/winetricks.log").arg(CoreLib->getStrictEscapeString(prefix_info.value("path")));
    QFile file(logfile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return installed_verbs;
    } else {
        QRegExp winver_regexp("^(nt40|vista|win([0-9][0-9k]?[0-9]?|xp|ver=))$");
        QTextStream textinputstream(&file);
        while (!textinputstream.atEnd()) {
            QString l = textinputstream.readLine();
            if (textinputstream.status() != QTextStream::Ok) {
                file.close();
                return installed_verbs;
            } else if (installed_verbs.contains(l)) {
                // Ignore duplicate
                continue;
            } else if (l.count('=') == 1 && !l.endsWith('=')) {
                // For some "name=value" settings,
                // use map to add the last (valid) one
                QString key = l.section('=', 0, 0);
                QString val = l.section('=', 1, 1);
                // If old "name=value" setting exists, it is overwritten
                name_val_settings[key] = val;
            } else {
                // If the verb is used for setting OS version, add the last one
                if (winver_regexp.indexIn(l) != -1) {
                    winver = l;
                } else {
                    installed_verbs.append(l);
                }
            }
        }
    }
    file.close();
    foreach (QString key, name_val_settings.keys()) {
        installed_verbs.append(QString("%1=%2").arg(key).arg(name_val_settings[key]));
    }
    if (winver != nullptr) {
        installed_verbs.append(winver);
    }

    return installed_verbs;
}

void winetricks::install_winetricks() {
    this->downloadwinetricks();
}

bool winetricks::check_script(bool warn){
    if (!QFile(this->winetricks_bin).exists()){
        if (warn)
            QMessageBox::warning(this, QString(tr("Error")), QString(tr("Cannot locate Winetricks script. Install Winetricks script first.")));
        return false;
    }
    return true;
}

void winetricks::run_winetricks(QString item){
    if (!check_script())
        return;

    if (item.isEmpty())
        return;

#ifdef DEBUG
    qDebug()<<"[plugin] run item";
#endif

   QStringList args;
        if (!console_args.isEmpty()){
                // If we have any conslope parametres, we gona preccess them one by one
                QStringList cons_args = console_args.split(" ");
                for (int i=0; i<cons_args.count(); i++){
                        if (!cons_args.at(i).isEmpty())
                                args.append(cons_args.at(i));
                }
        }

        if (console_bin.split("/").last() == "konsole"){
            args.append("/bin/sh");
            args.append("-c");
        }

        args.append(get_command(item).join(" "));

#ifdef DEBUG
        qDebug()<<"[DD] winetricks args: "<<args;
#endif

    QProcess proc;
    proc.startDetached(console_bin, args, QDir::homePath());

    return;
}

QStringList winetricks::get_command(QString item){
    QStringList sh_args;
    int proxy_type = CoreLib->getSetting("network", "type", false, 0).toInt();
    sh_args.append("env");

    if (proxy_type > 0){
        QString proxy_host = CoreLib->getSetting("network", "host", false).toString();
        QString proxy_auth = CoreLib->getSetting("network", "user", false).toString();
        if (!proxy_auth.isEmpty()){
            QString proxy_pass = CoreLib->getSetting("network", "pass", false).toString();
            proxy_auth.append(QString(":%1@").arg(proxy_pass));
        }
        QString proxy_var = QString("http://%1%2:%3").arg(proxy_auth).arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString());
        sh_args.append(QString("http_proxy='%1'").arg(proxy_var));
        sh_args.append(QString("https_proxy='%1'").arg(proxy_var));
        sh_args.append(QString("ftp_proxy='%1'").arg(proxy_var));
    }

    QHash<QString, QString> prefix_info = db_prefix.getByName(this->prefix_name);

    sh_args << QString("WINEPREFIX='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("path")));

    if (!prefix_info.value("server").isEmpty()){
        sh_args << QString("WINE='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("bin")));
        if (!prefix_info.value("libs").isEmpty())
            sh_args << QString("WINEDLLPATH='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("libs")));
        sh_args << QString("WINELOADER='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("loader")));
        sh_args << QString("WINESERVER='%1'").arg(CoreLib->getStrictEscapeString(prefix_info.value("server")));
    } else {
        QString prefixDllPath;
        Version vers;
        vers.id_ = prefix_info.value("version_id");
        if (vers.load()){
            if (prefix_info.value("arch") == "win32"){
                prefixDllPath = vers.wine_dllpath32_;
            } else if (prefix_info.value("arch") == "win64"){
                prefixDllPath = vers.wine_dllpath64_;
            } else {
                if (vers.wine_dllpath64_.isEmpty()){
                    prefixDllPath = vers.wine_dllpath32_;
                } else {
                    prefixDllPath = vers.wine_dllpath64_;
                }
            }
            sh_args << QString("WINE='%1'").arg(CoreLib->getStrictEscapeString(vers.wine_exec_));
            if (!prefixDllPath.isEmpty())
                sh_args << QString("WINEDLLPATH='%1'").arg(CoreLib->getStrictEscapeString(prefixDllPath));
            sh_args << QString("WINELOADER='%1'").arg(CoreLib->getStrictEscapeString(vers.wine_loader_));
            sh_args << QString("WINESERVER='%1'").arg(CoreLib->getStrictEscapeString(vers.wine_server_));
        }
    }


    if (!prefix_info.value("arch").isEmpty())
        sh_args << QString("WINEARCH='%1'").arg(prefix_info.value("arch"));


    sh_args.append(CoreLib->getWhichOut("sh"));
    sh_args.append("-c");
    sh_args.append(QString("\"%1 --no-isolate %2\"").arg(this->winetricks_bin).arg(item));
    return sh_args;
}

void winetricks::downloadwinetricks () {
    /*
     * Downloading winetricks and installing it
     */
    this->winetricks_bin = QDir::homePath();
    this->winetricks_bin.append("/.config/");
    this->winetricks_bin.append(APP_SHORT_NAME);
    this->winetricks_bin.append("/winetricks");

    QFile file(this->winetricks_bin);

    if (file.exists()){
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){
            QMessageBox::warning(this, QString(tr("Error")), QString(tr("File '%1' is not writable or not readable. Check file permissions.").arg(this->winetricks_bin)));
            return;
        }
    } else {
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){
            QMessageBox::warning(this, QString(tr("Error")), QString(tr("Can't create file '%1'. Check folder permissions.").arg(this->winetricks_bin)));
            return;
        } else {
            file.remove();
        }
    }

    QStringList args;
    if (!console_args.isEmpty()){
        // If we have any conslope parametres, we gona preccess them one by one
        QStringList cons_args = console_args.split(" ");
        for (int i=0; i<cons_args.count(); i++){
            if (!cons_args.at(i).isEmpty())
                args.append(cons_args.at(i));
        }
    }

    QString arg;
    arg.append(CoreLib->getWhichOut("sh"));
    arg.append(" -c \"");

    if (CoreLib->getSetting("network", "type", 0).toInt() != 0){
        QString proxy_host = CoreLib->getSetting("network", "host", false).toString();
        if (!proxy_host.isEmpty()){
            QString proxy_auth = CoreLib->getSetting("network", "user", false).toString();
            if (!proxy_auth.isEmpty()){
                QString proxy_pass = CoreLib->getSetting("network", "pass", false).toString();
                proxy_auth.append(QString(":%1@").arg(proxy_pass));
            }
            QString proxy_var = QString("http://%1%2:%3").arg(proxy_auth).arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString());
            arg.append(QString("env https_proxy='%1' ").arg(proxy_var));
        }
    }

    arg.append(CoreLib->getWhichOut("wget"));
    arg.append(" https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks -O ");
    arg.append(this->winetricks_bin);
    arg.append(" && ");
    arg.append(CoreLib->getWhichOut("chmod"));
    arg.append(" +x ");
    arg.append(this->winetricks_bin);
    arg.append("\"");

    if (console_bin.split("/").last() == "konsole"){
        args.append("/bin/sh");
        args.append("-c");
    }

    args.append(arg);

#ifdef DEBUG
        qDebug()<<"[DD] winetricks exec: "<<console_bin<<args;
#endif
    QProcess proc(this);
    proc.startDetached(console_bin, args);
    #ifdef DEBUG
    qDebug()<<"[plugin] download done";
    #endif
    return;
}

bool winetricks::parse() {
    if (!check_script())
        return false;

    QProgressDialog *pd = new QProgressDialog(tr("Refreshing Winetricks application list."), QString(), 0, 100);
    pd->setModal(true);
    pd->setWindowTitle(tr("Winetricks plugin"));
    pd->setFixedWidth(400);
    pd->show();
    pd->setValue(1);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

    #ifdef DEBUG
        qDebug()<<"[plugin] parsing winetricks script";
    #endif

    pd->setLabelText(tr("Dropping stale items..."));
    db_sysconfig.begin();
    db_sysconfig.drop_items(D_PROVIDER_WINETRICKS);
    db_sysconfig.commit();
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

    pd->setValue(2);
    pd->setLabelText(tr("Getting list of installed items..."));
    QStringList installed_apps = this->get_installed();
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);
    pd->setValue(4);

    metadata_type metadata;
    QString infile = QString(winetricks_bin);
    QFile file(infile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    else
    {
        QTextStream textinputstream(&file);

        pd->setValue(5);
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);

        QString name;
        bool in_metadata = false;
        while (!textinputstream.atEnd())
        {
            QString l = textinputstream.readLine();
            if (textinputstream.status() != QTextStream::Ok)
            {
                file.close();
                return false;
            }
            if (l.startsWith("w_metadata "))
            {
                QStringList wmeta = l.split(' ', QString::SkipEmptyParts);
                name = wmeta.at(1);
                metadata[name]["name"] = name;
                metadata[name]["category"] = wmeta.at(2);
                in_metadata = true;
            }
            else if (l.startsWith("load_"))
            {
                in_metadata = false;
            }
            else if (in_metadata)
            {
                l = l.trimmed();
                if (l.startsWith("title="))
                    metadata[name]["title"] = l.section("\"", 1, 1);
                else if (l.startsWith("publisher="))
                    metadata[name]["publisher"] = l.section("\"", 1, 1);
                else if (l.startsWith("year="))
                    metadata[name]["year"] = l.section("\"", 1, 1);
                else if (l.startsWith("media="))
                    metadata[name]["media"] = l.section("\"", 1, 1);
            }
            QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);
        }
    }
    file.close();
    pd->setValue(10);

    db_sysconfig.begin();

    int all_verbs = metadata.size();
    int added_verb = 0;
    foreach (qstring_map verb, metadata)
    {
        QString name = verb["name"];
        QString desc = verb["title"];
        qstring_map::const_iterator const it_publisher = verb.find("publisher");
        qstring_map::const_iterator const it_year = verb.find("year");
        if (it_publisher != verb.end() && it_year != verb.end())
            desc += " (" + verb["publisher"] + ", " + verb["year"] + ")";
        qstring_map::const_iterator const it_media = verb.find("media");
        if (it_media != verb.end() && verb["media"] == "download")
          desc += " [downloadable]";  // TODO: "cached" flag
        pd->setValue(90 * added_verb / all_verbs + 10);
        pd->setLabelText(tr("Adding verb: %1").arg(name));
        db_sysconfig.add_item(verb["name"], "application-x-ms-dos-executable", desc, verb["category"], D_PROVIDER_WINETRICKS, installed_apps.contains(verb["name"]));
        added_verb++;
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);
    }

    db_sysconfig.commit();

    db_sysconfig.vacuum();

    #ifdef DEBUG
        qDebug()<<"[plugin] parsing winetricks script done";
    #endif

    pd->setValue(100);

    return true;
}



