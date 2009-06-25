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

#include "q4wine-cli/main.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QTextStream Qcout(stdout);
	QString _PREFIX, _DIR, _ICON, _IMAGE;
	int _ACTION=0;

	//! This is need for libq4wine-core.so import;
	typedef void *CoreLibPrototype (bool);
	CoreLibPrototype *CoreLibClassPointer;
	corelib *CoreLib;

	//Classes
	DataBase db;
	Prefix *db_prefix;
	Dir *db_dir;
	Icon *db_icon;
	Image *db_image;

	if (!initDb())
	   return -1;

	QStringList tables;
	tables << "prefix" << "dir" << "icon" << "images";
	if (!db.checkDb(tables))
	   return -1;

	// Loading libq4wine-core.so
	QLibrary libq4wine;
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib = (corelib *)CoreLibClassPointer(false);

	// Creating database classes
	db_dir = new Dir();
	db_icon = new Icon();
	db_image = new Image();
	db_prefix = new Prefix();

	for (int i=1; i<argc; i++){
		if ((app.arguments().at(i)=="--help") or (app.arguments().at(i)=="-h")){
			_ACTION=-1;
			break;
		}
		if ((app.arguments().at(i)=="--version") or (app.arguments().at(i)=="-v")){
			_ACTION=11;
			break;
		}

		if ((app.arguments().at(i)=="--procs") or (app.arguments().at(i)=="-ps")){
			_ACTION=1;
		}

		if ((app.arguments().at(i)=="--prefix") or (app.arguments().at(i)=="-p")){
			i++;
			if (i<argc)
			_PREFIX=app.arguments().at(i);
			if (!db_prefix->isExistsByName(_PREFIX)){
				Qcout<<app.tr("Prefix named \"%1\" not exists. Run \"q4wine-cli -pl\" for prefix list.").arg(_PREFIX)<<endl;
				return -1;
			}
		}

		if ((app.arguments().at(i)=="--dir") or (app.arguments().at(i)=="-d")){
			i++;
			if (i<argc)
			_DIR=app.arguments().at(i);
		}

		if ((app.arguments().at(i)=="--icon") or (app.arguments().at(i)=="-i")){
			i++;
			if (i<argc)
			_ICON=app.arguments().at(i);
		}

		if ((app.arguments().at(i)=="--cdimage") or (app.arguments().at(i)=="-cd")){
			i++;
			if (i<argc)
			_IMAGE=app.arguments().at(i);
		}

		if ((app.arguments().at(i)=="--prefixlist") or (app.arguments().at(i)=="-pl")){
			_ACTION=2;
		}

		if ((app.arguments().at(i)=="--dirlist") or (app.arguments().at(i)=="-dl")){
			_ACTION=3;
		}

		if ((app.arguments().at(i)=="--iconlist") or (app.arguments().at(i)=="-il")){
			_ACTION=4;
		}

		if ((app.arguments().at(i)=="--cdlist") or (app.arguments().at(i)=="-cl")){
			_ACTION=5;
		}

		if ((app.arguments().at(i)=="--kill") or (app.arguments().at(i)=="-k")){
			_ACTION=6;
		}

		if ((app.arguments().at(i)=="--mount") or (app.arguments().at(i)=="-m")){
			_ACTION=7;
		}

		if ((app.arguments().at(i)=="--umount") or (app.arguments().at(i)=="-u")){
			_ACTION=8;
		}

		if ((app.arguments().at(i)=="--cdlist") or (app.arguments().at(i)=="-cl")){
			_ACTION=9;
			break;
		}

		if ((app.arguments().at(i)=="--mountlist") or (app.arguments().at(i)=="-ml")){
			_ACTION=10;
		}
	}

	QList<QStringList> result;
	QStringList sresult;
	QString path;
	switch (_ACTION){
		case 1:
			result = CoreLib->getWineProcessList();
			if (_PREFIX.isEmpty()){
				Qcout<<app.tr("Wine process list")<<endl;
			} else {
				Qcout<<app.tr("Wine process list for \"%1\" prefix").arg(_PREFIX)<<endl;
				path = db_prefix->getPath(_PREFIX);
			}
			// Preccess QList items one by one
			Qcout<<app.tr(" PID	  Nice	  Prefix path		  Name:")<<endl;
			for (int i = 0; i < result.size(); ++i) {
				if (_PREFIX.isEmpty()){
					Qcout<<" "<<result.at(i).at(0)<<"	  "<<result.at(i).at(2)<<"	  "<<result.at(i).at(3)<<"	  "<<result.at(i).at(1)<<endl;
				} else {
					if (path==result.at(i).at(3))
						Qcout<<" "<<result.at(i).at(0)<<"	  "<<result.at(i).at(2)<<"	  "<<result.at(i).at(3)<<"	  "<<result.at(i).at(1)<<endl;
				}
			}
		break;
		case 2:
			result = db_prefix->getFields();
			Qcout<<app.tr("Prefix list")<<endl;
			Qcout<<app.tr(" Name:		Path:")<<endl;
			for (int i = 0; i < result.size(); ++i) {
				if (!result.at(i).at(2).isEmpty()){
					Qcout<<" "<<result.at(i).at(1)<<"		"<<result.at(i).at(2)<<endl;
				} else {
					Qcout<<" "<<result.at(i).at(1)<<"	"<<QDir::homePath()<<"/.wine/"<<endl;
				}
			}
		break;
		case 3:
			if (_PREFIX.isEmpty()){
				Qcout<<app.tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			result = db_dir->getFieldsByPrefixName(_PREFIX);
			Qcout<<app.tr("Prefix \"%1\" has following dir list").arg(_PREFIX)<<endl;
			Qcout<<app.tr(" Name:")<<endl;
			for (int i = 0; i < result.size(); ++i) {
				Qcout<<" "<<result.at(i).at(1)<<endl;
			}
		break;
		case 4:
			if (_PREFIX.isEmpty()){
				Qcout<<app.tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}

			if (! db_dir->isExistsByName(_PREFIX, _DIR)){
				Qcout<<app.tr("Dir named \"%1\" not exists. Run \"q4wine-cli -dl\" for dir list.").arg(_DIR)<<endl;
				return -1;
			}

			result = db_icon->getByPrefixAndDirName(_PREFIX, _DIR);
			if (_DIR.isEmpty()){
				Qcout<<app.tr("Prefix \"%1\" has following icon list").arg(_PREFIX)<<endl;
			} else {
				Qcout<<app.tr("Prefix \"%1\" has following icon list at \"%2\" directory").arg(_PREFIX).arg(_DIR)<<endl;
			}
			Qcout<<app.tr(" Name:		Description:")<<endl;
			for (int i = 0; i < result.size(); ++i) {
				Qcout<<" "<<result.at(i).at(1)<<"	"<<result.at(i).at(2)<<endl;
			}
		break;
		case 5:
			result = db_image->getFields();
			Qcout<<app.tr("q4wine has following CD images at database")<<endl;
			Qcout<<app.tr(" Name:					Path:")<<endl;
			for (int i = 0; i < result.size(); ++i) {
				Qcout<<" "<<result.at(i).at(0)<<"			"<<result.at(i).at(1)<<endl;
			}
		break;
		case 6:
			if (_PREFIX.isEmpty()){
				Qcout<<app.tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			result = db_dir->getFieldsByPrefixName(_PREFIX);
			Qcout<<app.tr("Killing prefix \"%1\" wineserver.").arg(_PREFIX)<<endl;
			if (CoreLib->killWineServer(db_prefix->getPath(_PREFIX))){
				Qcout<<"Done"<<endl;
			} else {
				Qcout<<"Error"<<endl;
				return -1;
			}
		break;
		case 7:
			if (_PREFIX.isEmpty()){
				Qcout<<app.tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			sresult = db_prefix->getFieldsByPrefixName(_PREFIX);

			if (sresult.at(6).isEmpty()){
				Qcout<<app.tr("No mount point set in prefix configuration.")<<endl;
				return -1;
			}

			if (_IMAGE.isEmpty()){
				if (sresult.at(7).isEmpty()){
					Qcout<<app.tr("No cdrom drive set in prefix configuration.")<<endl;
					return -1;
				}
				Qcout<<app.tr("Mounting drive \"%1\" into mount point \"%2\".").arg(sresult.at(7)).arg(sresult.at(6))<<endl;
				if (CoreLib->mountImage(sresult.at(7), _PREFIX)){
					Qcout<<"Done"<<endl;
				} else {
					Qcout<<"Error"<<endl;
					return -1;
				}
			} else {
				if (!QFile(_IMAGE).exists()){
					if (!db_image->isExistsByName(_IMAGE)){
						Qcout<<app.tr("No CD iamge \"%1\" exists. Run \"q4wine-cli -cl\" for CD image list.").arg(_IMAGE)<<endl;
						return -1;
					}
				}

				if (CoreLib->mountImage(_IMAGE, _PREFIX)){
					Qcout<<"Done"<<endl;
				} else {
					Qcout<<"Error"<<endl;
					return -1;
				}
			}
		break;
		case 8:
			if (_PREFIX.isEmpty()){
				Qcout<<app.tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			sresult = db_prefix->getFieldsByPrefixName(_PREFIX);

			if (sresult.at(6).isEmpty()){
				Qcout<<app.tr("No mount point set in prefix configuration.")<<endl;
				return -1;
			}

			Qcout<<app.tr("Un mounting mount point \"%1\".").arg(sresult.at(6))<<endl;
			if (CoreLib->umountImage(_PREFIX)){
				Qcout<<"Done"<<endl;
			} else {
				Qcout<<"Error"<<endl;
				return -1;
			}
		break;
		case 9:
			result = db_image->getFields();
			Qcout<<app.tr("Image list")<<endl;
			Qcout<<app.tr(" Name:		Path:")<<endl;
			for (int i = 0; i < result.size(); ++i) {
				Qcout<<" "<<result.at(i).at(0)<<"		"<<result.at(i).at(1)<<endl;
			}
		break;
		case 10:
			if (_PREFIX.isEmpty()){
				result = db_prefix->getFields();
				Qcout<<app.tr("Mounted media list for all prefixes")<<endl;
				Qcout<<app.tr(" Prefix:	  Mount point:		Media:")<<endl;
				for (int i = 0; i < result.size(); ++i) {
						Qcout<<" "<<result.at(i).at(1)<<"	  "<<result.at(i).at(7)<<"		"<<CoreLib->getMountedImages(result.at(i).at(7))<<endl;
				}
			} else {
				sresult = db_prefix->getFieldsByPrefixName(_PREFIX);

				if (sresult.at(6).isEmpty()){
				Qcout<<app.tr("No mount point set in prefix configuration.")<<endl;
				return -1;
				}

				Qcout<<app.tr("Mounted media list for prefix \"%1\"").arg(_PREFIX)<<endl;
				Qcout<<app.tr(" Mount point:		Media:")<<endl;
				Qcout<<" "<<sresult.at(6)<<"	  "<<CoreLib->getMountedImages(sresult.at(6))<<endl;

			}
		break;
		case 11:
			Qcout<<app.tr("q4wine-cli %1").arg(APP_VERS)<<endl;
			Qcout<<app.tr("(Copyright (C) 2008-2009, brezblock core team.")<<endl;
			Qcout<<app.tr("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.")<<endl;
			Qcout<<app.tr("This is free software: you are free to change and redistribute it.")<<endl;
			Qcout<<app.tr("There is NO WARRANTY, to the extent permitted by law.")<<endl;
			Qcout<<endl;
			Qcout<<app.tr("Author: Malakhov Alexey aka John Brezerk.")<<endl;
		break;
		default:
			Qcout<<app.tr("Usage: q4wine-cli [KEY]...")<<endl;
			Qcout<<app.tr("Console utility for wine applications and prefixes management.")<<endl<<endl;
			Qcout<<app.tr("KEYs list:")<<endl;
			Qcout<<app.tr("  -h,  --help		  display this help and exit")<<endl;
			Qcout<<app.tr("  -v,  --version	  output version information and exit")<<endl;
			Qcout<<app.tr("  -ps, --procs		  output wine process list for current prefix or for all prefixes and exit ")<<endl;
			Qcout<<app.tr("  -p,  --prefix		  sets the current prefix name")<<endl;
			Qcout<<app.tr("  -d,  --dir		  sets the current direcory name")<<endl;
			Qcout<<app.tr("  -i,  --icon		  sets the current icon name")<<endl;
			Qcout<<app.tr("  -cd, --cdimage	  sets the cd iamge name")<<endl;
			Qcout<<app.tr("  -k,  --kill		  sends -9 term signal to current prefix precess or for all prefixes processes")<<endl;
			Qcout<<app.tr("  -pl, --prefixlist	  output all exesting prefixes names and exit")<<endl;
			Qcout<<app.tr("  -dl, --dirlist	  output all exesting dir names for current prefix and exit")<<endl;
			Qcout<<app.tr("  -il, --iconlist	  output all exesting icon names for current prefix/directory and exit")<<endl;
			Qcout<<app.tr("  -cl, --cdlist		  output all cd images list and exit")<<endl;
			Qcout<<app.tr("  -ml, --mountlist	  output all mounted media for current prefix or all prefixes and exit")<<endl;
			Qcout<<app.tr("  -m,  --mount		  mount an cd iamage or drive for current prefix and exit")<<endl;
			Qcout<<app.tr("  -u,  --umount		  umount an cd iamage or drive for current prefix and exit")<<endl;
			Qcout<<endl;
			Qcout<<app.tr("Exit status:")<<endl;
			Qcout<<app.tr("  0 if OK,")<<endl;
			Qcout<<app.tr("  -1 if serious troubles")<<endl;
			Qcout<<endl;
			Qcout<<app.tr("Report q4wine bugs to brezerk@gmail.com")<<endl;
			Qcout<<app.tr("q4wine homepage: <http://sourceforge.net/projects/q4wine/>")<<endl;
			Qcout<<app.tr("General help using GNU software: <http://www.gnu.org/gethelp/>")<<endl;
		break;
	}

	db.close();
	return 0;
}
