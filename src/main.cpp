/***************************************************************************
 *   Copyright (C) 2006 by Martin Strasser                                 *
 *   strasser  a t  cadmaniac  d o t  org                                  *
 *   Special thanks to Mario Boikov                                        *
 *   squeeze  a t  cadmaniac  d o t  org                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "kontrollerlab.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <qtextcodec.h>

static const char description[] =
    I18N_NOOP("A KDE AVR Microcontroller integrated development environment.");

static const char version[] = "1.0.0-alpha1";

static KCmdLineOptions options;

int main(int argc, char **argv)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    /*KAboutData about("kontrollerlab", I18N_NOOP("KontrollerLab"), version, description,
		     KAboutData::License_GPL, "(C) 2006 - 2008 cadManiac.org", 0, 0, "help@cadmaniac.org");
    about.addAuthor( "Martin Strasser", 0, "strasser@cadmaniac.org" );
    about.addAuthor( "Mario Boikov", 0, "squeeze@cadmaniac.org" );
    about.addAuthor( "Thomas Grübler", "Debian package manager", "debian@cadmaniac.org" );
    about.addCredit( "Peter Fleury", "For the dot matrix LCD library",
                     "pfleury@gmx.ch", "http://www.jump.to/fleury" );
    about.addCredit( "Jürgen Eckert", "For fixing a bug in the avr-gcc linker command",
                     "je.sid@gmx.de" );
    about.addCredit( "Christian Waldherr", "For the Arch Linux packages of KontrollerLab",
                     "woodghost@online.de" );
    about.addCredit( "Christian Riggenbach", "For writing some patches and improvements for KontrollerLab",
                     "criggenbach@magahugu.net" );


*/

    KAboutData about(QByteArray("kontrollerlab"), QByteArray("KontrollerLab"), ki18n(version), QByteArray(description), ki18n(""),
                 KAboutData::License_GPL, ki18n("(C) 2006 - 2008 cadManiac.org"), ki18n(""), QByteArray(""), QByteArray("help@cadmaniac.org"));

    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );

    KApplication app;
    KontrollerLab *mainWin = 0;
    //if (app.isRestored())
    //{
        //RESTORE(KontrollerLab);
    //}
    //else
    //{
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        // If a URL is given, don't open the session project!
        if ( args->count() > 0 )
            mainWin = new KontrollerLab( args->url(0).isValid() );
        else
            mainWin = new KontrollerLab();
        app.setMainWidget( mainWin );
        mainWin->show();
        if ( args->count() > 0 && args->url(0).isValid() )
        {
            KUrl openMe = args->url(0);
            if ((!openMe.isEmpty()) && (openMe.isValid()))
            {
                mainWin->openProject( openMe );
            }
        }
        args->clear();
    //}

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

