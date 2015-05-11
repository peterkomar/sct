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
#include "str_tools.h"

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qtextcodec.h>

static char Ch = 'C';
static char N = '1';
static char usb_N = '1';

myPartitions::myPartitions(QObject *parent, const char *name)
 : QObject(parent, name)
{
}


myPartitions::~myPartitions()
{
}

QStringList myPartitions::disklist()
{
	QStringList dsk;
	int fd;
	bool finished=false;
	#define CHECK_DEV(d)	if((fd=open(d, O_RDONLY))<0) { \
					finished=true; \
					continue; \
				} \
				close(fd); \
				dsk+=d
	QString dev="/dev/hd";	// IDE devices: /dev/hd[a-t]
	for(char a='a'; a<='t'; a++) {
		// We can't use finished for IDE drives - it's perfectly OK
		// to have a /dev/hdc but no /dev/hdb.
		CHECK_DEV(dev+a);
	}
	finished=false;
	dev="/dev/sd";		// SCSI disks 0-25
	for(char a='a'; a<='z' && !finished; a++) {
		CHECK_DEV(dev+a);
	}
	for(char prefix='a'; prefix <='d' && !finished; prefix++) {
		dev="/dev/sd" + prefix; // SCSI disks 26-127
		for(char a='a'; (a<='x' || (a<='z' && prefix<'d')) && !finished; a++) {
			CHECK_DEV(dev+a);
		}
	}
	finished=false;
	dev="/dev/i2o/hd";	// I2O disks 0-25: /dev/i2o/hd[a-z]
	for(char a='a'; a<='z' && !finished; a++) {
		CHECK_DEV(dev+a);
	}
	for(char prefix='a'; prefix <='d' && !finished; prefix++) {
		dev="/dev/i2o/hd" + prefix; // I2O disks 26-127
		for(char a='a'; (a<='x' || (a<='z' && prefix<'d')) && !finished; a++) {
			CHECK_DEV(dev+a);
		}
	}
	finished=false;
	dev="/dev/pd";		// Parallel port disks: /dev/pd[a-d]
	for(char a='a'; a<='d' && !finished; a++) {
		CHECK_DEV(dev+a);
	}
	finished=false;
	for(unsigned int i=0; i<=31 && !finished; i++) { // Software RAID
		QString device;
		device.sprintf("/dev/md%u", i);
		CHECK_DEV(device);
	}
	finished=false;
	for(unsigned int i=0; i<=7; i++) { //Compaq Smart Array
		for(unsigned int j=0; j<=15 && !finished; j++) {
			QString device;
			device.sprintf("/dev/ida/c%ud%u", i, j);
			CHECK_DEV(device);
		}
	}
	finished=false;
	for(unsigned int i=0; i<=7; i++) { // Mylex DAC960
		for(unsigned int j=0; j<=31 && !finished; j++) {
			QString device;
			device.sprintf("/dev/rd/c%ud%u", i, j);
			CHECK_DEV(device);
		}
	}
	finished=false;
	dev="/dev/ed";		// MCA ESDI harddisks: /dev/ed[ab]
	for(char a='a'; a<='b' && !finished; a++) {
		CHECK_DEV(dev+a);
	}
	finished=false;
	dev="/dev/xd";		// XT (8-bit) harddisks: /dev/xd[ab]
	for(char a='a'; a<='b' && !finished; a++) {
		CHECK_DEV(dev+a);
	}
	return dsk;
}


/*!
    \fn myPartitions::getPartitions()
 */
void myPartitions::get_part()
{
	QStringList s;
	QStringList d=disklist();
	for(QStringList::Iterator it=d.begin(); it!=d.end(); ++it) scan_part(*it);
	Ch='C';
	N = '1';
	usb_N = '1';
}


/*!
    \fn myPartitions::scan_part(QString part)
 */
QString myPartitions::scan_part(QString disk)
{
	QString cmd, tmp, s;
	
	bool b=false;
	myDataPartition data;
	cmd = QString("fdisk -l %1 2>&1").arg(disk);
	FILE *fdisk=popen(cmd, "r");
	char *buf=(char *) malloc(1024);
	QString dev;
	while(fgets(buf, 1024, fdisk)) {
		if(strlen(buf)<=1)
			continue;
		if(!b)
		{
		  if(strncmp("/dev/", buf, 5)!=0)
		  {
			  //while(isspace(*buf)) strcpy(buf, buf+1);
			  strcpy(buf, strchr(buf, ' ')); // skip Start
			  while(isspace(*buf)) strcpy(buf, buf+1);
			  *strchr(buf, ' ')=0;
			  s=buf;
			  trimStr(s,":");
			  data.Device = s;
			  
			  strcpy(buf, buf+strlen(buf)+1);
			  while(isspace(*buf)) strcpy(buf, buf+1);
			  *strchr(buf, ',')=0;
			  QTextCodec* codec = QTextCodec::codecForLocale();
			  
			  s = buf;
			  
			  s = codec->toUnicode( s );
			  data.SizeDevice = s;
		     b=true;
		     continue;
		  }
		}
	if(strncmp("/dev/", buf, 5)==0) { // Partition entry
			// We don't care about active vs. non-active partitions.
			// Remove the sign.
		if(strchr(buf, '?')>0)
		{
			data.part = data.Device;
			data.Size = data.SizeDevice;
			data.fileSystem = "USB Flash disk";
			data.nameDisk = QString("USB_%1").arg(usb_N);
			usb_N++;
			addListData(data);
			break;
		}
		
			while(strchr(buf, '*')) *strchr(buf, '*')=' ';
			// blanks are blanks...
			while(strchr(buf, '\t')) *strchr(buf, '\t')=' ';
			// Get the device
			*strchr(buf, ' ')=0;
			s = buf;
			data.part = s;
						
			strcpy(buf, buf+strlen(buf)+1);
			while(isspace(*buf)) strcpy(buf, buf+1);
			strcpy(buf, strchr(buf, ' ')); // skip Start
			while(isspace(*buf)) strcpy(buf, buf+1);
			strcpy(buf, strchr(buf, ' ')); // skip End
			while(isspace(*buf)) strcpy(buf, buf+1);
			*strchr(buf, ' ')=0;
			s = buf;
			trimStr(s,"+");
			int i=s.toInt();
			s=convertSize( i);
			data.Size = s;
			strcpy(buf, buf+strlen(buf)+1);
			strcpy(buf, strchr(buf, ' ')); // skip Blocks
			while(isspace(*buf)) strcpy(buf, buf+1);
			s = buf;
			if(s.find("f ")!=-1)
				continue;
			trimStr(s, "\n");
			data.fileSystem = s;
			
			if((s.find("FAT")!=-1) || (s.find("NTFS")!=-1))
			{
				data.nameDisk = Ch;
				Ch++;
			}
			else
			{
				s = QString("Disk_%1").arg(N);
				data.nameDisk = s;
				N++;
			}
			
			addListData(data);
		}
	}
	
	pclose(fdisk);
	free(buf);
	return QString("");
}


/*!
    \fn myPartitions::convertSize(int size)
 */
QString myPartitions::convertSize(int size)
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
		s.sprintf("%5.2f GB",fsize);
	}
   // Kilo-byte
	else if ( size > 1024 ) {
		fsize = ( float ) size / ( float ) 1024;
		s.sprintf("%5.0f MB",fsize);
	}
   // byte
	else {
		fsize = ( float ) size;
		s.sprintf("%5.0f kB",fsize);
	}
  
	return s;
}

void myPartitions::addListData(myDataPartition& data)
{
	myDataPartition* pNone = new myDataPartition; 
  
	pNone->Device = data.Device;
	pNone->MntPnt = data.MntPnt;
	pNone->Size = data.Size;
	pNone->Parameter = data.Parameter;
	pNone->fileSystem = data.fileSystem;
	pNone->freq = data.freq;
	pNone->passno = data.passno;
	pNone->part = data.part;
	pNone->nameDisk = data.nameDisk;
	pNone->SizeDevice = data.SizeDevice;
	pNone->b_mount = false;
	pNone->b_root = false;
  
	ListPart.append(pNone);
}
/*!
    \fn myPartitions::get_data()
 */
QPtrList<myDataPartition> myPartitions::get_data()
{
	get_part();
	return ListPart;
}



