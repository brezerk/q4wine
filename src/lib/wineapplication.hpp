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
#include <string>
#include <sstream>
#include <regex>

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
     * \param args The programm arguments.
     *
     * \param workDirectory The name of the directory where app store its
     * data. See also: #setWorkDirectory #setWorkDirectory
     *
     * \param iconName The name of the pixmap icon to be used to display
     * application icon.
     * See also: #setIconName #getIconName
     *
     * \param description An user defined application description.
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
     * \param lang The lanvuage to use. Usefill if app requre language differ
     * then the system one.
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
     * \param prefixId The associated row id for WinePrefix object.
     * If it set to 0, then the object is not associated with any
     * database record.
     * See also: setPrefixId getPrefixId
     *
     * \param dirId The associated row id for  WineApplication object.
     * If it set to 0, then the object is not associated with any
     * database record.
     * See also: setDirId getDirId
     *
     * \param id The associated row id. If it set to 0, then the object
     * is not associated with any database record.
     * See also: DBObject#setId DBObject#getId
    */
    WineApplication(std::string name,
            std::string path,
            std::string args,
            std::string workDirectory,
            std::string iconName = std::string(),
            std::string description = std::string(),
            std::string overrideDlls = std::string(),
            std::string wineDebug = std::string(),
            std::string virtualDesktop = std::string(),
            std::string lang = std::string(),
            bool useTerminal = false,
            std::string display = std::string(),
            intptr_t priority = 0,
            std::string preRunScript = std::string(),
            std::string postRunScript = std::string(),
            uintptr_t prefixId = 0,
            uintptr_t dirId = 0,
            uintptr_t id = 0);
    /*! Destroys this WineApplication object. */
    ~WineApplication();

    virtual bool save(void);

    /*!
     * \brief getNiceCmd Construct nice command
     * \return Nice command if priority_ !=0. Empty string otherwice.
     */
    const std::string getNiceCmd(void) const;

    /*! Constructs wine env variables (LANG, DISPLAY, WINEDLLOVERRIDES
     * WINEDEBUG) using WineApplication data.
     *
     * Example:
     * LANG='uk_UA.UTF8'
     * DISPLAY=':0'
     * WINEDLLOVERRIDES='comdlg32,shell32=n;c:\\foo\\bar\\baz=b'
     * WINEDEBUG='fixme-all,warn+cursor,+relay'
     *
     * \return Formatted env variables
     */
    const std::string getEnvVariables() const;

    void setName(std::string name);
    void setPath(std::string path);
    void setArgs(std::string args);
    void setWorkDirectory(std::string workDirectory);
    void setIconName(std::string iconName);
    void setDescription(std::string description);
    void setOverrideDlls(std::string overrideDlls);
    void setWineDebug(std::string wineDebug);
    void setVirtualDesktop(std::string virtualDesktop);
    void setUseTerminal(bool useTerminal);
    void setDisplay(std::string display);
    void setPriority(intptr_t priority);
    void setPreRunScript(std::string preRunScript);
    void setPostRunScript(std::string postRunScript);
    void setPrefixId(uintptr_t prefixId);
    void setDirId(uintptr_t dirId);

    const std::string getName(void) const;
    const std::string getPath(void) const;
    const std::string getArgs(void) const;
    const std::string getWorkDirectory(void) const;
    const std::string getIconName(void) const;
    const std::string getDescription(void) const;
    const std::string getOverrideDlls(void) const;
    const std::string getWineDebug(void) const;
    const std::string getVirtualDesktop(void) const;
    bool getUseTerminal(void) const;
    const std::string getDisplay(void) const;
    intptr_t getPriority(void) const;
    const std::string getPreRunScript(void) const;
    const std::string getPostRunScript(void) const;
    uintptr_t getPrefixId(void) const;
    uintptr_t getDirId(void) const;

 private:
    /*! Set the table name in q4wine database to lookup object data */
    static const std::string tableName_;
    std::string name_;
    std::string path_;
    std::string args_;
    std::string workDirectory_;
    std::string iconName_;
    std::string description_;
    std::string overrideDlls_;
    std::string wineDebug_;
    std::string virtualDesktop_;
    std::string lang_;
    bool useTerminal_;
    std::string display_;
    intptr_t priority_;
    std::string preRunScript_;
    std::string postRunScript_;
    uintptr_t prefixId_;
    uintptr_t dirId_;
};

}  // namespace lib
}  // namespace q4wine
