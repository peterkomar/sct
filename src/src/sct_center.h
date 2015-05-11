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


#ifndef MYCONFCENTER_H
#define MYCONFCENTER_H

#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <qaction.h>

#include "sct_views.h"

class myconfcenter: public QMainWindow
{
    Q_OBJECT

public:
    myconfcenter();
    ~myconfcenter();
    static QString get_locale();

private slots:
    void aboutQt();
    void msgClr();
    void msg(const QString& );
    void msg_info(const QString& );
    void updateGroupRightType(bool );

private:
	QAction *detalv;
	myconfviews *mainW;	
	
	int iviewr;//right panel
private:
    void loadMenu();
    void readSetings();
    void writeSetings();
    void init_connections();
public slots:
    void index();
    void about();
    void exit();
    void showPreferences();
};


#endif
