# Qt GUI for Wine

Q4Wine is a Qt GUI for Wine that helps manage Wine prefixes and installed applications.

[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner-direct.svg)](https://stand-with-ukraine.pp.ua)

## Notice

This software is published under the terms of the [GNU GPL V3](./COPYING) and comes with absolutely no warranty. You will NOT get any support or help for WineTools,
Wine, software installations, Linux, Microsoft Windows, and so forth from the author.

If you need help with Wine, send a message to the mailing lists at: http://www.winehq.org/site/forums.

If you want support, buy the commercial versions of Wine: CodeWeavers CrossOver Office
http://www.codeweavers.com for Desktop Applications.

Q4Wine was initially written by Oleksii S. Malakhov (also known as John Brezerk).

The general idea came from WineTools, which was initially written by Frank Hendriksen.

## Dependencies

- sqlite
- sudo
- which
- cmake >= 3.24
- qt6 >= 6.2.1 (core, gui, svg, network, xml, widgets)
- wine
- wget

## Optional dependencies

- fuseiso
- icoutils

## Installation

1. `$ tar -xvzf q4wine-x.xxx-rx.tar.gz`
2. `$ cd q4wine`
3. `$ mkdir build`
4. `$ cd build`
5. `$ cmake -DCMAKE_INSTALL_PREFIX=/usr ..`

## Build options:

By default, q4wine will install all files into these directories:

```
    CMAKE_INSTALL_PREFIX      : /usr/local
    DESKTOP_ENTRY_PATH        : /usr/local/share/applications
    PIXMAPS_ENTRY_PATH        : /usr/local/share/pixmaps
    MANPAGE_ENTRY_PATH        : /usr/local/share/man
    LIBS_ENTRY_PATH           : /usr/local/lib/q4wine   (for x86    systems)
    LIBS_ENTRY_PATH           : /usr/local/lib64/q4wine (for x86_64 systems)

    By default, q4wine will build and install all translations:

    LINGUAS                   : "af;cs;de;en;es;fa;fr;he;it;ja;pl;pt_BR;ru;uk;zh_TW"

    Optional build flags:     : Default | Possible


    DEBUG                     : OFF | ON
    RELEASE                   : ON  | OFF
    WITH_SYSTEM_SINGLEAPP     : ON  | OFF
    WITH_ICOUTILS             : ON  | OFF
    WITH_WINEAPPDB            : OFF | ON
    USE_BZIP2                 : OFF | ON
    USE_GZIP                  : OFF | ON
    WITH_DBUS                 : ON  | OFF

    DEBUG                     : If ON, then q4wine will send debug messages to STDOUT.
                             : Use it for development or bug reportint purposes;
    RELEASE                   : If OFF, then force q4wine to use APP_BUILD as CMAKE_INSTALL_PREFIX.
                             : Use it for development purposes;
    WITH_ICOUTILS             : If ON, then build q4wine with icoutils support;
    WITH_SYSTEM_SINGLEAPP     : If ON, then use system SingleApplication library instead building one;
    WITH_WINEAPPDB            : If ON, then build q4wine with Wine AppDB browser support (OBSOLETE);
    USE_BZIP2                 : Use bzip2 to compress man pages;
    USE_GZIP                  : Use gzip to compress man pages;
    WITH_DBUS                 : If ON, then build q4wine with QtDBus notification support;
```

6. `$ make`
7. `$ make install`
8. `$ q4wine`
9. Enjoy :)

## Post-install

> **NOTE**: 
These instructions may be outdated, so please see the latest version of this howto here: https://q4wine.brezblock.org.ua/documentation/en_us/02-post-installation.html

If you plan to use `kdesu`, `gksu` or `xdg-su`, then you may wish to add the following lines to `/etc/sudoers` (using `visudo`). This will allow one to run `/sbin/losetup`, which is used for building the mounted `.iso` menu lists without prompting for root password.

```
#---------- Cut here -------------------------
Cmnd_Alias Q4WINE_COMMS=/sbin/losetup
User_Alias Q4WINE_USERS=%games,%users

Q4WINE_USERS ALL=(ALL) NOPASSWD: Q4WINE_COMMS
#---------- End of Cut -----------------------
```

## Uninstall

To uninstall q4wine, run the following command from within q4wine's build root:

```bash
xargs rm < install_manifest.txt
```

You may wish to remove Desktop menu structure as well:

```bash
$HOME/.local/share/applications/q4wine
```

Use kbuildsycoca or kbuildsycoca6 (for KDE) to refresh application menu.


## Known Issues:

### Non-standard directories

If you have installed q4wine in a non-standard directory,
and you get the following error message: "error while loading shared libraries: libq4wine-core.so",
then you need to override LD_LIBRARY_PATH variable.

```bash
LD_LIBRARY_PATH={$path_to_q4wine_prefix}/lib/ q4wine
```

### q4wine prevents user from running winecfg from KRunner even after uninstalling:

You may wish to remove Desktop menu structure as well:

```bash
$HOME/.local/share/applications/q4wine
```

Use kbuildsycoca or kbuildsycoca6 (for KDE) to refresh application menu.

## Changelog

See [ChangeLog](./Changelog.md) file for details
