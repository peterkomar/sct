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
#ifndef LILOMAIN_H
#define LILOMAIN_H

#include "lilo.h"
#include "editliloconf.h"

#include <qtextedit.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include <qlabel.h>


class liloMain: public lilo_view {
Q_OBJECT
public:
    liloMain(QWidget *parent = 0, const char *name = 0);
	~liloMain();
    static QString get_locale();
public slots:
    void slotEditItem(QListViewItem* );
public slots:
    virtual void slotSetParameter();
    virtual void slotAddNewItem();
    virtual void slotEditItem( );
    virtual void slotRemoveItem();
    virtual void helpAbout();
	 void fileExit();
    virtual void helpIndex();
private slots:
    void slotEditItem(myMenuLiloData ,QString oldLabel);
    void slotNewItem(myMenuLiloData );
    void slotClickTheme(QListBoxItem* );
    void slotReadOut(QString ,int );
    virtual void slotChangeTabs(int );
    void slotClickFin();
public:
    EditLiloConf *conf;
private:
    void loadData();
    void init_connections();
    void loadMenu();
    QTextEdit *out_message;
    QPushButton *finishBtn;
    QLabel *tex_lab;
  
    QVBox *v_box;
    int b_win;
	 
private slots:
    void slotSelect(int );
protected:
	virtual void closeEvent ( QCloseEvent * );	
};

#endif
