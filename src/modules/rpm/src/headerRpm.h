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
 
#ifndef HEADER_RPM_H
#define HEADER_RPM_H

#include <qstringlist.h>

struct infoRpm{
 QString group;  //group rpm
 QString name;   //name rpm
 QString deppend; 
 QString Description; 
 QStringList files;
 QStringList names_files;//provides 
 QString version;   
 QString release;   
 QString summary;   
 QStringList ChangeLog;
 QString size; 
 int isize; 
 QString nameSource;// used by myRPM inst
 QString pathSource;// used by myRPM inst
 QString arch; //architecture //samle i586, i386
 int cd; //=1 if removable device 0 else
 QString pathRPM; //path location rpm package
 bool b_on; //if on source or off
};

struct add_rem_data{
 QString name;
 QString deppend; 
 QStringList names_files;//provides 
 QString version;
 QString release;
 QString summary; 
 int isize;
 QString nameSource;// used by myRPM inst
 QString pathSource;// used by myRPM inst
 QString pathRPM; //path location rpm package
 QString arch; //architecture //samle i586, i386
 int cd; //=1 if removable device 0 else
 bool b_additional;//additional package or no
};
#endif
// // 