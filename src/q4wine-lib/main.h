/***************************************************************************
 *   Copyright (C) 2008, 2009 by Malakhov Alexey                           *
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

/*!
 * \defgroup libq4wine libq4wine-core
 * \brief libq4wine-core pakage provide general functions for q4wine-gui and q4wine-cli pakages.
 */

#ifndef Q4WINELIB_H
#define Q4WINELIB_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include <QIODevice>
#include <QChar>
#include <QTextStream>

// FreeBSD support
#ifdef _OS_FREEBSD_
        #include <kvm.h>
        #include <sys/param.h>
        #include <sys/user.h>
        #include <sys/file.h>
        #include <sys/sysctl.h>
        #include <limits.h>
        #include <paths.h>
#endif

// Experimental Darwin support (no test were preformed)
#ifdef _OS_DARWIN_
        #include <kvm.h>
        #include <sys/param.h>
        #include <sys/user.h>
        #include <sys/file.h>
        #include <sys/sysctl.h>
        #include <limits.h>
        #include <paths.h>
#endif

/*!
 * \class corelib
 * \ingroup libq4wine
 * \brief This class provide general core functions for q4wine.
 *
 * It is not useful by itself, but helps to create user-side applications which
 * can use q4wine core functions;
 *
 * \note This is still in development state ;)
 */
class corelib {
public:
    //! Create an mainlib class
    corelib();

    /*! \brief This function tries to get wine process list running in the current system.
      *
      * It gets process values: pid, name, nice priority and WINEPREFIX environment variable
      * \note On Linux it uses /proc file system, and on FreeBSD/MacOS it uses /dev/kmem and kvm.h functions.
      * \return Return an array of QList which contains an QStringList
      */
    QList<QStringList> getWineProcessList();
    QString test();

}; // end of class corelib

/*! \ingroup libq4wine
 * \brief This function returns a class of corelib type.
 *
 * Here we create an instance of corelib class and returns its pointer.
 * \return Return an a pointer to core lib class instance.
 * \note This function is exported from shared library.
 */
extern "C" corelib* createCoreLib();

#endif
