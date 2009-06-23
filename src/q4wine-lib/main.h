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
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include <QIODevice>
#include <QChar>
#include <QTextStream>
#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <QRegExp>
#include <QTextCodec>

#include "process.h"

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

#include "config.h"

#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"
#include "src/core/database/image.h"

#include <iostream>
using namespace std;

/*!
 * \class corelib
 * \ingroup libq4wine
 * \brief This class provide general core functions for q4wine.
 *
 * It is not useful by itself, but helps to create user-side applications which
 * can use q4wine core functions;
 *
 */
class corelib {
public:
    //! Create an mainlib class
    corelib(bool _GUI_MODE);

    /*! \brief This function tries to get wine process list running in the current system.
     *
     * It gets process values: pid, name, nice priority and WINEPREFIX environment variable
     * \note On Linux it uses /proc file system, and on FreeBSD/MacOS it uses /dev/kmem and kvm.h functions.
     * \return Return an array of QList which contains an QStringList
     */
    QList<QStringList> getWineProcessList();

    /*! \brief This function get application settings.
     *
     * \param  group       Settings group.
     * \param  key         Settings key.
     * \param  defaultVal  Default returned value.
     * \param  checkExist  If true - then we need to check exists file or dir by value or not.
     * \return Return an a key value.
     */
    QVariant getSetting(const QString group, const QString key, const bool checkExist = true, const QVariant defaultVal = QVariant()) const;

	/*! \brief This function get cdrom devices from /etc/fstab file.
	 *
	 * \return Return an list of cdrom devices.
	 */

	/*! \brief This function get output from which shell built-in command.
	 *
	 * \return Return the full path of binary.
	 */
	QString getWhichOut (const QString fileName) const;

	/*! \brief This function get cdrom devices from /etc/fstab file.
	 *
	 * \return Return an list of cdrom devices.
	 */
	QStringList getCdromDevices(void) const;

	/*! \brief This function gets mouted image or media.
	 *
	 * \param  crom_mount	Mount point path.
	 * \return Return an list of cdrom devices.
	 */
	QString getMountedImages(const QString cdrom_mount) const;

	/*! \brief This function mount an image or drive to prefix mount point.
	 *
	 * \param  image_name	Image or drive name.
	 * \param  prefix_name	Prefix name.
	 * \return Return an list of cdrom devices.
	 */
	bool mountImage(QString image_name, const QString prefix_name) const;

	/*! \brief This function umount an image or drive from prefix mount point.
	 *
	 * \param  prefix_name	Prefix name.
	 * \return Return an list of cdrom devices.
	 */
	bool umountImage(const QString prefix_name) const;

	/*! \brief This function builds wine dlls list for prefix_lib_path.
	 *
	 * \param  prefix_lib_path	Path for wine libs.
	 * \return Return an list of cdrom devices.
	 */
	QStringList getWineDlls(QString prefix_lib_path) const;
    private:
    /*! Define is library operate in CLI or GUI mode.
     * \note This is typically need for error message display.
     */
    bool _GUI_MODE;

	/*! \brief Runs process.
	 *
	 * \param  exec	  Executable path.
	 * \param  args	  Arguments.
	 * \param  dir	  Exec directory.
	 * \return True on success.
	 */
	bool runProcess(const QString exec, const QStringList args, QString dir = "") const;


    /*! \brief Displays error message depending on _GUI_MODE variable value
     *
     * \param  message     Error message.
     * \param  info	   Define display type. If false - user interactive message.
     * \return When using an interactive display type, this functions returns a user selected value.
     */
	int showError(const QString message, const bool info) const;

	/*! \brief Displays error message depending on _GUI_MODE variable value
	 *
	 * \param  message     Error message.
	 */
	void showError(const QString message) const;

	Prefix *db_prefix;
	Image *db_image;



}; // end of class corelib

/*! \ingroup libq4wine
 * \brief This function returns a class of corelib type.
 *
 * Here we create an instance of corelib class and returns its pointer.
 * \return Return an a pointer to core lib class instance.
 * \note This function is exported from shared library.
 */
extern "C" corelib* createCoreLib(bool _GUI_MODE);

#endif
