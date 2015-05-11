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
 
#ifndef FINDDLGIMPL_H
#define FINDDLGIMPL_H

#include "finddlg.h"

struct find_struct{
QString name;
bool b_file;
bool b_absol;
bool b_case;
bool b_name;
bool b_descrip;
bool b_summary;
};

class finddlgImpl: public findDlg {
Q_OBJECT
public:
    finddlgImpl(QWidget *parent = 0, const char *name = 0);
private:
    find_struct f_find;
    bool bfile;
public slots:
    virtual void slotSelect(int );
    virtual void slotFind();
signals:
    void sendName(find_struct );
};

#endif
