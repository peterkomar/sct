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
//
#ifndef SELECTSOURCEIMPL_H
#define SELECTSOURCEIMPL_H

#include "selectsource.h"
#include "rpm_manager.h"

class QListViewItem;
class addSrcImpl;

class selectsourceImpl: public EditSource {
Q_OBJECT
public:
  selectsourceImpl(QWidget *parent = 0, const char *name = 0, bool modal=true, WFlags f=WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu);
	~selectsourceImpl();
private:
    QListViewItem* itemI;
    rpm_manager *manager;
    QString oldName;
    addSrcImpl *dlg;
    
    void loadData();
    void load_data(QPtrList<infoRpm> list);
    void load_hlp();
public slots:
    virtual void slotAdd();
    virtual void slotRemove();
    virtual void slotEdit();
    virtual void slotUpdateBtn();
    void slotGetMedia(QString , QString ,bool , int modif);
    virtual void slotYes();
    virtual void change_status();

signals:
    void finish_add(bool );
};

#endif
