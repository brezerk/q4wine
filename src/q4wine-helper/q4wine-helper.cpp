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

#include "q4wine-helper.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream QErr(stderr);

     //! This is need for libq4wine-core.so import;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::unique_ptr<corelib> CoreLib;
    QLibrary libq4wine;

    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName(QString("%1/q4wine-lib/libq4wine-core").arg(QString::fromUtf8(APP_BUILD)));
#endif

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib class pointer
    CoreLibClassPointer = reinterpret_cast<CoreLibPrototype*>(libq4wine.resolve("createCoreLib"));
    CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(false)));

    if (!CoreLib.get()){
        QErr<<"[EE] Cannot load shared library."<<endl;
        return -1;
    }

    DataBase db;
    if (!db.checkDb()){
        QErr<<"[EE] Cannot initialize database engine."<<endl;
        return -1;
    }

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

    if (!CoreLib->isConfigured()){
       QErr<<"[EE] App is not configured! Rerun wizard, or delete Q4Wine broken config files."<<endl;
       return -1;
    }

    QTextStream Qcout(stdout);
    WineObject wineObject;

    if (argc==1){
        app.arguments().append("-h");
        argc++;
    }

    for (int i=1; i<argc; i++){
        qDebug()<<app.arguments().at(i);
        if ((app.arguments().at(1)=="--version") or (app.arguments().at(1)=="-v")){
            Qcout<<QString("%1-helper %2").arg(APP_SHORT_NAME).arg(APP_VERS)<<endl;
            Qcout<<QString("Copyright (C) 2008-2020 by Oleksii S. Malakhov <brezerk@gmail.com>")<<endl;
            Qcout<<QString("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.")<<endl;
            Qcout<<QObject::tr("This is free software: you are free to change and redistribute it.")<<endl;
            Qcout<<QObject::tr("There is NO WARRANTY, to the extent permitted by law.")<<endl;
            CoreLib->getBuildFlags();
            Qcout<<QObject::tr("Author: %1.").arg("Oleksii S. Malakhov")<<endl;
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
        } else if (app.arguments().at(i)=="--wrkdir") {
            i++;
            if (i<argc)
                wineObject.setProgramWrkdir(app.arguments().at(i));
        } else if (app.arguments().at(i)=="--program-lang") {
            i++;
            if (i<argc)
                wineObject.setProgramLang(app.arguments().at(i));
        } else if (app.arguments().at(i)=="--override") {
            i++;
            if (i<argc)
                wineObject.setProgramOverride(app.arguments().at(i));
        } else if (app.arguments().at(i)=="--prerun") {
            i++;
            if (i<argc)
                wineObject.setPreRun(app.arguments().at(i));
        } else if (app.arguments().at(i)=="--postrun") {
            i++;
            if (i<argc)
                wineObject.setPostRun(app.arguments().at(i));
        } else {
            Qcout<<QObject::tr("Usage:")<<endl;
            Qcout<<QObject::tr("  %1-helper [KEYs]...").arg(APP_SHORT_NAME)<<endl;
            Qcout<<QObject::tr("Console utility for Q4Wine which helps to handle Wine application exit status and its stdout/stderr output logging.")<<endl<<endl;
            Qcout<<QObject::tr("KEYs list:")<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --prefix"<<QObject::tr("sets the current prefix name")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --wine-debug"<<QObject::tr("sets WINEDEBUG variable")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --console"<<QObject::tr("run with output in console")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --display"<<QObject::tr("sets DISPLAY variable")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --nice"<<QObject::tr("sets program niceness")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --desktop"<<QObject::tr("sets program virtual desktop resolution")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --program-bin"<<QObject::tr("sets program binary")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --program-args"<<QObject::tr("sets program args")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --wrkdir"<<QObject::tr("sets program working directory")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --program-lang"<<QObject::tr("sets program LANG variable")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --override"<<QObject::tr("sets WINEDLLOVERRIDES variable")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --prerun"<<QObject::tr("execute script before program run")<<qSetFieldWidth(0)<<endl;
            Qcout<<qSetFieldWidth(25)<<left<<"  --postrun"<<QObject::tr("execute script after program run")<<qSetFieldWidth(0)<<endl;
            Qcout<<endl;
            Qcout<<QObject::tr("Report %1 bugs to %2").arg(APP_SHORT_NAME).arg(APP_BUG_EMAIL)<<endl;
            Qcout<<QObject::tr("%1 homepage: <%2>").arg(APP_SHORT_NAME).arg(APP_WEBSITE)<<endl;
            Qcout<<QObject::tr("General help using GNU software: <http://www.gnu.org/gethelp/>")<<endl;
            return 0;
        }
    }

    return wineObject.run();
}
