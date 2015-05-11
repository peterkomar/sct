/***************************************************************************
 *   Copyright (C) 2006 by Peter Komar                                     *
 *   markus_sksoft@mail.ru                                                 *
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

#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qdir.h>

#include "rpm_ui.h"

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
		
		
		QTranslator appTranslator(0);
	
		QString path = qApp->applicationDirPath();
  	        setAplDir(path);
                
                QString slang = rpm_ui::get_locale();
	 
                appTranslator.load(QString( "rpm_")+ slang, path+"/lang" );
                a.installTranslator(&appTranslator);
		
		bool b_i=false;
		if(argc>1)
		{
			QString s = argv[1];
			
			if(s == "-i")
				b_i = true;
		}
		rpm_ui * mw = new rpm_ui(b_i);
                                              
		mw->setCaption( "SCT rpm @BETA1" );
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
