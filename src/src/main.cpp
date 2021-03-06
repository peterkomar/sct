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

#include "sct_center.h"
#include "str_tools.h"

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
    
    //lovale gui
    QTranslator appTranslator(0);
	
    QString path; // = qApp->applicationDirPath();
	
    setAplDir(path);
    
    QString slang = myconfcenter::get_locale(); 
	 
    appTranslator.load(QString( "sct_")+ slang, path+"/lang" );
	
    a.installTranslator(&appTranslator);
	
    
    myconfcenter * mw = new myconfcenter();
    mw->setCaption(QObject::tr("SCT control center Beta2 @SKsoft"));
    mw->resize(750,570);
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
