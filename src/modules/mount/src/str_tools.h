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
#ifndef str_tools
#define str_tools


#include <qstring.h>
#include <unistd.h>

inline void setAplDir(QString& path)
{
	//QString path = qApp->applicationDirPath();
  
     int i = path.findRev("/");
     path.remove(i,20);
     path += "/pixmaps/";
}

inline void trimRight(QString& str, QString pStr)
{
	while(!pStr.isEmpty())
	{
		while(str.right(1)==pStr.left(1))
			str.remove(str.length()-1,1);
		pStr.remove(0,1);
	}
};

inline void get_dev_name(QString& str)
{
	int i = str.find("(");
	int j = str.find(")");
	
	QString tmp = str.mid(i+1,j-i-1);
	
	str = tmp;
};

inline void trimLeft(QString& str, QString pStr)
{
	while(!pStr.isEmpty())
	{
		while(str.left(1)==pStr.left(1))
			str.remove(0,1);
		pStr.remove(0,1);
	}
};

inline void trimStr(QString& str, const char* pStr)
{
	trimLeft(str,pStr);
	trimRight(str,pStr);
};
#endif

//
