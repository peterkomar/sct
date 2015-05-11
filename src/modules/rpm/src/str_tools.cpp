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

#include "str_tools.h"
#include <qbitmap.h>
#include <qobject.h>

void setAplDir(QString& path)
{
	//QString path = qApp->applicationDirPath();
  int i = path.findRev("/");
  path.remove(i,20);
  path += "/";
}

void trimRight(QString& str, QString pStr)
{
	while(!pStr.isEmpty())
	{
		while(str.right(1)==pStr.left(1))
			str.remove(str.length()-1,1);
		pStr.remove(0,1);
	}
}

bool cmp_str(const QString& str1/*what*/, const QString& str2/*in*/, bool bcase, bool babsolutely)
{
	if(str1.isEmpty())
		return false;
	
	if(str2.isEmpty())
		return false;
	
	if(babsolutely)
	{
		if(bcase){
			if(!str1.compare(str2))
				return true;
		}
		else
			if(str1 == str2)
				return true;
	}
	else
	{
		if(bcase){
			if(str2.find(str1,0,FALSE)!=-1)
				return true;
		}
		else
			if(str2.find(str1)!=-1)
				return true;
	}
	
	return false;
}


void get_dev_name(QString& str)
{
	int i = str.find("(");
	int j = str.find(")");
	
	QString tmp = str.mid(i+1,j-i-1);
	
	str = tmp;
}


void trimLeft(QString& str, QString pStr)
{
	while(!pStr.isEmpty())
	{
		while(str.left(1)==pStr.left(1))
			str.remove(0,1);
		pStr.remove(0,1);
	}
}
/*
QPixmap setGray(QPixmap& pix)
{
	QImage img, imgMask;
	img = pix;
	
	//img.setAlphaBuffer(true);
  //QBitmap hmask=pix.mask();
	imgMask = img.createHeuristicMask(FALSE);
		
	for(int y=0;y<img.height();y++)
		for(int x=0;x<img.width();x++)
	{
		if(!imgMask.pixel(x,y))
		{
			QRgb cl = img.pixel(x,y);
			int c = (qRed(cl)+qGray(cl)+qBlue(cl))/3;
			img.setPixel(x,y,qRgb(c,c,c));
		}
	}

	
	pix.convertFromImage(img);
	return pix;
}
*/
QString convert_size_disk(int size)
{
	float fsize;
	QString s;
   // Tera-byte
	if ( size >= 1073741824 ) {
		fsize = ( float ) size / ( float ) 1073741824;
		if ( fsize > 1024 )         // no name for something bigger than tera byte
         // let's call it Zega-Byte, who'll ever find out? :-)
			s.sprintf("%5.1f ZB", fsize / ( float ) 1024 );
		else
			s.sprintf("%5.1f TB", fsize );
	}
   // Giga-byte
	else if ( size >= 1048576 ) {
		fsize = ( float ) size / ( float ) 1048576;
		s.sprintf("%5.1f GB", fsize);
	}
   // Mega-byte
	else if ( size > 1024 ) {
		fsize = ( float ) size / ( float ) 1024;
		s.sprintf("%5.1f MB", fsize);
	}
   // Kilo-byte
	else {
		fsize = ( float ) size;
		s.sprintf("%5.1f KB", fsize);
	}
	return s;
}

QString convert_size(int size)
{
	float fsize;
	QString s;
    
    //size*=1024;
   // Tera-byte
	if ( size >= 1073741824 ) {
		fsize = ( float ) size / ( float ) 1073741824;
		if ( fsize > 1024 )         // no name for something bigger than tera byte
         // let's call it Tera-Byte, who'll ever find out? :-)
			s.sprintf("%5.2f TB",fsize / ( float ) 1024);
		else
			s.sprintf("%5.2f GB",fsize);
	}
	
   // Mega-byte
	else if ( size >= 1048576 ) {
		fsize = ( float ) size / ( float ) 1048576;
		s.sprintf("%5.2f MB",fsize);
	}
   // Kilo-byte
	else if ( size > 1024 ) {
		fsize = ( float ) size / ( float ) 1024;
		s.sprintf("%5.0f KB",fsize);
	}
   // byte
	else {
		fsize = ( float ) size;
		s.sprintf("%5.0f Byte",fsize);
	}
  
	return s;
}

QString get_disk_statistic()
{
	QString cmd, s, msg;
	int i=0;
		
	cmd = QString("df 2>&1");
	FILE *fdisk=popen(cmd, "r");
	char *buf=(char *) malloc(1024);
	QString dev;
	while(fgets(buf, 1024, fdisk)) {
		if(strlen(buf)<=1)
			continue;
		if(strncmp("/dev/", buf, 5)==0)
			{
			  //while(isspace(*buf)) strcpy(buf, buf+1);
				while(strchr(buf, '\t')) *strchr(buf, '\t')=' ';//replease all symbol \n in space
			// Get the device
				*strchr(buf, ' ')=0; //mid first string
				s = buf;//device
				msg +="<tr bgcolor=\"#EEF6FF\"><td>"+s+"</td>";
						
				strcpy(buf, buf+strlen(buf)+1); //step next word
				while(isspace(*buf)) strcpy(buf, buf+1);
				*strchr(buf, ' ')=0;
				s = buf;
				i = s.toInt();
				msg +="<td>"+convert_size_disk(i)+"</td>";//size
				strcpy(buf, buf+strlen(buf)+1);
				while(isspace(*buf)) strcpy(buf, buf+1);
				strcpy(buf, strchr(buf, ' ')); // skip used
				while(isspace(*buf)) strcpy(buf, buf+1);
				*strchr(buf, ' ')=0;
				s = buf;//Available
				i = s.toInt();
				msg +="<td>"+convert_size_disk(i)+"</td>";
				strcpy(buf, buf+strlen(buf)+1);
				while(isspace(*buf)) strcpy(buf, buf+1);
				strcpy(buf, strchr(buf, ' ')); // skip procent
				while(isspace(*buf)) strcpy(buf, buf+1);
				//*strchr(buf, ' ')=0;
				s = buf;
				trimStr(s,"\n");
				msg +="<td>"+s+"</td></tr>";//mounted
			}
	}
	
	pclose(fdisk);
	free(buf);
	return msg;
}

void trimStr(QString& str, const char* pStr)
{
	trimLeft(str,pStr);
	trimRight(str,pStr);
}