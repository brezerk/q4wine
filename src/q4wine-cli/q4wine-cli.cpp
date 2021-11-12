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

#include "q4wine-cli.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTextStream Qcout(stdout);
    QTextStream QErr(stderr);
    QString _PREFIX, _DIR, _ICON, _IMAGE;
    QString path;
    QStringList extra_args;
    int _ACTION=0;

    //! This is need for libq4wine-core.so import;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::unique_ptr<corelib> CoreLib;

    // Loading libq4wine-core.so
    QLibrary libq4wine;
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName(QString("%1/q4wine-lib/libq4wine-core").arg(QString::fromUtf8(APP_BUILD)));
#endif

    if (!libq4wine.load()){
        return -1;
    }

    // Getting corelib class pointer
    CoreLibClassPointer = reinterpret_cast<CoreLibPrototype*>(libq4wine.resolve("createCoreLib"));
    CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(false)));

    QTranslator qtt;

#ifdef RELEASE
    #ifdef _OS_DARWIN_
        QString l10nPath = QString("%1/%2.app/Contents/l10n").arg(QDir::currentPath()).arg(APP_SHORT_NAME);
    #else
        QString l10nPath = QString("%1/share/%2/l10n").arg(QString::fromUtf8(APP_PREF)).arg(APP_SHORT_NAME);
    #endif
#else
    QString l10nPath = QString("%1/l10n").arg(QString::fromUtf8(APP_BUILD));
#endif

    qtt.load(CoreLib->getTranslationLang(), l10nPath);
    app.installTranslator(&qtt);

    _ACTION=-1;

    for (int i=1; i<argc; i++){
        if ((app.arguments().at(i)=="--help") or (app.arguments().at(i)=="-h")){
            Qcout<<QObject::tr("Usage:")<<Qt::endl;
            Qcout<<QObject::tr("  %1-cli [KEY]...").arg(APP_SHORT_NAME)<<Qt::endl;
            Qcout<<QObject::tr("  %1-cli -p <prefix_name> [-d <dir_name>] -i <icon_name>").arg(APP_SHORT_NAME)<<Qt::endl;
            Qcout<<QObject::tr("  %1-cli -p <prefix_name> -b <windows_binary_path> [args]").arg(APP_SHORT_NAME)<<Qt::endl;
            Qcout<<QObject::tr("Console utility for wine applications and prefixes management.")<<Qt::endl<<Qt::endl;
            Qcout<<QObject::tr("KEYs list:")<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -h,  --help"<<QObject::tr("display this help and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -v,  --version"<<QObject::tr("output version information and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -ps, --procs"<<QObject::tr("output wine process list for current prefix or for all prefixes and exit ")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -p,  --prefix"<<QObject::tr("sets the current prefix name")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -d,  --dir"<<QObject::tr("sets the current directory name")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -i,  --icon"<<QObject::tr("sets the current icon name")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -cd, --cdimage"<<QObject::tr("sets the cd image name")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -b, --binary"<<QObject::tr("sets the path to windows binary for execute with current prefix settings")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -k,  --kill"<<QObject::tr("sends -9 term signal to current prefix process or for all prefixes processes")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -pl, --prefixlist"<<QObject::tr("output all existing prefixes names and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -dl, --dirlist"<<QObject::tr("output all existing dir names for current prefix and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -il, --iconlist"<<QObject::tr("output all existing icon names for current prefix/directory and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -cl, --cdlist"<<QObject::tr("output all cd images list and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -ml, --mountlist"<<QObject::tr("output all mounted media for current prefix or all prefixes and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -m,  --mount"<<QObject::tr("mount a cd image or drive for current prefix and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<qSetFieldWidth(25)<<Qt::left<<"  -u,  --umount"<<QObject::tr("unmount a cd image or drive for current prefix and exit")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<Qt::endl;
            Qcout<<QObject::tr("Exit status:")<<Qt::endl;
            Qcout<<QObject::tr("  0 if OK,")<<Qt::endl;
            Qcout<<QObject::tr(" -1 if serious troubles")<<Qt::endl;
            Qcout<<Qt::endl;
            Qcout<<QObject::tr("Report %1 bugs to %2").arg(APP_SHORT_NAME).arg(APP_BUG_EMAIL)<<Qt::endl;
            Qcout<<QObject::tr("%1 homepage: <%2>").arg(APP_WEBSITE).arg(APP_SHORT_NAME)<<Qt::endl;
            Qcout<<QObject::tr("General help using GNU software: <http://www.gnu.org/gethelp/>")<<Qt::endl;
            return 0;
        }
        if ((app.arguments().at(i)=="--version") or (app.arguments().at(i)=="-v")){
            Qcout<<QString("%1-cli %2").arg(APP_SHORT_NAME).arg(APP_VERS)<<Qt::endl;
            Qcout<<QString("Copyright (C) 2008-2021 by Oleksii S. Malakhov <brezerk@gmail.com>")<<Qt::endl;
            Qcout<<QString("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.")<<Qt::endl;
            Qcout<<QObject::tr("This is free software: you are free to change and redistribute it.")<<Qt::endl;
            Qcout<<QObject::tr("There is NO WARRANTY, to the extent permitted by law.")<<Qt::endl;
            CoreLib->getBuildFlags();
            Qcout<<QObject::tr("Author: %1.").arg("Oleksii S. Malakhov")<<Qt::endl;
            return 0;
        }
    }

    //Classes
    DataBase db;
    if (!db.checkDb())
        return -1;

    Prefix db_prefix;
    Dir db_dir;
    Icon db_icon;
    Image db_image;

    if (!CoreLib->isConfigured()){
        QErr<<"[EE] App not configured! Rerun wizard, or delete Q4Wine broken config files."<<Qt::endl;
        return -1;
    }

    for (int i=1; i<argc; i++){
        if ((app.arguments().at(i)=="--procs") or (app.arguments().at(i)=="-ps")){
            _ACTION=1;
            continue;
        }

        if ((app.arguments().at(i)=="--prefix") or (app.arguments().at(i)=="-p")){
            i++;
            if (i<argc){
                _PREFIX=app.arguments().at(i);
                continue;
            }
            if (!db_prefix.isExistsByName(_PREFIX)){
                Qcout<<QObject::tr("Prefix named \"%1\" does not exist. Run \"%2-cli -pl\" for prefix list.").arg(_PREFIX).arg(APP_SHORT_NAME)<<Qt::endl;
                return -1;
            }
        }

        if ((app.arguments().at(i)=="--dir") or (app.arguments().at(i)=="-d")){
            i++;
            if (i<argc)
                _DIR=app.arguments().at(i);
            continue;
        }

        if ((app.arguments().at(i)=="--icon") or (app.arguments().at(i)=="-i")){
            i++;
            if (i<argc)
                _ICON=app.arguments().at(i);
            if (_ACTION==-1)
                _ACTION=0;
            continue;
        }

        if ((app.arguments().at(i)=="--cdimage") or (app.arguments().at(i)=="-cd")){
            i++;
            if (i<argc)
                _IMAGE=app.arguments().at(i);
            continue;
        }

        if ((app.arguments().at(i)=="--prefixlist") or (app.arguments().at(i)=="-pl")){
            _ACTION=2;
            continue;
        }

        if ((app.arguments().at(i)=="--dirlist") or (app.arguments().at(i)=="-dl")){
            _ACTION=3;
            continue;
        }

        if ((app.arguments().at(i)=="--iconlist") or (app.arguments().at(i)=="-il")){
            _ACTION=4;
            continue;
        }

        if ((app.arguments().at(i)=="--cdlist") or (app.arguments().at(i)=="-cl")){
            _ACTION=5;
            continue;
        }

        if ((app.arguments().at(i)=="--kill") or (app.arguments().at(i)=="-k")){
            _ACTION=6;
            continue;
        }

        if ((app.arguments().at(i)=="--mount") or (app.arguments().at(i)=="-m")){
            _ACTION=7;
            continue;
        }

        if ((app.arguments().at(i)=="--umount") or (app.arguments().at(i)=="-u")){
            _ACTION=8;
            continue;
        }

        if ((app.arguments().at(i)=="--mountlist") or (app.arguments().at(i)=="-ml")){
            _ACTION=10;
            continue;
        }

        if ((app.arguments().at(i)=="--binary") or (app.arguments().at(i)=="-b")){
            i++;
            if (i<argc)
                _IMAGE=app.arguments().at(i);
            path.clear();
            for (int j=++i; j<argc; j++){
                path.append(app.arguments().at(j));
            }
            _ACTION=12;
            continue;
        }

        extra_args.append(app.arguments().at(i));
    }

    if (_ACTION==2){
        QStringList list = db_prefix.getPrefixList();
        Qcout<<QObject::tr("Prefix list")<<Qt::endl;
        Qcout<<" "<<qSetFieldWidth(15)<<Qt::left<<QObject::tr("Name")<<QObject::tr("Path")<<qSetFieldWidth(0)<<Qt::endl;
        for (int i = 0; i < list.size(); ++i) {
            QString path = db_prefix.getPath(list.at(i));
            if (!path.isEmpty()){
                Qcout<<" "<<qSetFieldWidth(15)<<Qt::left<<list.at(i)<<path<<qSetFieldWidth(0)<<Qt::endl;
            } else {
                Qcout<<" "<<qSetFieldWidth(15)<<Qt::left<<list.at(i)<<QDir::homePath()<<"/.wine/"<<qSetFieldWidth(0)<<Qt::endl;
            }
        }
        return 0;
    }

    QList<QStringList> result;
    QStringList sresult;

    ExecObject execObj;

    switch (_ACTION){
    case 0:
        // Running selected icon
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<Qt::endl;
            return -1;
        }

        if (_ICON.isEmpty()){
            Qcout<<QObject::tr("No current icon set. Set icon via \"-i <icon_name>\" key.")<<Qt::endl;
            return -1;
        }

        if (!db_icon.isExistsByName(_PREFIX, _DIR, _ICON)){
            Qcout<<QObject::tr("Icon named \"%1\" does not exist.  Run \"%2-cli -il\" for icon list.").arg(_ICON).arg(APP_SHORT_NAME)<<Qt::endl;
            return -1;
        }

        if (CoreLib->runIcon(_PREFIX, _DIR, _ICON, extra_args)){
            Qcout<<"Done"<<Qt::endl;
        } else {
            Qcout<<"Error"<<Qt::endl;
            return -1;
        }
        break;
    case 1:
        // Show wine process list
        result = CoreLib->getWineProcessList();
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("Wine process list")<<Qt::endl;
        } else {
            Qcout<<QObject::tr("Wine process list for \"%1\" prefix").arg(_PREFIX)<<Qt::endl;
            path = db_prefix.getPath(_PREFIX);
        }
        // Process QList items one by one
        Qcout<<" "<<qSetFieldWidth(8)<<Qt::left<<QObject::tr("PID")<<qSetFieldWidth(6)<<Qt::left<<QObject::tr("Nice")<<qSetFieldWidth(20)<<Qt::left<<QObject::tr("Name")<<QObject::tr("Prefix path")<<qSetFieldWidth(0)<<Qt::endl;
        for (int i = 0; i < result.size(); ++i) {
            if (_PREFIX.isEmpty()){
                Qcout<<" "<<qSetFieldWidth(8)<<Qt::left<<result.at(i).at(0)<<qSetFieldWidth(6)<<Qt::left<<result.at(i).at(2)<<qSetFieldWidth(20)<<Qt::left<<result.at(i).at(1)<<result.at(i).at(3)<<qSetFieldWidth(0)<<Qt::endl;
            } else {
                if (path==result.at(i).at(3))
                    Qcout<<" "<<qSetFieldWidth(8)<<Qt::left<<result.at(i).at(0)<<qSetFieldWidth(6)<<Qt::left<<result.at(i).at(2)<<qSetFieldWidth(20)<<Qt::left<<result.at(i).at(1)<<result.at(i).at(3)<<qSetFieldWidth(0)<<Qt::endl;
            }
        }
        break;
    case 3:
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<Qt::endl;
            return -1;
        } else {
            QStringList list = db_dir.getDirList(_PREFIX);
            Qcout<<QObject::tr("Prefix \"%1\" has following dir list").arg(_PREFIX)<<Qt::endl;
            Qcout<<" "<<QObject::tr("Name")<<Qt::endl;
            for (int i = 0; i < list.size(); ++i) {
                Qcout<<" "<<list.at(i)<<Qt::endl;
            }
        }
        break;
    case 4:
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<Qt::endl;
            return -1;
        }

        if (! db_dir.isExistsByName(_PREFIX, _DIR)){
            Qcout<<QObject::tr("Dir named \"%1\" does not exist. Run \"%2-cli -dl\" for dir list.").arg(_DIR).arg(APP_SHORT_NAME)<<Qt::endl;
            return -1;
        } else {
            QStringList list = db_icon.getIconsList(_PREFIX, _DIR, "");
            if (_DIR.isEmpty()){
                Qcout<<QObject::tr("Prefix \"%1\" has following icon list").arg(_PREFIX)<<Qt::endl;
            } else {
                Qcout<<QObject::tr("Prefix \"%1\" has following icon list at \"%2\" directory").arg(_PREFIX).arg(_DIR)<<Qt::endl;
            }
            Qcout<<" "<<qSetFieldWidth(15)<<Qt::left<<QObject::tr("Name")<<QObject::tr("Description")<<qSetFieldWidth(0)<<Qt::endl;

            for (int i = 0; i < list.size(); ++i) {
                Qcout<<" "<<qSetFieldWidth(15)<<Qt::left<<list.at(i)<<db_icon.getByName(_PREFIX, _DIR, list.at(i)).value("desc")<<qSetFieldWidth(0)<<Qt::endl;
            }
        }
        break;
    case 5:
        result = db_image.getFields();
        Qcout<<QObject::tr("%1 has the following disc images in the database").arg(APP_SHORT_NAME)<<Qt::endl;
        Qcout<<" "<<qSetFieldWidth(25)<<Qt::left<<QObject::tr("Name")<<QObject::tr("Path")<<qSetFieldWidth(0)<<Qt::endl;
        for (int i = 0; i < result.size(); ++i) {
            Qcout<<" "<<qSetFieldWidth(25)<<Qt::left<<result.at(i).at(0)<<result.at(i).at(1)<<qSetFieldWidth(0)<<Qt::endl;
        }
        break;
    case 6:
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<Qt::endl;
            return -1;
        } else {
            Qcout<<QObject::tr("Killing prefix \"%1\" wineserver.").arg(_PREFIX)<<Qt::endl;
            if (CoreLib->killWineServer(db_prefix.getPath(_PREFIX), "")){
                Qcout<<"Done"<<Qt::endl;
            } else {
                Qcout<<"Error"<<Qt::endl;
                return -1;
            }
        }
        break;
    case 7:
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<Qt::endl;
            return -1;
        } else {
            QString mount = db_prefix.getMountPoint(_PREFIX);

            if (mount.isEmpty()){
                Qcout<<QObject::tr("No mount point set in prefix configuration.")<<Qt::endl;
                return -1;
            }

            if (!_IMAGE.isEmpty()){
                if (!QFile(_IMAGE).exists()){
                    if (!db_image.isExistsByName(_IMAGE)){
                        Qcout<<QObject::tr("Disc image named \"%1\" does not exist. Run \"%2-cli -cl\" for disc image list.").arg(_IMAGE).arg(APP_SHORT_NAME)<<Qt::endl;
                        return -1;
                    }
                }

                if (CoreLib->mountImage(_IMAGE, _PREFIX)){
                    Qcout<<"Done"<<Qt::endl;
                } else {
                    Qcout<<"Error"<<Qt::endl;
                    return -1;
                }
            }
        }
        break;
    case 8:
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<Qt::endl;
            return -1;
        } else {
            QString mount = db_prefix.getMountPoint(_PREFIX);

            if (mount.isEmpty()){
                Qcout<<QObject::tr("No mount point set in prefix configuration.")<<Qt::endl;
                return -1;
            }

            Qcout<<QObject::tr("Unmounting mount point \"%1\".").arg(mount)<<Qt::endl;
            if (CoreLib->umountImage(_PREFIX)){
                Qcout<<"Done"<<Qt::endl;
            } else {
                Qcout<<"Error"<<Qt::endl;
                return -1;
            }
        }
        break;
    case 10:
        if (_PREFIX.isEmpty()){
            QStringList list = db_prefix.getPrefixList();
            Qcout<<QObject::tr("Mounted media list for all prefixes")<<Qt::endl;
            Qcout<<" "<<qSetFieldWidth(15)<<Qt::left<<QObject::tr("Prefix")<<qSetFieldWidth(25)<<Qt::left<<QObject::tr("Mount point")<<QObject::tr("Media")<<qSetFieldWidth(0)<<Qt::endl;
            for (int i = 0; i < list.size(); ++i) {
                QString mount = db_prefix.getMountPoint(list.at(i));
                Qcout<<" "<<qSetFieldWidth(15)<<Qt::left<<list.at(i)<<qSetFieldWidth(25)<<Qt::left<<mount<<CoreLib->getMountedImages(mount)<<qSetFieldWidth(0)<<Qt::endl;
            }
        } else {
            QString mount = db_prefix.getMountPoint(_PREFIX);

            if (mount.isEmpty()){
                Qcout<<QObject::tr("No mount point set in prefix configuration.")<<Qt::endl;
                return -1;
            }

            Qcout<<QObject::tr("Mounted media list for prefix \"%1\"").arg(_PREFIX)<<Qt::endl;
            Qcout<<" "<<qSetFieldWidth(25)<<Qt::left<<QObject::tr("Mount point")<<QObject::tr("Media")<<qSetFieldWidth(0)<<Qt::endl;
            Qcout<<" "<<qSetFieldWidth(25)<<Qt::left<<mount<<CoreLib->getMountedImages(mount)<<qSetFieldWidth(0)<<Qt::endl;
        }
        break;
    case 12:
        if (_PREFIX.isEmpty()){
            Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<Qt::endl;
            return -1;
        }

        qDebug()<<_IMAGE;

        if (!QFile(_IMAGE).exists()){
            Qcout<<QObject::tr("File \"%1\" does not exist.").arg(_IMAGE)<<Qt::endl;
            return -1;
        }

        sresult = _IMAGE.split("/");
        execObj.wrkdir = _IMAGE.left(_IMAGE.length() - sresult.last().length());
        execObj.override = "";
        execObj.winedebug = "";
        execObj.useconsole = "";
        execObj.display = "";
//        execObj.cmdargs = path;
        execObj.cmdargs = "";
        execObj.desktop = "";
        execObj.execcmd=_IMAGE;
        if (CoreLib->runWineBinary(execObj, _PREFIX)){
            Qcout<<"Done"<<Qt::endl;
        } else {
            Qcout<<"Error"<<Qt::endl;
            return -1;
        }
        break;
    default:
        exit (255);
    }

    db.close();
    return 0;
}
