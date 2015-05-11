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
#ifndef MYCONFVIEWS_H
#define MYCONFVIEWS_H

#include <qvbox.h>
#include <qpixmap.h>

#include "str_tools.h"
#include "myqloader.h"

class QListViewItem;
class QListBoxItem;
class QIconViewItem;
class QProcess;
class QVBox;
class QHBox;
class QListView;
class QListBox;
class QIconView;
class QTextEdit;

/**
Central widget main window

@author Peter Komar
*/
class myconfviews : public QVBox
{
Q_OBJECT
public:
    myconfviews(QWidget* parent=0, const char* name=0);

    ~myconfviews();
    void setFullModeView(bool );
    bool load_Icons();

private:
    void create_GUI();
    void create_Left_Panel();
    void create_Right_Panel();
    void loadModulesToRightPanel(QString name_g/*name group in left panel*/);
    void load_Mod_App(QString ); //load modules or programs after click mouse button;
    void init_actions();
       	
	//members
	QListBox *g_tools;
	QListView *tools_full;
	QIconView *tools_short;
	QHBox *panel_group_tools;
	QVBox *panel_tools;
	QTextEdit *l_info;
	
	QPtrList<myInfo_modules> listModules;
	myQLoader *loader;
		
	//pixmaps
	//QPixmap pixBoot;
	QPixmap pixBootL;
	QPixmap pixMnt;
        QPixmap pixHwBr;
	QPixmap pixConfigNetwork;
	QPixmap pixConfigPackades;
	QPixmap pixConfig;
	
private slots:
    void slotClickFullMode(QListViewItem* );
    void slotClickLeftPanel(QListBoxItem* );
    void slotClickShortMode(QIconViewItem*  );

private slots:
    void restoreCursor();
    void slotOnItem(QIconViewItem* );
    void slotOnViewPort();
    void slotOnItem(QListViewItem* );
    void slotOnItem(QListBoxItem* );
signals:
    void msgClr();
    void statusMsg(const QString& );
    void status_msg_info(const QString& );
};

#endif
