# Qt GUI for Wine

Q4Wine is a Qt GUI for Wine that helps manage Wine prefixes and installed applications.

[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner-direct.svg)](https://stand-with-ukraine.pp.ua)

# Changelog

## Version 1.4.2

Fixed:

- Qt6 deprecation warnings;
- No such signal QComboBox::currentIndexChanged(const QString);
- Ignoring return value of function declared with 'nodiscard' attribute [-Wunused-result];
- No such signal SingleApplication::messageReceived(const QString);
- Wine variables are not parsed properly for wine prefix kill action;
- Invalid qm file location for cmake 4.x and qt6.9.x (I hope so);

## Version 1.4.1

Updated:

- Updated Dutch translation (#203);

Fixed:

- qm files are created in `CMAKE_BINARY_DIR` (#204);

## Version 1.4.0

Initial Qt6 release

Updated:

- CMake minimum version requirement set to 3.24;
- SingleApplication is used instead of obsolete QtSingleApplication;
- Wine AppDB browser is marked as obsolete and disabled by default;
- Cleanup legacy code and files;
- PrefixSetupWizzard: Added missing win8, win8.1, win10, win11 options versions [PR #197];
- Update `zh_CN` translation [PR #197];

## Version 1.3.13

Fixed:

- Prefix Import feature is not working as expected [BUG-156];
- Misspelled Color Theme Settings [BUG-155];
- CMake warning [BUG-158];
- XDG Base Directory Specification support [BUG-178];
- Use `LC_ALL` instead of LANG to force override applciation locale [BUG-165];

## Version 1.3.12

Updated:

- Afrikaans translation;
- Japanese translation;
- Ukrainian translation;
- Traditional Chinese translation;
- German translation;

Added:

- Add an option to show / hide "Args" parameter [BUG-114];
- Generate StartupWMClass values in created .desktop files [BUG-153];
- Added a feature to keep the terminal open when running winetricks so users can see the output.

Fixed:

- Fix parsing of env causing issues with wine kill [BUG-143];
- Error: wrong or broken XML data. Try again later. [BUG-146];
- Fixed a bug where /etc/mtab was not being read and resulted in being unable to unmount iso images.
- Fixed a bug where the temporary director for generating icons wasn't being deleted. Refactored this to use QTemporaryDir.
- Added workaround for st terminal, similar to konsole.
- Fixed warnings: Qt deprecated function useage;
- Fixed warnings: use of old-style cast

## Version 1.3.11

Fixed:

- Display of Valve Proton in Processes view [BUG-138];
- Improve all dialogs of choosing files [BUG-139];
- Allow Stop Wine action to kill Valve Proton in Processes view [BUG-138];

Updated:

- Code cleanup: drop Qt4 support;

## Version 1.3.10
Fixed:

- cppcheck: Uninitialized struct member: `item.provider_id`;

Add:

- cleanup.sh: to get rid of all non-release stuff;

## Version 1.3.9

Fixed:

- Fixed build with Qt >= 5.11 [BUG-125];
- Error: wrong or broken XML data. Try again later. [BUG-132]

Added:

- Custom virtual desktop resolution size [BUG-131];

## Version 1.3.8
Updated:

- `gui_sudo` missing in Ubuntu 18.04 [BUG-120];

Fixed:

- winetricks: Proxy settings are used even if proxy type set to 'none';

## Version 1.3.7

Updated:

- Wine stock icons to be organised through Q4Wine [BUG-117];
- Add gnome-terminal into terminal detection logic [BUG-118];

Added:

- French translation (thx 2 seigneurfuo);

## Version 1.3.6

Updated:

- Added indication of applications which was installed through winetricks [BUG-106][BUG-111];
- Default Wine Desktop Integartion path settings for new prefixes [BUG-106];
- Added option to run custom scripts with fake drive settings after creation of fake drive [BUG-106];
- Qt5 build enabled by default;
- Taiwan translation;
- Ukrainian translation;
- Spanish translation;
- Czech translation;
- Bind Plus and Minus keys for Program tab for +\- zoom respectively [BUG-112];
- Bind Plus and Minus keys for Config tab for +\- zoom respectively [BUG-112];
- Allow to change Config and Program Icon sizes independently  [BUG-112];

Documented:

- q4wine prevents user from running winecfg from KRunner even after uninstalling [BUG-98];
Fixed:
- Static code analysis: errors and warnings [BUG-116];
- Error: wrong or broken XML data. Try again later. [BUG-114];

## Version 1.3.5

Updated:

- Italian translation;
- Ukrainian translation;

Fixed:

- Removed extra Doxygen files to reduce artifact size;

## Version 1.3.4

Added:

- Add WINE env var to console environment [BUG-91];

Fixed:

- Icons are not loaded properly with Qt 5.7.x;
- Console launch environment uses dead setting fields [BUG-90];
- Q4Wine file selectors filters files with upper case extension [BUG-94][QTBUG-51712];
- Refactor winetrkics parser: use direct parser + transactions;
- Links imported from Windows desktop do not work [BUG-103];
- Import from Windows desktop is not working in some cases [BUG-103];
- Processes tab does not detect some wine processes correctly [BUG-100];

## Version 1.3.3

Fixed:

- Problem opening file dialog to mount ISO [BUG-86];

## Version 1.3.2

Fixed:

- [regression, bisected] Icon information is not displayed correctly [BUG-80];
- Winetricks does not update [BUG-77];
- First startup wizard (quick mount profile): profile strings are initially empty if fuseiso is not installed [BUG-81];
- Download (latest) Winetricks from GitHub [BUG-82];
- Correct proxy-related environment variables in Winetricks plugin [BUG-83];
- Use env command in the command line to download Winetricks [BUG-83];
- Support proxy username/password in Winetricks plugin [BUG-83];
- Treat aarch64 as 64 bit platform and put libraries in lib64 directory [BUG-85];

## Version 1.3.1

Added:

- Allow to execute and create icons for *.bat and *.msi files [BUG-69];
- Added Chinese (China) translation (thx 2 Mingye Wang and Mingcong Bai);
- Added Japanese translation (thx 2 Masanori Kakura);

Fixed:

- Renamed 'New' menu option for icon and prefix view to 'New icon' and 'New
folder' respectively to avoid confusion [BUG-69];
- Icon description is not exported into *.desktop file [BUG-69];
- Winetrick's item description label doesn't wrap properly [BUG-71];
- Qt5: detailed view item selection duplicate [BUG-75];
- q4wine-cli does not accept extra app arguments [BUG-78];
- Translation files moved from i18n to l10n;
- Hide deprecated prefix options if values are not set;

## Version 1.3

Added:

- Taiwan translation (thx 2 Jeff Huang);
- Use icons from the system icon theme where possible [BUG-22];
- Replace default raster icons with SVG analogs [BUG-21];
- Extend App icons zoom sizes up to 512px [BUG-21];
- Allow to choose `*.svg` and `*.svgz` images for icons [BUG-21];
- Add an app option to switch `default/ambiance-light/ambiance-dark` icon [BUG-21];
- Use ISO country codes for translation files (required for: pootle, gentoo ebuild);
- Use native language name for language selection;

Fixed:

- WineLibs paths are no longer strictly required [BUG-68];
- Process explorer is broken [BUG-70];

## Version 1.2-r2

Added:

- Czech translation update (thx 2 Pavel Fric);

Fixed:

- `QtSystemApplication` library detection does not work well for Qt5 [BUG-64];

## Version 1.2-r1

Added:

- Deleting prefix does not remove prefix folder in `~/.local/share/wineprefixes` [BUG-43];

Fixed:

- Fixed typo in Startup Wizard;
- Fixup Wine Libs detection [BUG-50];
- Remove Wine settings from App settings [BUG-50,BUG-23];
- Make Wine64 libs optional (for x86 only platforms) [BUG-50];
- Backup/Restore prefix path handler;
- Can't use pre/post run scripts without standalone script [BUG-49];
- Do not force startpage open;
- add custom dll overrides outside from the list [BUG-60];
- Revert "Kill dead online components" breaks the Qt5 build [BUG-62];
- Do not build wineappdb core components if `-DWITH_WINEAPPDB=OFF` is set [BUG-62];
- 'Run -> Browser' on Programs window not work [BUG-54];
- QT5: Winetricks application list fails to refresh and causes segfault on quit [BUG-53];
- Make -DQT5 flag visible during cmake build and runtime;
- CLI: `-v/-h` options must not trigger nor database check nor wizard startup;
- Remove useless and confusing appdb startup dialog;

## Version 1.1-r2

Fixed:

- Error while loading application settings by key: 'WineLibs' [BUG-42];

## Version 1.1-r1

Fixed:

- Error while loading application settings by key: 'WineLibs' [BUG-42];
- Updated Afrikaans translation. thx 2 Danie Wessels;
- Fixed cmake warnings;
- Updated .desktop file;

## Version 1.1

Added:

- extended list of a virtual desktop resolutions [BUG-35];
- configure multiple wine paths globally to allow easily selection in prefixes [BUG-23];
- create predefined execution template strings for optirun, separate X display, e.t.c. [BUG-37];

Fixed:

- Gentoo: QtSingleApplication package not found with `-DWITH_SYSTEM_SINGLEAPP=ON` [BUG-39];
- FreeBSD\clang error: use of undeclared identifier `LC_*` [BUG-41];

## Version 1.0-r3

Fixed:

- "Use single-click to run icons" does not affect icons in "setup" tab [BUG-27];
- q4wine.desktop value "" for key "Path" in group "Desktop Entry" [BUG-29];
- Pre and Post run Icon scripts unusable, if the path contains ")" [BUG-33];

Added:

- Keyboard navigation for items in "setup" and "icon" tabs;
- Afrikaans translation. Thx 2 Danie Wessels;

## Version 1.0-r2

Fixed:

- Clean setup: error while loading application settings by key: prefixDefaultPath [BUG-25];
- Winetricks plugin does not check script presence [BUG-26];
- "Use single-click to run icons" does not affect icons in "setup" tab [BUG-27];

Added:

- Persian translation. Thx 2 Arash Ariayee;
- Winetricks plugin can use script installed in system [BUG-28];

## Version 1.0-r1

Fixed:

- Prefix tree "Delete" menu item do not remove menu tree.
- Support for utf8 system user name [BUG-20];
- Winetricks doesn't work properly if WINEPATH contain spaces [BUG-19];

## Version 1.0

Fixed:

- Quote run string in case of console usage (gnome-terminal, see BUG-15)
- Winetricks functionality fails to work [BUG-15]
- Cannot run 'Create Fake Drive' for a new prefix with a win32 architecture [BUG-14]
- System Language autodetection does not work [BUG-13]
- Error while loading application settings by key: 'WineLibs'. File or path does not exist: "" [BUG-12]
- q4wine doesn't clean up user's tmp directory [BUG-11]

Removed:

- File -> Install wizard menu entry;

Added:

- Option to set default directory for new wine prefixes (default: $HOME/.local/share/wineprefixes)
- q4wine desktop menu tree [BUG-16]

## Version 0.999-rc8

Added:

- Prefix execution template string;

Fixed:

- regression: selection focus jumps to prefix name on subdirectory selection;
- regression: can't load user selected language

## Version 0.999-rc7

Added:

- Winetricks database cache;
- Merge Wintricks core;
- New System Config interface;
- Mac OS X support;
- Pre and Post run Icon scripts [3465598][3388054][2716333];
- Icon and Prefix sorting (name, date);
- Version string to About page;
- Handler for wrong URL in wine app db;
- Add Logging subsystem activation\deactivation button to Logging tab toolbar;
- Add taskmgr, winemine, iexplorer, control, oleview icons to prefix's system folder;
- New system icons from latest wine version;
- Install program icons in $prefix/share/icons/hicolor/; Add 16,22,32,48,64,96,128,256 icon sizes [3505384];
- Prefix to translation files names [3506372];
- Prefix execution template string;

Fixed:

- Enhanced win64 support for wine;
- Better Wine library path autodetection [Linux];
- Create ~/.local/share/wineprefixes directory if it does not exist;
- Error is Desktop paths are empty;
- Desktop Paths are wrongly labeled [3491775];
- Win32 prefix creation while prefix directory exists;
- Unite Icon\Detail toolbar actions;
- No translations autoupdate at project build. Use `make translations` instead;
- Fixed process refresh loop;
- Extended env command after console terminal;
- Updated README file: added "Known Issues" section;
- Set WINEDEBUG to '-all' if logging subsystem disabled and program was
executed not in console session;
- Icon export dialog empty icons [3358599];
- Dropped obsolete strings from translation files [3358602];
- Fixed build in Debian GNU/Hurd (should also works in Arch Hurd);
- Possibly fixed build in OpenBSD and NetBSD;

## Version 0.121

Added:

- updated winetricks plugin;
- option to make Prefix view unexpanded at startup;
- Polish translation by Frank (nobange);
- interoperability of wineprefixes between q4wine, wibom, vineyard, and
winetricks (and maybe playonlinux) http://wiki.winehq.org/BottleSpec;
- German translation by Ron;
- experimental build support for Mac OS X;
- extended virtual desktop resolution list [3150062];
- request for language locale support [3144001 and 3149056];
- Hebrew translation by Yaron Shahrabani;
- Prefix alphabetic sort;

Fixed:

- build on FreeBSD;
- removed embedded fuseiso support (q4wine-mount);

## Version 0.120-r1

Added:

- q4wine now will create new symlinks when mounting $WINEPREFIX/dosdevices/${DRIVE}:
pointing to the mount point and $WINEPREFIX/dosdevices/${DRIVE}:: pointing to
the Image file;
Apparently this is needed so that the images are recognized as a proper
cd/dvd drive; by aleric
- Added cmake option `WITH_SYSTEM_SINGLEAPP` to compile q4wine with system
installed QtSingleAppliaction library;
- Added cmake option `RELEASE` for translation and developing purposes;
- Added cmake option `LINGUAS` which specify languages to build;
- "make translations" target; Use it for translation updates;
- Added cmake option `LIBS_ENTRY_PATH` to allow override for libs install directory;
- Implementation for feature request 3079649 (Prefixes list sorting);

Fixed:

- mount images in read only mode (ro); by aleric
- removed media drive option in prefix settings dialog;
- Fixed quote bugs for `WINEDEBUG` and `DISPLAY` variables, reported by Carlos Ignacio Ruax;
- English fixes by Steven Panek;
- Fake drive can create unlinked desktop folders;
- Move File System item upper in Fake Drive Settings dialog;

## Version 0.120

Added:

- Czech translation (thx to Pavel Fric);
- Added cmake option `MANPAGE_ENTRY_PATH`;
- User now can open system terminal from q4wine programs tab menu for selected dir;
- Winetricks plugin updates;
- Added minimize to tray at startup option;
- Added minimize to tray at application icon startup option;
- DBUS notification;
- Added Wine WOW64Node support and configuration settings;

Fixed:

- warning: ignoring return value of `int chdir(const char*)`, declared with attribute `warn_unused_result`;
- Fixed unescaped characters in q4wine-helper (q4wine-Bugs-3059156);
- Fixed run dialog bug for gut trunk (q4wine-Bugs-3075628);
- Fixed bug in q4wine-helper while run games like: Fallout 1\2, Truck Simulator e.t.c.
- Fixed some typo and GUI layouts;
- Fixed fakedrivesettings.cpp rewrite sh scripts in to C++\Qt code;
- FreeBSD: report error while can't read process info from /proc fs;
- Fixed $HOME/.config/q4wine/tmp not required any more, we use $WINEPREFIX/tmp instead;
- Desktop file updates;

## Version 0.119-r1

Added:

- Settings dialog online documentation;

Fixed:

- Some of English spell errors;
- hyphen-used-as-minus-sign errors for man pages;
- bug with dll override app run (q4wine-Bugs-3043548);
- bug with user console wine app run;
- strange locale bug reported by Taras Rozbitskiy;

## Version 0.119

Added:

- New Fake Drive settings dialog (Wizard now obsolete);
- Added a lot of new Fake Drive options;
- Desktop integration;
- Wine .desktop files import;
- Sync fake drive settings to head wine version;
- Various user interface enhancement;
- Updated Ukrainian, Italian and Spanish translations;
- Updated online documentation;

Fixed:

- Some fixes for GUI layout;
- Winetricks plugin improvement;
- Fixed bugs reported by users;
- Fixed some of English spellings;

## Version 0.118

Added:

- Wine AppDB support;
- Helper binary and Application logger subsystem;
- A lot of GUI improvements;
- You can mount iso images just by dropping them into q4wine icons list space;
- Automatic unmounting for mouned media before mount new one;
- Q4Wine now can be associated with application/x-ms-dos-executable mime type files;
- Added AUTHORS file;
- Added man pages;
- Try to get program's icon from ~/.local/share/icons by it's name;

Fixed:

- CMake now do check for fuseiso;
- CMake now make more verbose output;
- Total rewrite of CMake rules;
- A lot of fixes in Fake drive creation\update wizard;
- Fixed locale detection bugs;
- Fixes in tray icon support;
- Fixes in winetricks plugin;
- A lot fixes in q4wine.desktop file;
- Removed xdg-utils dependency;
- A lot of fixes and speedups;
- Fixed GUI issue with qtcurve theme engine;
- Fixed *.bat files drop;
- Removed _junk_ WineBinaryLauncher class;
- Removed _junk_ includes;
- MainWindow class cleanup;
- Deprecated wineprefixcreate command was replaced by wineboot;
- Fixed memory leaks reported by Valgrid;
- Fixed crush while drop existing icon;
- Fixed not critical error for fake C: symlink;
- Fixed white spaces catch in wine app run engine;
- Fixed cmake: appprefix setting now overrides default icon and desktop places;
- Fixed strange locale bug on some Slakware distros;

## Version 0.114-r1

Fixed:

- Fixed wrong wine C: drive symlink while create new Fake Drive (thx 2 Kyrill Detinov);
- Fixed some text layouts;

## Version 0.114

Added:

- New Registry read\write engine;
- New color option at Fake drive creation\update wizard;
- New wine settings in Fake drive creation\update wizard;
- Fake drive update wizard now can read actual wine registry configuration;
- Filter text field for q4wine icons;
- New Spanish translation (thx 2 Carlos Ignacio Ruax);
- New Italian translation (thx 2 Ercole Carpanetto);
- New FreeBSD mount and umount scripts (thx 2 Sergey V. Dyatko);

Fixed:

- Removed _junk_ translation strings from source code and UI;
- Fix for malfunctioning check whether or not to use ICOTOOLS. (thx 2 Impulze);
- Prefix combobox change do not set right Prefix in Prefix tree view;
- Fake drive update wizard now remove registry key while user reset it to 'Default' state;
- Fix for prefix import and export;
- And some small fixes for icoutils;

## Version 0.113

Added:
- Added embedded q4wine-mount (This is a copy of fuseiso + RH path);
	Note: use this if you too lazy to compile fuseiso from SF and apply paths;
- fuseiso ( http://sourceforge.net/projects/fuseiso/ ) full support.
- q4wine now remembers 8 recent mounted images;
- q4wine now remembers 8 recent launched binaries via Run dialog;
- QtSingleApp integration. Now you can run only one instance of q4wine-gui;
- Online documentation;
- New website: https://q4wine.brezblock.org.ua/;
- Now q4wine save last user selected prefix and dir;
- Added "open directory" menu items via xdg-utils;
- Added "open directory" menu items via winefile;
- Added xdg-utils support (note: now it is in the depends list)
- Added q4wine-cli console utility for wine applications and prefixes management.
- Added libq4wine implementation;
- Added translation file for Portuguese (Brazil) by Marcio Moraes;
- Added `*.xpm` filter to icon import patten;
- Added http proxy support for winetricks;
- Improved icon display widget;
- Added Drag & Drop support;
- Added Drag support q4wine icons export;
- Added Drop support for wine .exe and .com files;
- Added Drop support for wine .bat files (Now autoadd wineconsole binary args);
- Added splitter for programs and icons lists;
- Added command line option for q4wine (See q4wine --help for details);

Fixed:
- Linux: Improved wine process list build (thx Sergey Kishchenko (tilarids));
- Cleanup q4wine tmp directory on exit;
- Some fixes for q4wine.desktop (thx to Eugene Pivnev);
- Fixed GUI bug: Text fields size, on some desktop configurations, are too small to edit;
- Fixed a lot of English spelling errors (thx Sergey Kishchenko (tilarids));
- First steps for source code documentation via Doxygen....;
- Total code reorganize;
- Database engine rewrite;
- Many fixes for q4wine.desktop file;
- Fixed compilation with Qt-4.4.X;

## Version 0.112-r1

Fixed:

- Fixed compilation with Qt-4.4.X;
- Removed QDir::Files from Directory selection dialogs;
- Fixed compilation warning for registry.h;
- Fixed warnings ignoring return value of 'int system(const char*)' on 1666 and 1647 lines;

## Version 0.112

New:

- Added Sidebar Urls for Run and IconSettings file dialogs;
- Wine Dlls path autodetections; (If not work on your system -- plz mail to me ;) )
- You can disable icotools support in q4wine by adding `-DWITH_ICOTOOLS=OFF` to cmake;
- Winetools support is disabled by default. You can enable it by adding `-DWITH_WINETOOLS=ON` to `cmake` args;
- All developer stuff is disabled by default. You can enable it by adding `-DWITH_DEVELOP_STUFF=ON` to `cmake` args;
- New database class;
- `kdesu`\`gksu` support;

Fixed:

- Fixed import\export prefix functions;
- Many small fixes;
- Updated STDERR message catch function;
- Added FreeBSD port patches;
- Fixed readonly text field on prefix create;
- Fixed q4wine.desktop GENERIC names;
- Fixed IconSettings path detections (ProgramPath\WorkDirPath e.t.c.);
- Fixed .exe adding to IconName on Program file select;
- Run, AppSettings, ImageManager, Wizard and IconSettings dialogs now are modal dialogs;
- Fixed LC_ALL and LS_MESSAGES locale detection;
- Winetricks executable now puts into $HOME/.config/q4inwe/ directory;
- No sudo now needed for Winetricks download and install;

## Version 0.111

New:

- New q4wine icons with different sizes;
- Updated q4wine.desktop;
- Make install now install q4wine.desktop and icons;
- Added basic shortcuts;
- q4wine now save and restore size and position of main window;

Fixed:

- Fixed icoutils icon export bugs;
- Fixed file name case sense on icon export;
- Added .exe or .dll file selection masks on icon export;
- Fixed start selection file path on icon export;
- FreeBSD: kvm functions now do not need kmem gid;
- FreeBSD: wine_pthread now resolve exec name;
- Updated About dialog;
- Updated Ukrainian locales;

## Version 0.110

New:

- FreeBSD full support (proc and mounts);
- nrg images mount (Linux only);
- new core functions class (will be moved to libq4wine shared library);
- updated Ukrainian locale;
- basic wis xml parser class;
- Added nice level field to Run dialog;
- Added desktop size to Run dialog;

Fixed:

- Winetricks prefix fixes;
- FreeBSD kmem read exception check;
- Can't delete image from ImageManager;
- Table view and resize;
- Fixed getWineDll at Run dialog;
- User Locale detection bug;
- Copy & Paste issues;
- Removed deprecated konsole args;
- Fixed Prefix import bug;
- Fixed /bin/tar compiled path for import and export prefixes;
- Many fixes;
- Fixed CMake qm locale gen\install;

## Version 0.109-r2

New:

- New CMake (required version 2.6)) build system now used;
- Added q4wine.desktop file;
- New core class;

Experimental:

- Temporary winetricks support;
- Added winetricks download support;

Fixed:

- Fixed icon options edit error: "Debug: Parameter count mismatch";
- Fixed regression at icon options edit;
- Removed q4wine.pro;
(Note: qmake now deprecated, use cmake)
- updated INSTALL text file;
- Fixed empty console arguments process;

## Version 0.109-r1

Fixed:

- Fixed icon add error: "Debug: Parameter count mismatch";
- Fixed spaces in Program path and Working directory errors;
- Removed src.pro;
- Added INSTALL text file;

## Version 0.109

New:

- New CD image manager;
- New CD mount menu style;
- TrayIcon now shows and hide q4wine when double clicking on it;
- CD mount detection;
- Database version updater;

Fixed:

- Removed mount point and cd image settings at Icon Settings;
- Fixed prefix toolbar click crush;
- Fixed some English mistakes ;)
- Updated Ukrainian translation;
- Fixed icons show update at context menu request;
- Prefix setup bug with mount point and mount drive;
- Numerous fixes and code reorganization;
- Prefix Settings abd Prefix Wizard now use `/etc/fstab` instead of `/proc/diskstats`;
- Fixed QTranslator i18n settings load;

## Version 0.108

New:

- Fake drive wizard creates `autostart` folder.

Fixed:

- Copy issue.

## Version 0.107

New:

- Updated Ukrainian translation.
- Added `en_us` dummy translation (need professional English translation).

Fixed:

- Updated database creation wizard.
- CHANGED default database and config files location.
WARNING: You must manually delete all old files at:
$ rm -rdf $HOME/.q4wine
$ rm -rdf $HOME/.config/Brezerk\ GNU\ Soft

New files location:
$HOME/.config/q4wine
- Add outdated notifier messages.
- Fixed icon loose at PrefixSettings dialog.

## Version 0.106

New:

- Update prefix functions.

Fixed:

- Remove old prefix while creation new one.
- Run FakeDrive creation wizard after PrefixCreation wizard (If requested).
- SQL "Parameter count mismatch" at Prefix creation wizard.
- Removed version selector at PrefixCreation wizard.

## Version 0.105

New:
- New winetool class.
- Q4Wine now can import QT palette in to wine color settings

Fixed:

- Bug when deleting folder with icons.
- Bug in UpdateDatabaseConnectedItems.
- Crashes when lstFolders lose focus.

## Version 0.104

New:
- Fake drive creation wizard.
- Registry core functions.
- Fake drive wizard creates 'system' folder and wine utils icons.
- Fake drive wizard runs wineprefixcreate.

Version 0.103

New:
- Fake drive core functions.

Version 0.102

Fixed:
- Some stuff and optimization.

New:
- Display info and error notification when starts detached wine process.
- Now you can manually set wine desktop name and desktop size at icon settings.
- Now you can manually set _nice_ (man nice) priority at icon settings.
(Note: priority might by at range from 0 to 20)
- Now you can renice process priority using context menu at "Process" tab.
(Note: this action need root privileges. i.e. sudo app.)

Version 0.101

Fixed:

- Fixed TRANSLATIONS variable in q4wine.pro

New:

- Full professional Ukrainian translation by Chornoivan Uriy Olexeevich

Version 0.100

New:
- Full theme support;
- Translation support;

Fixes:

- "Programs" tab, was set as default at startup;
- Fixed tray icon display;
- Fixed "Can't find theme. File or path does not exist: Default" error message;
- Fixed tray icon image load;
- Fixed process output encoding;
- run.cpp moved to /src/gui directory;
- Fixed crashes of wrestool while getting icons;
- Fixed process exit code detection;
- Fixed envelopment variable detection;
- Fixed startup wizard, now it creates Default prefix;
