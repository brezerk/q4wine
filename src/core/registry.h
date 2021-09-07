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

/*!
 * \defgroup registry Q4Wine registry core
 * \brief Registry core package provides general registry functions like reading and writing for Wine registry files.
 */

#ifndef REGISTRY_H
#define REGISTRY_H

#include <memory>

#include <config.h>

#include <unistd.h>

#include <QTime>

//q4wine lib
#include "q4wine-lib.h"

/*!
 * \class Registry
 * \ingroup registry
 * \brief This class provides general registry functions for Q4Wine.
 *
 * This class can create registry files to export via Wine regedit.
 * Also this class can read keys value from Wine registry.
 *
 */
class Registry: public QObject
{
    Q_OBJECT
public:
    /*! \brief This is empty class constructor.
      */
    Registry();
    /*! \brief This is class constructor.
      * \param prefixPath Base prefix path. This is need to get wine system registry file path.
      */
    explicit Registry(const QString &prefixPath);

    /*! \brief Creates header information for exported reg file.
      *
      * \return true on success.
      */
    bool init();

    /*! \brief Appends path and keys to exported reg file.
      * \param reg_keys QString of keys and it's values.
      */
    void append(const QString &reg_keys);

    /*! \brief Append set key to exported reg file.
      * \param path  Registry path
      * \param key   Registry key name
      * \param value Registry key value
      */
    void set(const QString &path,
             const QString &key,
             const QString &value,
             const QString &hkey = "HKEY_CURRENT_USER");

    /*! \brief Append set unsetkey to exported reg file.
      * \param path  Registry path
      * \param key   Registry key name
      */
    void unset(const QString &path,
               const QString &key,
               const QString &hkey = "HKEY_CURRENT_USER");
    void unsetPath(const QString &path,
                   const QString &hkey = "HKEY_CURRENT_USER");

    /*! \brief Executes regedit to import exported file into wine registry.
      *
      * \return true on success.
      */
    bool exec(QObject *parent,
              const QString &prefix_path,
              const QString &prefix_name = "Default",
              const QString &post_run = "");

    /*! \brief Read registry keys value from regfile by path.
      * \param path Registry path.
      * \param keys List of keys to be read from registry.
      *
      * \return List of read key's value. Index of read keys identical to requested keys.
      */
    QStringList readKeys(const QString &sysfile,
                         const QString &path,
                         const QStringList &keys) const;

    /*! \brief Read registry all keys (exclude of requested keys) value from regfile by path.
      * \param sysfile
      * \param path Registry path.
      * \param keys List of keys to be excluded read from registry.
      * \param count Number of keys to be read from registry.
      *
      * \return List of read key's value. Index of read keys identical to requested keys.
      */
    QStringList readExcludedKeys(const QString &sysfile,
                                 const QString &path,
                                 const QStringList &keys,
                                 const int count) const;

private:
    //! This is need for libq4wine-core.so import;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::unique_ptr<corelib> CoreLib;
    QLibrary libq4wine;

    QString regfile;
    QString regfile_image;
};

#endif
