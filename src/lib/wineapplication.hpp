/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#pragma once

#include <stdint.h>

#include <QString>
#include <QStringList>

#include "src/lib/defines.hpp"
#include "src/lib/dbobject.hpp"

namespace q4wine {
namespace lib {

/*! \class WineApplication wineapplication.hpp
 * <q4wine/src/lib/wineapplication.hpp>
 * \brief Describes a wine application configuration.
 *
 * \par Wine Terms
 *
 * In terms of Wine, application is a DOS, Windows 3.x, Win32 or Win64
 * executable (on 64-bit systems).
 *
 * The program name may be specified in DOS format (C:\\WINDOWS\\SOL.EXE)
 * or in Unix format (/msdos/windows/sol.exe). You may pass arguments to
 * the program being executed by adding them to the end of the command line
 * invoking wine (such as: wine notepad C:\\TEMP\\README.TXT). Note that you
 * need to '\' escape special characters (and spaces) when invoking Wine via
 * a shell, e.g: wine C:\\Program\ Files\\MyPrg\\test.exe
 *
 * It can also be one of the Windows executables shipped with Wine, in which
 * case specifying the full path is not mandatory,  e.g.  wine explorer or
 * wine notepad.
 *
 * \par Q4Wine Terms
 *
 * In terms of Q4Wine WineApplication is a set of options which describe
 * addtional system and wine specific parameters to be used to execute
 * the application: such as nice prority, desktop size e.t.c.
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class WineApplication : public DBObject {
 public:
    /*! Constructs an empty WineApplication object. */
    WineApplication();
    /*! Constructs an WineApplication object.
     *
     * \param name Is user defined name, should be unique.
     * See also: #setName #getName
     *
     * \param path The program name may be specified in DOS or Unix formats.
     * See also: #setPath #getPath
     *
     * \param workDirectory The name of the directory where app store its
     * data. See also: #setWorkDirectory #setWorkDirectory
     *
     * \param icon The name of the pixmap icon to be used to display
     * application icon
     * See also: #setIconName #getIconName #getIcon
     *
     * \param description An user defined application description
     * See also: #setDescription #getDescription
     *
     * \param overrideDlls Defines the override type and load order of dlls
     * used in the loading process for any dll. There are currently two types
     * of libraries that can be loaded into a process address space: native
     * windows dlls (native) and Wine internal dlls (builtin). The type may be
     * abbreviated with the first letter of the type (n or b). The library may
     * also be disabled (''). Each sequence of orders must be separated by
     * commas.
     *
     * Each dll may have its own specific load order. The load order
     * determines which version of the dll is attempted to be loaded
     * into the address space. If the first fails, then the next is tried and
     * so on. Multiple libraries with the same load order can be  separated
     * with commas. It is also possible to use specify different loadorders
     * for different libraries by separating the entries by ";"
     *
     * This optin will be converted
     * into env variable WINEDLLOVERRIDES during application execution.
     * See also: #setOverrideDlls #getOverrideDlls
     *
     * \param wineDebug Turns debugging messages on or off. The syntax of the
     * variable is of the form [class][+|-]channel[,[class2][+|-]channel2]
     *
     * This optin will be converted
     * into env variable WINEDEBUG during application execution.
     * See alse: #setWineDebug #getWineDebug
     *
     * \param virtualDesktop The virtual desktop size. If set, wine will
     * emulate a virtual desktop. In this case, all programs will run
     * in a separate window.
     * See also: #setVirtualDesktop #getVirtualDesktop
     *
     * \param useTerminal If set, then application will be executed using
     * user-defined terminal application instead of q4wine-helper.
     * See also: #setUseTerminal #getUseTerminal
     *
     * \param display Specifies the X11 display to use.
     * See also: #setDisplay #getDisplay
     *
     * \param priority Run a program with modified scheduling priority.
     * Niceness values range from -20 (most favorable to the process) to
     * 19 (least favorable to the process).
     * See also: #setPriority #getPriority
     *
     * \param preRunScript
     * See also: #setPreRunScript #getPreRunScript
     *
     * \param postRunScript
     * See also: #setPostRunScript #getPostRunScript
     *
     * \param prefix_id The associated row id for WinePrefix object.
     * If it set to 0, then the object is not associated with any
     * database record.
     * See also: setPrefixId getPrefixId
     *
     * \param dir_id The associated row id for  WineApplication object.
     * If it set to 0, then the object is not associated with any
     * database record.
     * See also: setDirId getDirId
     *
     * \param id The associated row id. If it set to 0, then the object
     * is not associated with any database record.
     * See also: DBObject#setId DBObject#getId
    */
    WineApplication(QString name,
            QString path,
            QString args,
            QString workDirectory,
            QString iconName = QString::null,
            QString description = QString::null,
            QString overrideDlls = QString::null,
            QString wineDebug = QString::null,
            QString virtualDesktop = QString::null,
            QString lang = QString::null,
            bool useTerminal = false,
            QString display = QString::null,
            int32_t priority = 0,
            QString preRunScript = QString::null,
            QString postRunScript = QString::null,
            uint32_t prefixId = 0,
            uint32_t dirId = 0,
            uint32_t id = 0);
    /*! Destroys this WineApplication object. */
    ~WineApplication();

    void setName(QString name);
    void setPath(QString path);
    void setArgs(QString args);
    void setWorkDirectory(QString workDirectory);
    void setIconName(QString iconName);
    void setDescription(QString description);
    void setOverrideDlls(QString overrideDlls);
    void setWineDebug(QString wineDebug);
    void setVirtualDesktop(QString virtualDesktop);
    void setUseTerminal(bool useTerminal);
    void setDisplay(QString display);
    void setPriority(int32_t priority);
    void setPreRunScript(QString preRunScript);
    void setPostRunScript(QString postRunScript);
    void setPrefixId(uint32_t prefixId);
    void setDirId(uint32_t dirId);

    const QString getName(void) const;
    const QString getPath(void) const;
    const QString getArgs(void) const;
    const QString getWorkDirectory(void) const;
    const QString getIconName(void) const;
    const QString getDescription(void) const;
    const QString getOverrideDlls(void) const;
    const QString getWineDebug(void) const;
    const QString getVirtualDesktop(void) const;
    bool getUseTerminal(void) const;
    const QString getDisplay(void) const;
    int32_t getPriority(void) const;
    const QString getPreRunScript(void) const;
    const QString getPostRunScript(void) const;
    uint32_t getPrefixId(void) const;
    uint32_t getDirId(void) const;

 private:
    /*! Set the table name in q4wine database to lookup object data */
    static const QString tableName_;
    QString name_;
    QString path_;
    QString args_;
    QString workDirectory_;
    QString iconName_;
    QString description_;
    QString overrideDlls_;
    QString wineDebug_;
    QString virtualDesktop_;
    QString lang_;
    bool useTerminal_;
    QString display_;
    int32_t priority_;
    QString preRunScript_;
    QString postRunScript_;
    uint32_t prefixId_;
    uint32_t dirId_;
};

}  // namespace lib
}  // namespace q4wine
