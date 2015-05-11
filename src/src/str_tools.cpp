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

#include "str_tools.h"

#include <qobject.h>
#include <qfile.h>
#include <qdir.h>
#include <qapplication.h>

//system information
#include <sys/utsname.h>
#include <linux/kernel.h>
#include <linux/sys.h>
#include <sys/sysinfo.h>

void setAplDir(QString& path)
{
  path = qApp->applicationDirPath();
  
  int i;
  if(path.right(1)=='/')
  {
    i = path.findRev('/');
    
    path.remove(i,3);
  }
  path+="+";
  i = path.findRev("/bin+");
  path.remove(i,20);
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

void trimLeft(QString& str, QString pStr)
{
  while(!pStr.isEmpty())
  {
    while(str.left(1)==pStr.left(1))
      str.remove(0,1);
    pStr.remove(0,1);
  }
}

void trimStr(QString& str, const char* pStr)
{
  trimLeft(str,pStr);
  trimRight(str,pStr);
}

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

QString get_info_from_file(const char* path_file, const char* name_info, const char* color)
{
  QString s_msg = "<table border=0 width=100%>";
  s_msg += QString("<tr bgcolor=\"lightGray\"><th>%1</th><tr>").arg(name_info);
  
  QString msg;
  
  QFile f_b(path_file);
  
  if(f_b.exists())
  {
    if ( f_b.open( IO_ReadOnly ) ) {
      QTextStream stream( &f_b );
      
      while ( !stream.atEnd() ) {
        
        QString line = stream.readLine();
        
        if(line.isEmpty())
          continue;
        
        line.replace("<","&lt;");
        
        QString s1 = QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"%1\">").arg(color);
         
        int i = line.find("=");
        
        if(i == -1)
          i = line.find(":");
        
        if(i!=-1)
        {
          QString s2 = "</font><font color=\"#0F990F\">";
          
          line.insert(i+1,s2);
          
          msg += s1+line+"</font></td></tr>";
        }
        else
          msg += s1 + line + "</font></td></tr>";
        
      }
      f_b.close();
    }
    else
      msg +=QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"red\">%1 : %2</font></td>").arg(QObject::tr("can't open file")).arg(path_file);
  }
  else
  {
    msg +=QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"red\">%1</font></td>").arg(QObject::tr("Not found"));
  }
	
  s_msg += msg;
  s_msg += "</table>";
  
  return s_msg;
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
  
  QString font = "<font size=+1 color=\"blue\"><b>%1</b> : </font><br><br><br>";
  
  QString s_msg = font.arg(QObject::tr("Disk's statistic"));
  
  s_msg += "<table border=0 width=100%>";
  s_msg += QString("<tr bgcolor=\"lightGray\"><th>%1</th><th>%2</th><th>%3</th><th>%4</th></tr>")
      .arg(QObject::tr("Device")).arg(QObject::tr("Size")).arg(QObject::tr("Available")).arg(QObject::tr("Mounted to"));
	
  s_msg += msg;
  s_msg += "</table>";
  
  return s_msg;
}

QString get_boot_statistic()
{
  QString font = "<font size=+1 color=\"blue\"><b>%1</b> : </font><br><br><br>";
  
  QString s_msg = font.arg(QObject::tr("Boot loaders information"));
  
  s_msg += get_info_from_file("/etc/grub.conf","Grub","blue");
  
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/etc/lilo.conf","Lilo","#E9D346");
  
 
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/root/.SCT/lilo.conf","SCT lilo","blue");
        
  return s_msg;
}

QString get_devices_statistic()
{
  QString font = "<font size=+1 color=\"blue\"><b>%1</b> : </font><br><br><br>";
  
  QString s_msg = font.arg(QObject::tr("Hardware information"));
  
  s_msg += get_info_from_file("/proc/cpuinfo","Processor","blue");
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/proc/meminfo","Memory","blue");
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/proc/ioports","IO ports","blue");
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/proc/devices","Devices","blue");
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/proc/interrupts","Interrupts","blue");
  
    
  return s_msg;
}

QString get_network_statistic()
{
  QString msg;
  QString font = "<font size=+1 color=\"blue\"><b>%1</b> : </font><br><br><br>";
  
  QString s_msg = font.arg(QObject::tr("Network"));
  
 /* struct sysinfo si;
  
  sysinfo(&si);*/
  
  struct utsname u;
  
  uname(&u);
  
  s_msg += "<table border=0 width=100%>";
  s_msg += QString("<tr bgcolor=\"lightGray\"><th>%1</th></tr>").arg(QObject::tr("Host name"));
  s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"blue\">%1</font></td></tr>").arg(u.nodename);
	
  s_msg += msg;
  s_msg += "</table>";
  
  s_msg += get_info_from_file("/etc/hosts","Hosts","blue");
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/etc/networks","Networks","blue");
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/etc/wvdial.conf","Wvdial","blue");
    
  return s_msg;
}

QString get_software_statistic()
{
  QString msg;
  QString font = "<font size=+1 color=\"blue\"><b>%1</b> : </font><br><br><br>";
  
  QString s_msg = font.arg(QObject::tr("Supported packages"));
  
  s_msg += "<table border=0 width=100%>";
  
  QFile f;
  QDir d;
  
  s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"blue\">%1</font></td>").arg(QObject::tr("Debian packages"));
  
  if(f.exists("/usr/bin/dpkg") || d.exists("/etc/apt"))
    s_msg += QString("<td><font color=\"#0F990F\">%1</font></td></tr>").arg(QObject::tr("Support"));
  else
    s_msg += QString("<td><font color=\"red\">%1</font></td></tr>").arg(QObject::tr("Not support"));
  
  s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"blue\">%1</font></td>").arg(QObject::tr("Rpm packages"));
  
  if(d.exists("/usr/lib/rpm"))
    s_msg += QString("<td><font color=\"#0F990F\">%1</font></td></tr>").arg(QObject::tr("Support"));
  else
    s_msg += QString("<td><font color=\"red\">%1</font></td></tr>").arg(QObject::tr("Not support"));
	
  s_msg += "</table>";
  
  return s_msg;
}

QString get_system_statistic()
{
  QString line;
  QString font = "<font size=+1 color=\"blue\"><b>%1</b> : </font><br><br><br>";
  
  struct utsname u;
  
  uname(&u);
  
  QFile file;
  
  QString name_lin, path_pix;
  QString path; // = qApp->applicationDirPath();
  setAplDir(path);
  path += "/pixmaps/";
    
  if(file.exists("/etc/redhat-release"))
  {
    file.setName("/etc/redhat-release"); 
    name_lin = "Fedora Core family";
    path_pix = path+"p_fedora_new.png";
    if ( file.open( IO_ReadOnly ) ) {
      QTextStream stream( &file );
      line = stream.readLine(); // line of text excluding '\n'
      file.close();
    }
  }
  if(file.exists("/etc/SuSE-release"))
  {
    file.setName("/etc/SuSE-release"); 
    name_lin = "SuSE family";
    path_pix = path+"p_suse.png";
    if ( file.open( IO_ReadOnly ) ) {
      QTextStream stream( &file );
      line = stream.readLine(); // line of text excluding '\n'
      file.close();
    }
  }
  if(file.exists("/etc/debian_version"))
  {
    file.setName("/etc/debian_version"); 
    name_lin = "Debian family";
    path_pix = path+"p_debian.png";
    if ( file.open( IO_ReadOnly ) ) {
      QTextStream stream( &file );
      line = stream.readLine(); // line of text excluding '\n'
      file.close();
    }
  }
  
  if(file.exists("/etc/mandrake-release"))
  {
    file.setName("/etc/mandrake-release"); 
    name_lin = "Mandrake family";
    path_pix = path+"p_mandrake.png";
    if ( file.open( IO_ReadOnly ) ) {
      QTextStream stream( &file );
      line = stream.readLine(); // line of text excluding '\n'
      file.close();
    }
  }
  if(file.exists("/etc/mandriva-release"))
  {
    file.setName("/etc/mandriva-release"); 
    name_lin = "Mandriva family";
    path_pix = path+"p_mandriva.png";
    if ( file.open( IO_ReadOnly ) ) {
      QTextStream stream( &file );
      line = stream.readLine(); // line of text excluding '\n'
      file.close();
    }
  }
  
    if(name_lin.isEmpty())
      name_lin = "Other linux family";
    if(path_pix.isEmpty())
      path_pix = path+"p_linux_new.png";
        
    if(line.isEmpty())
      line = "linux";
  


  
  QString s_msg = font.arg(QObject::tr("System"));
  
  s_msg += "<table border=0 width=100%>";
  s_msg += QString("<tr bgcolor=\"lightGray\"><th colspan=2>%1</th></tr>").arg(QObject::tr("Operation system"));
  s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><img src=\"%1\"></td><td><font color=\"blue\">%2</font></td>").arg(path_pix).arg(name_lin);
  s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"blue\">%1</td><td>%2</font></td>").arg(QObject::tr("Name")).arg(line);
  s_msg += "</table>";
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += "<table border=0 width=100%>";
  s_msg += QString("<tr bgcolor=\"lightGray\"><th colspan=2>%1</th></tr>").arg(QObject::tr("Kernel"));
  s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"blue\">%1 :</td><td>%2</font></td>").arg(QObject::tr("version")).arg(u.release);
  s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"blue\">%1</td><td>%2</font></td>").arg(QObject::tr("architecture")).arg(u.machine);
  s_msg += "</table>";
  
  s_msg += "<br>";
  s_msg += "<br>";
  
  s_msg += get_info_from_file("/etc/passwd","Users","blue");
  
  s_msg += "<br>";
  s_msg += "<br>";
    
  s_msg += "<table border=0 width=100%>";
  s_msg += QString("<tr bgcolor=\"lightGray\"><th>%1</th></tr>").arg(QObject::tr("Environments"));
  extern char** environ;
  
  char** var;
  
  for(var = environ; *var != NULL; ++var)
    s_msg += QString("<tr bgcolor=\"#EEF6FF\"><td><font color=\"#0F990F\">%1</font></td></tr>").arg(*var);
    s_msg += "</table>";
  
  
  return s_msg;
}

//
