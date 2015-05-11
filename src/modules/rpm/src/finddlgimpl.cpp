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
 
#include "finddlgimpl.h"

#include <qpixmap.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>


finddlgImpl::finddlgImpl(QWidget *parent, const char *name)
  :findDlg(parent, name) 
{
	bfile = false;
	f_find.b_file=false;
	f_find.b_case = false;
	f_find.b_descrip = false;
	f_find.b_name = false;
	f_find.b_summary = false;
	f_find.b_absol = false;
  
	find_line->setFocus();
}

void finddlgImpl::slotFind()
{
	QString s = find_line->currentText();
	find_line->insertItem(s);
	f_find.name = s.stripWhiteSpace();
	
	f_find.b_file = bfile;
	f_find.b_case = f_cmp_case->isChecked();
	f_find.b_descrip = f_description->isChecked();
	f_find.b_name = f_name->isChecked();
	f_find.b_summary = f_summary->isChecked();
	f_find.b_absol = f_absolutely->isChecked();
  
	emit sendName(f_find);
}

void finddlgImpl::slotSelect(int i)
{
  if(i)
		bfile=true;
  else
		bfile=false;
}


