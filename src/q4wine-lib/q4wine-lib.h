/***************************************************************************
 *   Copyright (C) 2008-2020 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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
 * \defgroup libq4wine libq4wine-core
 * \brief libq4wine-core package provides general functions for q4wine-gui and q4wine-cli packages.
 */

#ifndef Q4WINELIB_H
#define Q4WINELIB_H

#include "config.h"
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
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QTranslator>
#include <QStandardPaths>
#include <locale.h>

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

// Experimental Darwin support (no test were performed)
#ifdef _OS_DARWIN_

#include <sys/queue.h>

#include <mach/mach.h>
#include <mach/mach_error.h>
#include <mach/policy.h>
#include <mach/task_info.h>
#include <mach/thread_info.h>
#include <sys/sysctl.h>
#include <sys/resourcevar.h>

#include "src/darwin/ps.h"

#endif


#include "prefix.h"
#include "dir.h"
#include "icon.h"
#include "image.h"
#include "versions.h"

/*!
 * \class corelib
 * \ingroup libq4wine
 * \brief This class provides general core functions for Q4Wine.
 *
 * It is not useful by itself, but helps to create user-side applications which
 * can use Q4Wine core functions;
 *
 */

//! Exec obj struct
struct ExecObject{
    QString execcmd;
    QString prefixid;
    QString runcmd;
    QString useconsole;
    QString cmdargs;
    QString override;
    QString winedebug;
    QString display;
    QString wrkdir;
    QString desktop;
    QString nice;
    QString name;
    QString lang;
    QString prerun;
    QString postrun;
};

class corelib {
public:
    //! Create an mainlib class
    explicit corelib(const bool GUI_MODE);

    /*! \brief This function tries to get wine process list running in the current system.
     *
     * It gets process values: pid, name, nice priority and WINEPREFIX environment variable
     * \note On Linux it uses /proc file system, and on FreeBSD/MacOS it uses /dev/kmem and kvm.h functions.
     * \return Return an array of QList which contains a QStringList
     */
    QList<QStringList> getWineProcessList(const QString &prefix_name = "");

    /*! \brief This function get application settings.
     *
     * \param  group       Settings group.
     * \param  key         Settings key.
     * \param  defaultVal  Default returned value.
     * \param  checkExist  If true - then we need to check exists file or dir by value or not.
     * \return Return an a key value.
     */
    QVariant getSetting(const QString &group,
                        const QString &key,
                        const bool checkExist = true,
                        const QVariant &defaultVal = QVariant()) const;

    void checkSettings(void);

    QIcon loadIcon(const QString &iconName, bool fromTheme = true);
    QIcon loadAppIcon(const QString &iconName);
    QPixmap loadPixmap(const QString &pixmapName);

    QString getTranslationLang();
    QString getLang(const bool fromSettings = true);
    QString getLocale();

    bool isConfigured();
    bool checkDirs(const QString &rootPath,
                   QStringList subDirs = QStringList()) const;

    void getBuildFlags();

    /*! \brief This function get cdrom devices from /etc/fstab file.
     *
     * \return Return an list of cdrom devices.
     */

    /*! \brief This function get output from which shell built-in command.
     *
     * \return Return the full path of binary.
     */
    QString getWhichOut (const QString &fileName, bool showErr = true);

    /*! \brief This function get cdrom devices from /etc/fstab file.
     *
     * \return Return an list of cdrom devices.
     */
    QStringList getCdromDevices(void) const;

    /*! \brief This function get cdrom wine device name
     *         by mount point path.
     *
     * \param prefix_path Prefix path :)
     * \param mount_point Mount point path
     *
     * \return Return an drive letter on success or null on fail
     */
    QChar getCdromWineDrive(QString prefix_path,
                            const QString &mount_point);

    /*! \brief This function opens help url in xdg-open browser.
     *
     * \return Return nothing.
     */
    void openHelpUrl(const QString &rawurl);

    /*! \brief This function opens url in xdg-open browser.
     *
     * \return Return nothing.
     */
    void openHomeUrl(const QString &rawurl) const;
    void openUrl(const QString &rawurl) const;
    void openConsole(const QString &path, const QString &prefix_name);
    void updateRecentImagesList(const QString &media) const;


    /*! \brief This function gets mouted image or media.
     *
     * \param  crom_mount	Mount point path.
     * \return Return an list of cdrom devices.
     */
    QString getMountedImages(QString cdrom_mount) const;

    /*! \brief This function mount an image or drive to prefix mount point.
     *
     * \param  image_name	Image or drive name.
     * \param  prefix_name	Prefix name.
     * \return Return an list of cdrom devices.
     */
    bool mountImage(const QString &image_name, const QString &prefix_name);

    /*! \brief This function umount an image or drive from prefix mount point.
     *
     * \param  prefix_name	Prefix name.
     * \return @c true on success or @c false on error.
     */
    bool umountImage(const QString &prefix_name);

    /*! \brief This function kills wineserver.
     *
     * \param  prefix_path	Prefix name.
     * \return Return an list of cdrom devices.
     */
    bool killWineServer(const QString &prefix_path, const QString &pid);

    /*! \brief Prepare icon data for run wine.
     *
     * \param  prefix_name	Prefix name.
     * \param  dir_name		Dir name.
     * \param  icon_name	Icon name.
     * \return Return true on success.
     */
    bool runIcon(const QString &prefix_name,
                 const QString &dir_name,
                 const QString &icon_name,
                 const QStringList &extra_args = QStringList());

    /*! \brief Prepare icon data for run wine.
     *
     * \param  ExecObject	An icon data struct.
     * \param  prefix_name	Prefix name.
     * \return Return true on success.
     */
    bool runWineBinary(const ExecObject &execObj,
                       const QString &prefix_name,
                       const bool detach=true);
    //bool runWineBinary(const QString winebinary, const QString cmdargs, const QString prefix_name, const QString wineAppendBin="", const bool releaseProc = true);
    bool checkFileExists(const QString &path);


    QString createDesktopFile(const QString &prefix_name,
                              const QString &dir_name,
                              const QString &icon_name,
                              const bool is_menu = false) const;
    bool deleteDesktopFile(const QString &prefix_name,
                           const QString &dir_name,
                           const QString &icon_name) const;

    QString getWinePath(const QString &path, const QString &option);

    /*! \brief This function builds wine dlls list for prefix_lib_path.
     *
     * \param  prefix_lib_path	Path for wine libs.
     * \return Return an list of cdrom devices.
     */
    QStringList getWineDlls(QString prefix_lib_path) const;

    /*! \brief Runs process.
     *
     * \param  exec	  Executable path.
     * \param  args	  Arguments.
     * \param  dir	  Exec directory.
     * \return True on success.
     */
    bool runProcess(const QString &exec,
                    const QStringList &args,
                    QString dir = "",
                    const bool showLog = true);
    bool runProcess(const QStringList &args,
                    const QString &caption,
                    const QString &message);

    /*! \brief Gets string with escaped special characters
     *
     * \param string	Base string
     */
    QString getEscapeString(const QString &string,
                            const bool spaces = true) const;

    QString getShellEscapeString(QString string);
    QString getStrictEscapeString(QString string);


    /*! \brief Gets mount string based on QuikMount type profile
     *
     * \param profile	Profile type ID
     */
    QString getMountString(const int profile);

    /*! \brief Gets mount image string based on QuikMount type profile
     *
     * \param profile	Profile type ID
     */
    QString getMountImageString(const int profile);

    /*! \brief Gets umount string based on QuikMount type profile
     *
     * \param profile	Profile type ID
     */
    QString getUmountString(const int profile);
    bool reniceProcess(const int pid, const int priority);
    void runAutostart(void);

    void createPrefixDBStructure(const QString &prefixName);

    QString decodeRegString(const QString &string);

    /*! \brief This function is used to export prefix info.
     *
     * This function will export Q4Wine prefixes information according with http://wiki.winehq.org/BottleSpec
     *
     * \return true on success.
     */
    bool exportPrefixesConfiguration(void);

    /*! \brief This function is used to import prefix info.
     *
     * This function will import wine's prefixes information according with http://wiki.winehq.org/BottleSpec
     *
     * \return true on success.
     */
    QStringList importPrefixesConfiguration(void);

    /*! \brief This function is used to remove directory.
     *
     * This function is used to remove directory
     *
     * \return true on success.
     */
    bool removeDirectory(const QString &dirPath);

    static const QString formatLocation(const QStringList &path);
    static const QString getAppConfigLocation(const QStringList &path = QStringList());
    static const QString getGenericConfigLocation(const QStringList &path = QStringList());
    static const QString getAppDataLocation(const QStringList &path = QStringList());
    static const QString getGenericDataLocation(const QStringList &path = QStringList());
    static const QString getAppCacheLocation(const QStringList &path = QStringList());
    static const QString getGenericCacheLocation(const QStringList &path = QStringList());

private:
    /*! Define is library operate in CLI or GUI mode.
     * \note This is typically need for error message display.
     */
    bool m_GUI_MODE;

    QString mdconfig, fuseiso, fusermount, ln, rm, sh;

    /*! \brief Displays error message depending on _GUI_MODE variable value
     *
     * \param  message     Error message.
     * \param  info	   Define display type. If false - user interactive message.
     * \return When using an interactive display type, this functions returns a user selected value.
     */
    int showError(const QString &message, const bool info) const;

    /*! \brief Displays error message depending on _GUI_MODE variable value
     *
     * \param  message     Error message.
     */
    void showError(const QString &message) const;

    Prefix db_prefix;
    Image db_image;
    Icon db_icon;
    Dir db_dir;

}; // end of class corelib

/*! \ingroup libq4wine
 * \brief This function returns a class of corelib type.
 *
 * Here we create an instance of corelib class and returns its pointer.
 * \return Return an a pointer to core lib class instance.
 * \note This function is exported from shared library.
 */
extern "C" corelib* createCoreLib(const bool GUI_MODE);

#endif
