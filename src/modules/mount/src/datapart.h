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

#ifndef MYQDATAPART_H
#define MYQDATAPART_H 1

#include <qstring.h>

struct myDataPartition{
  QString Device; //device. sample /dev/hda
	QString SizeDevice;
  QString MntPnt; //mount point. sample /mnt/myC
  QString Size;   //size partition
  QString Parameter;
  QString fileSystem;
  QString FS_fstab;
  QString freq;
  QString passno;
  QString part; //partition. sample /dev/hda1
  bool b_mount;
  QString nameDisk;
  bool b_root;
};

#endif
