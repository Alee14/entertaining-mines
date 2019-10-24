/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2019 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "gamewindow.h"

#include <tapplication.h>
#include <QStyleFactory>
#include <QDir>
#include <entertaining.h>

int main(int argc, char *argv[])
{
    tApplication a(argc, argv);

    if (QDir("/usr/share/entertaining-mines").exists()) {
        a.setShareDir("/usr/share/entertaining-mines");
    } else if (QDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../share/entertaining-mines/")).exists()) {
        a.setShareDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../share/entertaining-mines/"));
    }
    a.installTranslators();

    a.setOrganizationName("theSuite");
    a.setOrganizationDomain("");
    a.setApplicationIcon(QIcon::fromTheme("entertaining-mines", QIcon(":/icons/icon.svg")));
    a.setApplicationVersion("0.1");
    a.setGenericName(QApplication::translate("main", "Minesweeper"));
//    a.setAboutDialogSplashGraphic(a.aboutDialogSplashGraphicFromSvg(":/icons/aboutsplash.svg"));
    a.setApplicationLicense(tApplication::Gpl3OrLater);
    a.setCopyrightHolder("Victor Tran");
    a.setCopyrightYear("2019");
    #ifdef T_BLUEPRINT_BUILD
        a.setApplicationName(QApplication::translate("main", "Entertaining Mines Blueprint"));
    #else
        a.setApplicationName(QApplication::translate("main", "Entertaining Mines"));
    #endif

    a.setStyle(QStyleFactory::create("contemporary"));

    Entertaining::initialize();

    GameWindow w;
    w.show();
    return a.exec();
}
