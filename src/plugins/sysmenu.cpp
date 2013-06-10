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


#include "sysmenu.h"

system_menu::system_menu()
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

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    QString home_path = QDir::homePath();

    base_directory = QString("%1/.local/share/desktop-directories").arg(home_path);
    base_icon = QString("%1/.local/share/applications").arg(home_path);
    base_menu = QString("%1/.config/menus/applications-merged/%2.menu").arg(home_path).arg(APP_SHORT_NAME);
}

bool system_menu::add_dom_icons(QDomDocument & menu_xml, QDomElement & root, const QString prefix_name, const QString dir_name, const QStringList & iconsList){
    if (iconsList.count() <= 0)
        return true;

    QString entry;
    if (prefix_name.isEmpty()){
        entry = APP_SHORT_NAME;
    } else {
        entry = prefix_name;
        if (!dir_name.isEmpty()){
            entry.append("-");
            entry.append(dir_name);
        }
    }

    QDomElement nroot = menu_xml.createElement("Include");
    for (int m = 0; m < iconsList.size(); ++m) {
        QString ientry = entry;
        ientry.append("-");
        ientry.append(iconsList.at(m));
        ientry.append(".desktop");
        QDomElement name_tag = menu_xml.createElement("Filename");
        nroot.appendChild(name_tag);
        QDomText name_text = menu_xml.createTextNode(QString("%1-%2").arg(APP_SHORT_NAME).arg(ientry));
        name_tag.appendChild(name_text);
    }
    root.appendChild(nroot);

    return true;
}

QDomElement system_menu::create_dom_menu(QDomDocument & menu_xml, QDomElement & root, const QString prefix_name, const QString dir_name){
    QString entry;
    if (prefix_name.isEmpty()){
        entry = APP_SHORT_NAME;
    } else {
        entry = prefix_name;
        if (!dir_name.isEmpty()){
            entry.append("-");
            entry.append(dir_name);
        }
    }
    QDomElement nroot = menu_xml.createElement("Menu");
    QDomElement name_tag = menu_xml.createElement("Name");
    nroot.appendChild(name_tag);
    QDomText name_text = menu_xml.createTextNode(QString("%1-%2").arg(APP_SHORT_NAME).arg(entry));
    name_tag.appendChild(name_text);
    QDomElement dir_tag = menu_xml.createElement("Directory");
    nroot.appendChild(dir_tag);
    QDomText dir_text = menu_xml.createTextNode(QString("%1-%2.directory").arg(APP_SHORT_NAME).arg(entry));
    dir_tag.appendChild(dir_text);

    root.appendChild(nroot);
    return nroot;
}

QString system_menu::get_dir_path(const QString prefix_name, const QString dir_name){
    QStringList path;
    path << base_icon;
    path << APP_SHORT_NAME;

    if (!prefix_name.isEmpty()){
        path << prefix_name;
        if (!dir_name.isEmpty()){
            path << dir_name;
        }
    }

    return path.join("/");
}

QString system_menu::get_dirfile_path(const QString prefix_name, const QString dir_name){
    QStringList path;
    QStringList name;

    path << base_directory;
    name << APP_SHORT_NAME;

    if (!prefix_name.isEmpty()){
        name << prefix_name;
        if (!dir_name.isEmpty()){
            name << dir_name;
        }
    } else {
        name << APP_SHORT_NAME;
    }

    name.last() = QString("%1.directory").arg(name.last());

    return path.join("/").append("/").append(name.join("-"));
}

bool system_menu::move_dir_info(const QString prefix_name, const QString dir_name, const QString new_name) {
    QString file_path;
    QString dir_path;
    QString dir_path_new;

    dir_path = get_dir_path(prefix_name, dir_name);
    dir_path_new = get_dir_path(prefix_name, new_name);
    file_path = get_dirfile_path(prefix_name, dir_name);

#ifdef DEBUG
    qDebug()<<"mv_dir_info:"<<file_path<<dir_path<<dir_path_new;
#endif

    QFile file(file_path);
    if (!file.exists())
        return false;

    if (!file.remove()){
        return false;
    }

    QDir dir(dir_path);
    if (!dir.exists())
        return false;

    if (!dir.rename(dir_path, dir_path_new))
        return false;

    if (!dir_name.isEmpty()){
        this->create_dir_info(prefix_name, new_name);
    } else {
        this->create_dir_info(new_name);
    }

    return true;
}

bool system_menu::remove_dir_info(const QString prefix_name, const QString dir_name) {
    QString file_path;
    QString dir_path;

    dir_path = get_dir_path(prefix_name, dir_name);
    file_path = get_dirfile_path(prefix_name, dir_name);

#ifdef DEBUG
    qDebug()<<"[DD] rm_dir_info:"<<file_path<<dir_path;
#endif

    QFile file(file_path);
    file.remove();

    if (dir_name.isEmpty()){
        QDir dir(base_directory);
        QRegExp rx(QString("%1-%2-.*\\.directory").arg(APP_SHORT_NAME).arg(prefix_name));
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            if (rx.exactMatch(list.at(i).fileName())){
#ifdef DEBUG
                qDebug()<<"[DD]"<<list.at(i).fileName();
#endif
                dir.remove(list.at(i).absoluteFilePath());
            }
        }
    }

    QDir dir(dir_path);
    if (dir.exists()){
        CoreLib->removeDirectory(dir_path);
        dir.rmdir(dir_path);
    }
    return true;
}

bool system_menu::create_dir_info(const QString prefix_name, const QString dir_name) {
    QString name;
    QString icon;

    QString file_path;
    QString dir_path;

    if (prefix_name.isEmpty()){
        name = APP_SHORT_NAME;
        icon = APP_SHORT_NAME;
    } else {
        if (dir_name.isEmpty()){
            name = prefix_name;
            icon = "folder";
        } else {
            name = dir_name;
            icon = "folder";
        }
    }

    dir_path = get_dir_path(prefix_name, dir_name);
    file_path = get_dirfile_path(prefix_name, dir_name);

#ifdef DEBUG
    qDebug()<<"[DD] cr_dir_info:"<<file_path<<dir_path;
#endif

    QFile file(file_path);
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << "[Desktop Entry]\nType=Directory\nName=" << name << "\nIcon=" << icon <<"\n";

    file.close();

    QDir dir(dir_path);
    if (!dir.exists()){
        if (!dir.mkdir(dir_path))
            return false;
    }

    return true;
}

bool system_menu::writeXMLSystemMenu(){

    menu_xml = QDomDocument("Menu PUBLIC \"-//freedesktop//DTD Menu 1.0//EN\" \"http://www.freedesktop.org/standards/menu-spec/menu-1.0.dtd\"");
    QDomElement root = menu_xml.createElement("Menu");
    menu_xml.appendChild(root);
    QDomElement tag = menu_xml.createElement("Name");
    root.appendChild(tag);
    QDomText t = menu_xml.createTextNode("Applications");
    tag.appendChild(t);

    QDomElement root_l0 = this->create_dom_menu(menu_xml, root);
    this->create_dir_info();

    QStringList list = db_prefix.getPrefixList();
    for (int i = 0; i < list.size(); ++i) {
        QStringList iconsList=db_icon.getIconsList(list.at(i), "", "");
        QDomElement root_l1 = this->create_dom_menu(menu_xml, root_l0, list.at(i));
        this->add_dom_icons(menu_xml, root_l1, list.at(i), "", iconsList);
        QStringList subresult = db_dir.getDirList(list.at(i));
        for (int j = 0; j < subresult.size(); ++j) {
            QDomElement root_l2 = this->create_dom_menu(menu_xml, root_l1, list.at(i), subresult.at(j));
            QStringList iconsList=db_icon.getIconsList(list.at(i), subresult.at(j), "");
            this->add_dom_icons(menu_xml, root_l2, list.at(i), subresult.at(j), iconsList);
        }
    }

    QFile file(this->base_menu);
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out<<menu_xml.toString()<<endl;

    file.close();

    return true;
}

bool system_menu::generateSystemMenu(const QString prefix_name, const QString dir_name){
    QStringList prefixes;
    QStringList subdirs;

    if (prefix_name.isEmpty()){
        prefixes << db_prefix.getPrefixList();
    } else {
        prefixes << prefix_name;
    }

    for (int i = 0; i < prefixes.size(); ++i) {
        this->create_dir_info(prefixes.at(i));
        QStringList iconsList=db_icon.getIconsList(prefixes.at(i), "", "");
        for (int m = 0; m < iconsList.size(); ++m) {
            CoreLib->createDesktopFile(prefixes.at(i), "", iconsList.at(m), true);
        }

        if (dir_name.isEmpty()){
            subdirs << db_dir.getDirList(prefixes.at(i));
        } else {
            subdirs << dir_name;
        }

        for (int j = 0; j < subdirs.size(); ++j) {
            this->create_dir_info(prefixes.at(i), subdirs.at(j));
            QStringList iconsList=db_icon.getIconsList(prefixes.at(i), subdirs.at(j), "");
            for (int z = 0; z < iconsList.size(); ++z) {
                CoreLib->createDesktopFile(prefixes.at(i), subdirs.at(j), iconsList.at(z), true);
            }
        }
    }

    return writeXMLSystemMenu();
}

bool system_menu::wipeSystemMenu(){
    QFile file(this->base_menu);
    file.remove();

    QDir dir(base_directory);
    QRegExp rx(QString("%1-.*\\.directory").arg(APP_SHORT_NAME));
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        if (rx.exactMatch(list.at(i).fileName())){
#ifdef DEBUG
            qDebug()<<"[DD] Wipe: "<<list.at(i).fileName();
#endif
            dir.remove(list.at(i).absoluteFilePath());
        }
    }

    QString dir_path = QString("%1/%2").arg(base_icon).arg(APP_SHORT_NAME);
    dir.setPath(dir_path);
    if (dir.exists()){
        CoreLib->removeDirectory(dir_path);
        dir.rmdir(dir_path);
    }

    return true;
}
