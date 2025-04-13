/***************************************************************************
 *   Copyright (C) 2008-2025 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#ifndef SYSTMENU_H
#define SYSTMENU_H

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include "config.h"
#include "dir.h"
#include "icon.h"
#include "prefix.h"
#include "q4wine-lib.h"

class system_menu {
 public:
  system_menu();

  bool move_dir_info(const QString &prefix_name, const QString &dir_name,
                     const QString &new_name);
  bool create_dir_info(const QString &prefix_name = "",
                       const QString &dir_name = "");
  bool remove_dir_info(const QString &prefix_name,
                       const QString &dir_name = "");

  /*! \brief Generate System Menu (xml, desktop and directory files)
   *
   * \param prefix_name Prefix name (optional)
   * \param dir_name Directory name (optional)
   *
   * \return true on success
   */
  bool generateSystemMenu(const QString &prefix_name = "",
                          const QString &dir_name = "");

  /*! \brief Write xml menu file
   *
   * \return true on success
   */
  bool writeXMLSystemMenu();

  /*! \brief Wipe menu file, dirs and desktop files
   *
   * \return true on success
   */
  bool wipeSystemMenu();

 private:
  //! This is need for libq4wine-core.so import.
  QLibrary libq4wine;
  typedef void *CoreLibPrototype(bool);
  CoreLibPrototype *CoreLibClassPointer;
  std::unique_ptr<corelib> CoreLib;

  /*! \brief Create dom menu structure
   */
  QDomElement create_dom_menu(QDomDocument &menu_xml, QDomElement &root,
                              const QString &prefix_name = "",
                              const QString &dir_name = "");

  /*! \brief Populate menu entry with icons descriptions
   */
  bool add_dom_icons(QDomDocument &menu_xml, QDomElement &root,
                     const QString &prefix_name, const QString &dir_name,
                     const QStringList &iconsList);

  QString get_dir_path(const QString &prefix_name = "",
                       const QString &dir_name = "");
  QString get_dirfile_path(const QString &prefix_name = "",
                           const QString &dir_name = "");

  QDomDocument menu_xml;

  QString base_directory;
  QString base_icon;
  QString base_menu;

  Prefix db_prefix;
  Icon db_icon;
  Dir db_dir;
};

#endif  // SYSMENU_H
