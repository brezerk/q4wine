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

#ifndef IMAGE_H
#define IMAGE_H

#include "config.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

/*!
 * \class Image
 * \ingroup database
 * \brief This class provide database functions for Image table.
 *
 */
class Image
{
public:
    //! Class constructor
    Image();
    /*! \brief Getting images list.
     * \return Return QList on QStringList containing images.
     */
    QList<QStringList> getFields(void) const;

    /*! \brief Getting file path reccord by image name
     * \param  name   image name
     * \return Return QList on QStringList containing images.
     */
    QString getPath(const QString &name) const;\

    /*! \brief Check for image existence by name
     * \param  name   image name
     * \return If image exists -- returns true.
     */
    bool isExistsByName(const QString &name) const;

    /*! \brief Add image reccord
     * \param  name   image name
     * \param  path   image file path
     * \return If image exists -- returns true.
     */
    bool addImage(const QString &name, const QString &path) const;
    bool delImage(const QString &name) const;
    bool renameImage(const QString &name, const QString &old_name) const;
};

#endif // IMAGE_H
