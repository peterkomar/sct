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
#include "mypartitions.h"

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

myPartitions::myPartitions(QObject *parent, const char *name)
 : QObject(parent, name)
{
}


myPartitions::~myPartitions()
{
}




/*!
    \fn myPartitions::getPartitions()
 */
QStringList myPartitions::getPartitions()
{
	QFile filePart("/proc/partitions");
	QStringList lines;
	int i;
	
	if ( filePart.open( IO_ReadOnly ) ) {
		QTextStream stream( &filePart );
		QString line;
          stream.readLine();
		while ( !stream.atEnd() ) {
			line = stream.readLine(); // line of text excluding '\n'
			i = line.find("h");
               if(i==-1)
                 i = line.find("s");//scsi or sda
               if(i==-1)
                 i = line.find("p");//Parallel port disks:
               if(i==-1)
                 i = line.find("m");//RAID
               if(i==-1)
                 i = line.find("c");//Compaq
               if(i==-1)
                 i = line.find("e");//MCA ESDI harddisks: /dev/ed[ab]
               if(i==-1)
                 i = line.find("x");//XT (8-bit) harddisks: /dev/xd[ab]
                              
			if(i!=-1)
				lines += "/dev/" + line.mid(i,line.length()-i);
		}
		lines+="/dev/fd0";
		lines+="/dev/fd1";
		filePart.close();
	}
	return lines;
}
