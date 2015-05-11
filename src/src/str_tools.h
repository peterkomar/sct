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

const QString APP_KEY = "/SCT/";

void setAplDir(QString& path);
void trimRight(QString& str, QString pStr);
void trimLeft(QString& str, QString pStr);
void trimStr(QString& str, const char* pStr);

QString convert_size_disk(int );
QString get_info_from_file(const char* path_file, const char* name_info, const char* color);

QString get_boot_statistic();
QString get_disk_statistic();
QString get_devices_statistic();
QString get_network_statistic();
QString get_software_statistic();
QString get_system_statistic();

#endif

//
