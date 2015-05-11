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
#ifndef MYQLOADER_H
#define MYQLOADER_H

#include <qobject.h>
#include <qstring.h>
#include <qptrlist.h>
#include <qpixmap.h>

#include <qprocess.h>

struct myInfo_modules
{
	QString name; //name module
	QString group; //group module
	QString description; //descriptions of module
	QString exec; //exec name module or program
	bool module; //identificiren it this program or module
	QPixmap pixModule;//pixmap module
};

/**
Load modules (mylilo, mymount etc.) and load programs

	@author Peter Komar <peter_komar@mail.ru>
*/
class myQLoader : public QObject
{
Q_OBJECT
public:
    myQLoader();

    ~myQLoader();
	 QPtrList<myInfo_modules> getNamesModules();
    void loadModule(QString name);
    QString get_module_info(const QString& );

private:
    void findModules();
    void loadList();
	void addToList(myInfo_modules );
	void load_module_ex(const QString& ,bool ,const QString& nameModule, const QPixmap& pix);
    bool getModule_Info(myInfo_modules& );
    void load_Icons();
	 
	 //members
	 QPtrList<myInfo_modules> listModules;
	 QProcess *worker;
	 		 
	 //pixmaps
	 QPixmap pixBootRedHat;
     
	 QPixmap pixRedHatUserMount;
	 
	 QPixmap pixHwBrowser;
	 QPixmap pixDisplay;
	 QPixmap pixSoundcard;
	 QPixmap pixKeyboard;
	 QPixmap pixPrinter;
	 QPixmap pixMouse;
	 QPixmap pixScaner;
        
	 QPixmap pixFTP;
	 QPixmap pixMail;
	 QPixmap pixHTTP;
	 QPixmap pixNFS;
	 QPixmap pixSamba;
	 QPixmap pixNet;
	 QPixmap pixDNS;
	 QPixmap pixDHCP;
	 QPixmap pixProxy;
        
	 QPixmap pixPackage;
	 QPixmap pixRpm_add;
         QPixmap pixRpm_rem;
            
	 QPixmap pixDateTime;
	 QPixmap pixSysLog;
	 QPixmap pixUserConfig;
	 QPixmap pixServicess;
	 QPixmap pixRoot;
	 QPixmap pixSequrytiLevel;
	 QPixmap pixLang;
};

#endif
