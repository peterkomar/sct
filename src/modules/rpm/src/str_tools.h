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
#ifndef STR_TOOLS
#define STR_TOOLS


#include <qstringlist.h>
#include <unistd.h>
#include <qimage.h>
#include <qpixmap.h>

void setAplDir(QString& path);

void trimRight(QString& str, QString pStr);

bool cmp_str(const QString& str1/*what*/, const QString& str2/*in*/, bool bcase, bool babsolutely);

void get_dev_name(QString& str);

void trimLeft(QString& str, QString pStr);

//QPixmap setGray(QPixmap& pix);

void trimStr(QString& str, const char* pStr);

QString convert_size(int );
QString convert_size_disk(int );

QString get_disk_statistic();

#endif

//
