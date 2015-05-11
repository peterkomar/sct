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
#include "mymountmanager.h"
#include "mypartitions.h"

#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <fstab.h>
#include <qmessagebox.h>
#include <qtextcodec.h>

#include "str_tools.h"

const char* pathFSTAB = "/etc/fstab";
//const char* pathFSTAB = "/home/peter/develop/test/fstab"; //test path

myMountManager::myMountManager()
{
}


myMountManager::~myMountManager()
{
}

/*!
    \fn myMountManager::getMountedPartitions()
 */
QStringList myMountManager::getMountedPartitions()
{
  QString deviceStr, mntPStr, FSStr, parameterStr;
  QString freq, passno;
  
  myPartitions *part = new myPartitions;
  ListPart = part->get_data();
  delete part;
  
  
  myDataPartition *data = NULL;
  fstab* tab;
  tab = getfsent();
  int pas, frq;
  while(tab!=NULL)
  {  
    deviceStr = tab->fs_spec;
    mntPStr = tab->fs_file;
    FSStr = tab->fs_vfstype;
    parameterStr = tab->fs_mntops; //get parameter from list
    
    frq = tab->fs_freq;//first parameter(�������˦�)
    freq.setNum(frq,10);
    pas = tab->fs_passno;//last parameter(���///)
    passno.setNum(pas,10);
    
    data = (myDataPartition*)ListPart.first();
    
    while(data!=NULL)
    {
	    if(deviceStr == data->part)
	    {
		    data->MntPnt=mntPStr;
		    data->Parameter=parameterStr;
		    data->FS_fstab=FSStr;
		    data->freq = freq;
		    data->passno = passno;
		    break;
	    }
	    
	    data = (myDataPartition*)ListPart.next();
    }
    
    tab = getfsent();
  }
  endfsent();
  return QStringList("");
}


/*!
    \fn myMountManager::addDataToFstab(myDataPartition )
 */
void myMountManager::addDataToFstab(myDataPartition data)
{
  QString glob;
  bool bW=false;
  
  if(data.Parameter.isEmpty())
    data.Parameter = "defaults";
  
  QFile file( pathFSTAB );
  if ( file.open( IO_ReadOnly ) ) {
    QTextStream stream( &file );
    QString line;
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      
        if(line.find(data.part,0)!=-1)
        {
          line = data.part;
					line+="\t"+data.MntPnt+"\t"+data.FS_fstab+"\t"+data.Parameter+" "+data.freq+" "+data.passno;
          bW=true;
        }
      glob += line+"\n";
    }
    file.close();
    
    if(!bW)
    {
      line = data.part;
			line+="\t"+data.MntPnt+"\t"+data.FS_fstab+"\t"+data.Parameter+" "+data.freq+" "+data.passno;
      glob += line+"\n";
    }
  }
  else
    qFatal(tr("Can't open fstab file"));  
  
  QFile fileIn( pathFSTAB );
   if ( fileIn.open( IO_WriteOnly ) ) {
     QTextStream streamIn( &fileIn );
     streamIn << glob;
      file.close();
    }
    else
      qFatal(tr("Can't write to fstab file"));
}


/*!
    \fn myMountManager::removeDataFromFstab(QString )
 */
void myMountManager::removeDataFromFstab(QString str)
{
  QString glob;
	
	    
  QFile file( pathFSTAB );
  if ( file.open( IO_ReadOnly ) ) {
    QTextStream stream( &file );
    QString line;
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      
      if(line.find(str)!=-1)
        continue;
      
      glob += line+"\n";
    }
    file.close();
  }
  else
    qFatal(tr("Can't open fstab file"));  
  
  QFile fileIn( pathFSTAB );
  if ( fileIn.open( IO_WriteOnly ) ) {
    QTextStream streamIn( &fileIn );
    streamIn << glob;
    file.close();
  }
  else
    qFatal(tr("Can't write to fstab file"));
}


/*!
    \fn myMountManager::getPartition()
 */
QPtrList<myDataPartition> myMountManager::getPartition(bool update)
{
  if(update)
  {
      ListPart.clear();
      loadData();
  }
  
  return ListPart;
}


/*!
    \fn myMountManager::mountPartition(myDataPartition )
 */
int myMountManager::mountPartition(myDataPartition data, bool b)
{
  trimStr(data.Parameter,",");
	
	if(data.MntPnt == "swap")
	{
		addDataToFstab(data);
		return 0;
	}
  
  QDir d(data.MntPnt);
    
  if(!d.exists())
  {
    //QString s = data.MntPnt;
    //s.insert(0,"/home/peter");
    d.mkdir(data.MntPnt);
    //d.mkdir(s.section("/",1,1));
    //d.mkdir(s);
  }  
		
  pMount = new QProcess(this);

  pMount->addArgument( "mount" );
  pMount->addArgument( "-t" );
  pMount->addArgument( data.FS_fstab );
  pMount->addArgument("-o");
  pMount->addArgument(data.Parameter);
  pMount->addArgument(data.part);
  pMount->addArgument(data.MntPnt);
  
  connect( pMount, SIGNAL(readyReadStderr()),this, SLOT(slotDataErrorMount()));
  
  if ( !pMount->start() ) {
    qFatal(tr("can't start mount command"));
  }
  while(pMount->isRunning());
  QString t;
  
  delete pMount;
  t = getErrorr();
  if(!t.isEmpty())
  {
	  QTextCodec *codec = QTextCodec::codecForLocale(); 
	  QString uni = codec->toUnicode(t);
	  //t=t.unicode();
	  QMessageBox::critical(0,tr("Error mount"),uni);
     errorString="";
     return -1;
  }
  
  if(b)
    addDataToFstab(data);
  
  /*myDataPartition *dt = (myDataPartition*)ListPart.first();
  
  while(dt!=NULL)
  {
    if(dt->Device==data.Device)
    {
			dt->b_mount = true;
    }
    dt = (myDataPartition*)ListPart.next();
}*/
  return 0;
}


/*!
    \fn myMountManager::UmountPartition(myDataPartition )
 */
int myMountManager::UmountPartition(const char* name, bool force, bool del)
{
	if(force && del)
	{
		removeDataFromFstab(name);
		
		return 0;
	}
	
  QString s(name);
  
  pMount = new QProcess(this);

  pMount->addArgument( "umount" );
  
  if(force)
      pMount->addArgument( "-l" );
  pMount->addArgument(s);
    
  connect( pMount, SIGNAL(readyReadStderr()),this, SLOT(slotDataErrorMount()));
  
  if ( !pMount->start() ) {
    qFatal(tr("can't start umount command"));
  }
  while(pMount->isRunning());
  QString t;
  
  delete pMount;
  t = getErrorr();
  if(!t.isEmpty())
  {
	  QTextCodec *codec = QTextCodec::codecForLocale();
	  QString uni = codec->toUnicode(t);
	  //t=t.unicode();
	  
	  QMessageBox::critical(0,tr("Error umount"),uni);
	  errorString="";
    return -1;
  }
  
//  myDataPartition *dt = (myDataPartition*)ListPart.first();
  
  /*while(dt!=NULL)
  {
    if(dt->Device==s)
    {
      dt->MntPnt = "";
      dt->Parameter = "";
      dt->fileSystem = "";
      dt->freq = "";
      dt->passno = "";
			dt->b_mount = false;
      break;
    }
    dt = (myDataPartition*)ListPart.next();
}*/
  
  if(del)
  {
    removeDataFromFstab(s);
  }
   
  return 0;
}


/*!
    \fn myMountManager::loadData()
 */
void myMountManager::loadData()
{
  QString root_dev = getRootFS(); 
  getMountedPartitions();
	get_umount_part();
  
  myDataPartition *data = (myDataPartition*)ListPart.first();
    
  while(data!=NULL)
  {
	  if(root_dev == data->part)
	  {
		  data->b_root = true;
		  break;
	  }
	  data = (myDataPartition*)ListPart.next();
  }
}


/*!
    \fn myMountManager::getErrorr()
 */
QString myMountManager::getErrorr()
{
  return errorString;
}


/*!
    \fn myMountManager::slotDataErrorMount()
 */
void myMountManager::slotDataErrorMount()
{
  errorString += pMount->readStderr();
  errorString+="\n";
}


/*!
    \fn myMountManager::convertString(QString& str)
 */
void myMountManager::convertString(QString& str)//replaces all \t an space �� �������æ ��������
{
  unsigned int i=0;
  QChar c;
  QString Res;
  
  while(i<=str.length())
  {
    c = str.at(i);
    if(c=="\t")
      Res+=" ";
    else
      Res+=c;
    ++i;
  }
  str=Res;
}


/*!
    \fn myMountManager::getRootFS()
 */
QString myMountManager::getRootFS()
{
	const char s[]="/etc/mtab";
	
	QFile f(s);
	
	QString line, tmp, Res;
	int i;
	
	if ( f.open( IO_ReadOnly ) ) {
		QTextStream stream( &f );
    
		while(!stream.atEnd())
		{
			line = stream.readLine();
			
			convertString(line);
			
			i = line.find(" ");
			Res = line.mid(0,i);
			line.remove(0,i+1);
			i = line.find(" ");
			tmp = line.mid(0,i);
			
			if(tmp=="/")
				return Res;
		}
	}
	else
		qFatal(tr("Can't find mtab file "));
     return QString("error");
}


/*!
    \fn myMountManager::convertFS(QString )
 */
QString myMountManager::convertFS(QString str)
{
	if(str.find("FAT")!=-1)
		return QString("vfat");
	
	if(str.find("NTFS")!=-1)
		return QString("ntfs");
	
	if(str.find("83  Linux")!=-1)
		return QString("ext3");
	
	if(str.find("Linux swap")!=-1)
		return QString("swap");
	
	if(str.find("Linux")!=-1)
		return QString("ext2");
	
	else
		return QString("auto");
}


/*!
    \fn myMountManager::fast_mount(QString dev)
 */
QString myMountManager::fast_mount(QString dev)
{
	QString locale = readLocale();
	
	myDataPartition *data = (myDataPartition*)ListPart.first();
    
	while(data!=NULL)
	{
	 if(dev == data->part)
	 { 
	   if(data->MntPnt.isEmpty())
		   data->MntPnt="/mnt/"+data->nameDisk;
	   if(data->fileSystem.find("Linux")!=-1)
	   { 
	   if(data->Parameter.isEmpty())
 	    data->Parameter = QString("defaults").arg(locale);
	   }
	   else
	   {
             if(data->Parameter.isEmpty())
	     data->Parameter = QString("users,gid=users,umask=0002,iocharset=%1").arg(locale);
	   }
	   if(data->FS_fstab.isEmpty())
		   data->FS_fstab = convertFS(data->fileSystem);
	   if(data->fileSystem.find("Linux")!=-1)
	   {
	     if(data->freq.isEmpty())
	        data->freq = "1";
	     if(data->passno.isEmpty())
		data->passno = "2";
	   }
	   else
	   {
	     if(data->freq.isEmpty())
	      data->freq = "0";
	     if(data->passno.isEmpty())
	      data->passno = "0";
	   }
      	   data->b_mount = true;
			break;
	 }
	
	 data = (myDataPartition*)ListPart.next();
	}
	
	if((mountPartition(*data)) == -1)
	  return QString("");
	
	return data->MntPnt;
}


/*!
    \fn myMountManager::readLocale()
 */
QString myMountManager::readLocale()
{
	QString lines, line;
	
	line = QTextCodec::locale();
		
	int i = line.find(".");
	
	lines = line.mid(i,line.length());
	
	trimStr(lines,"\".");
	
	i = lines.find("-");
	
	if(i!=-1)
		lines.remove(i,1);
	
	
	return lines.lower();
}


/*!
    \fn myMountManager::get_umount_part()
 */
void myMountManager::get_umount_part()
{
	QString glob;
	
	QFile file("/etc/mtab");
	
	if ( file.open( IO_ReadOnly ) ) {
		QTextStream stream( &file );
		while ( !stream.atEnd() ) {
			glob += stream.readLine(); 
		}
		file.close();
	}
	else
	{
		QMessageBox::critical(0, tr("Error"), tr("Can't find <mtab> file."));
		return;
	}
	
  
	myDataPartition *data = NULL;
	int i=0;
	  
		data = (myDataPartition*)ListPart.first();
    
		while(data!=NULL)
		{
			if(glob.find(data->part) != -1)
			{
				data->b_mount = true;
				i=glob.find(data->part);
				int j = glob.find(" ", i+1);
				
				while(glob.mid(j,1) == " ")
					glob.remove(j,1);
				
				i = glob.find(" ",j+1);
				
				QString s = glob.mid(j,i-j+1);
				
				trimStr(s, " ");
				if(data->MntPnt.isNull())
				  data->MntPnt = s;
			}
	    
			data = (myDataPartition*)ListPart.next();
		}

}
