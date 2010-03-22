/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                                 *
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
 ***************************************************************************/

#include "q4wine-helper.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream QErr(stderr);

     //! This is need for libq4wine-core.so import;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::auto_ptr<corelib> CoreLib;
    QLibrary libq4wine;

    // Loading libq4wine-core.so
    libq4wine.setFileName("libq4wine-core");

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(false));

    if (!CoreLib.get()){
        QErr<<"[EE] Can't load shared library."<<endl;
        return -1;
    }

    DataBase db;
    if (!db.checkDb()){
        QErr<<"[EE] Can't init database engine."<<endl;
        return -1;
    }

    QTranslator qtt;
    qtt.load(CoreLib->getTranslationLang(), QString("%1/share/%2/i18n").arg(APP_PREF).arg(APP_SHORT_NAME));
    app.installTranslator(&qtt);

    if (!CoreLib->isConfigured()){
       QErr<<"[EE] App not configured! Re run wizard, or delete q4wine broken config files."<<endl;
       return -1;
    }

    QTextStream Qcout(stdout);
    WineObject wineObject;

    if (argc==1){
        app.arguments().append("-h");
        argc++;
    }

    for (int i=1; i<argc; i++){
        if ((app.arguments().at(1)=="--version") or (app.arguments().at(1)=="-v")){
            Qcout<<QString("%1-helper %2").arg(APP_SHORT_NAME).arg(APP_VERS)<<endl;
            Qcout<<QString("(Copyright (C) 2008-2009, brezblock core team.")<<endl;
            Qcout<<QString("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.")<<endl;
            Qcout<<QObject::tr("This is free software: you are free to change and redistribute it.")<<endl;
            Qcout<<QObject::tr("There is NO WARRANTY, to the extent permitted by law.")<<endl;
            CoreLib->getBuildFlags();
            Qcout<<QObject::tr("Author: %1.").arg("Malakhov Alexey aka John Brezerk")<<endl;
            return 0;
        } else if (app.arguments().at(i)=="--prefix"){
            i++;
            if (i<argc)
                wineObject.setPrefix(app.arguments().at(i));
        }  else if (app.arguments().at(i)=="--wine-debug"){
            i++;
            if (i<argc)
                wineObject.setProgramDebug(app.arguments().at(i));
        }  else if (app.arguments().at(i)=="--console"){
            i++;
            if (i<argc)
                wineObject.setUseConsole(app.arguments().at(i).toInt());
        }  else if (app.arguments().at(i)=="--display"){
            i++;
            if (i<argc)
                wineObject.setProgramDisplay(app.arguments().at(i));
        }  else if (app.arguments().at(i)=="--nice"){
            i++;
            if (i<argc)
                wineObject.setProgramNice(app.arguments().at(i).toInt());
        }  else if (app.arguments().at(i)=="--desktop"){
            i++;
            if (i<argc)
                wineObject.setProgramDesktop(app.arguments().at(i));
        }  else if (app.arguments().at(i)=="--program-bin") {
            i++;
            if (i<argc)
                wineObject.setProgramBinary(app.arguments().at(i));
        } else if (app.arguments().at(i)=="--program-args") {
            i++;
            if (i<argc)
                wineObject.setProgramArgs(app.arguments().at(i));
        } else if (app.arguments().at(i)=="--override") {
            i++;
            if (i<argc)
                wineObject.setProgramOverride(app.arguments().at(i));
        } else {
            Qcout<<QObject::tr("Usage:")<<endl;
            Qcout<<QObject::tr("  %1-helper [KEYs]...").arg(APP_SHORT_NAME)<<endl;
            Qcout<<QObject::tr("Console utility for q4wine which helps to handle wine application exit status and it's stdout\\stderr output logging.")<<endl<<endl;
            Qcout<<QObject::tr("KEYs list:")<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --prefix"<<QObject::tr("sets the current prefix name")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --wine-debug"<<QObject::tr("sets WINEDEBUG variable")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --console"<<QObject::tr("run with output in console")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --display"<<QObject::tr("sets DISPLAY variable")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --program-bin"<<QObject::tr("sets program binary")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --program-args"<<QObject::tr("sets program args")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --override"<<QObject::tr("sets WINEDLLOVERRIDES variable")<<qSetFieldWidth(0)<<endl;
            Qcout<<endl;
            Qcout<<QObject::tr("Report %1 bugs to %2").arg(APP_SHORT_NAME).arg(APP_BUG_EMAIL)<<endl;
            Qcout<<QObject::tr("%1 homepage: <%2>").arg(APP_SHORT_NAME).arg(APP_WEBSITTE)<<endl;
            Qcout<<QObject::tr("General help using GNU software: <http://www.gnu.org/gethelp/>")<<endl;
            return 0;
        }
    }

    wineObject.runSys();
/*
    if (app.arguments().count()>1){
        if ((app.arguments().at(1)=="--version") or (app.arguments().at(1)=="-v")){
            Qcout<<QString("%1-helper %2").arg(APP_SHORT_NAME).arg(APP_VERS)<<endl;
            Qcout<<QString("(Copyright (C) 2008-2009, brezblock core team.")<<endl;
            Qcout<<QString("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.")<<endl;
            Qcout<<QObject::tr("This is free software: you are free to change and redistribute it.")<<endl;
            Qcout<<QObject::tr("There is NO WARRANTY, to the extent permitted by law.")<<endl;
            CoreLib->getBuildFlags();
            Qcout<<QObject::tr("Author: %1.").arg("Malakhov Alexey aka John Brezerk")<<endl;
            return 0;
        } else if ((app.arguments().at(1)=="--binary") or (app.arguments().at(1)=="-b")) {
            //startState = 1;
        } else {
            Qcout<<QObject::tr("Usage:")<<endl;
            Qcout<<QString("  %1 -b <unix_path_to_windown_binary>").arg(APP_SHORT_NAME)<<endl;
            Qcout<<QObject::tr("  %1 [KEY]...").arg(APP_SHORT_NAME)<<endl;
            Qcout<<QObject::tr("GUI utility for wine applications and prefixes management.")<<endl<<endl;
            Qcout<<QObject::tr("KEYs list:")<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  -h,  --help"<<QObject::tr("display this help and exit")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  -v,  --version"<<QObject::tr("output version information and exit")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  -b,  --binary"<<QObject::tr("Open q4wine run dialog for windows binary")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  -m,  --minimize"<<QObject::tr("minimize %1 main window on startup").arg(APP_SHORT_NAME)<<qSetFieldWidth(0)<<endl;
            Qcout<<endl;
            Qcout<<QObject::tr("Report %1 bugs to %2").arg(APP_SHORT_NAME).arg(APP_BUG_EMAIL)<<endl;
            Qcout<<QObject::tr("%1 homepage: <%2>").arg(APP_SHORT_NAME).arg(APP_WEBSITTE)<<endl;
            Qcout<<QObject::tr("General help using GNU software: <http://www.gnu.org/gethelp/>")<<endl;
            return 0;
        }
    }
*/

    return 0;
}
