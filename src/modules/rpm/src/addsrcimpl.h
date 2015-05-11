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
 
#ifndef ADDSRCIMPL_H
#define ADDSRCIMPL_H

#include "addsrcdlg.h"

 
class addSrcImpl: public AddSrcdlg {
Q_OBJECT
public:
    addSrcImpl(QWidget *parent = 0, const char *name = 0);
		void set_data(QString name, QString path, bool cd=false, int mode=1);/*1 - new media, 2 - edit media, 3 - update media*/
    virtual void show();
private:
    bool b_new;
    bool bCheked;
    int total;
    
    void load_hlp();
public slots:
    virtual void slotBrowse();
    void finish(bool er);
    void set_cur(int );
    void slot_progress(QString msg, int code);
    void set_total(int );
    void slot_cancel();
protected slots:
    void next();
signals:
    void send_data(QString name, QString path, bool cd, int code);//code =0 - change path, =1 - change name,
	                                                              //=2 - change cd/dvd, =3 - break process
    void abort_add();
};

#endif
