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
//
#include "configurator_ui.h"
#include "str_tools.h"

#include <qfiledialog.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qdir.h>
#include <qapplication.h>
#include <qcombobox.h>
#include <qtextcodec.h>
#include <qmessagebox.h>


myConfImpl::myConfImpl(QWidget *parent, const char *name)
    :configurator(parent, name)
	,bLang(false)
{
  setMinimumSize(570,350);
  //setMaximumSize(570,350); //removte later
  setFixedSize(570,350);
  
  readConfig();
}

void myConfImpl::slotBrwsBoot()
{
  QString s = browseMetod();
  if(!s.isEmpty())
   lineboot->setText(s);
}

void myConfImpl::slotBPrinter()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineprinter->setText(s);
}


void myConfImpl::slotBKeyB()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linekeyboard->setText(s);
}


void myConfImpl::slotBSound()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linesound->setText(s);
}


void myConfImpl::slotBXSrv()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linexserver->setText(s);
}


void myConfImpl::slotBHwrInfo()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linehwinfo->setText(s);
}


void myConfImpl::slotBMount()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linemount->setText(s);
}

void myConfImpl::slotBMail()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linemail->setText(s);
}


void myConfImpl::slotBFTP()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineftp->setText(s);
}


void myConfImpl::slotBNFS()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linenfs->setText(s);
}


void myConfImpl::slotBSamba()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linesmb->setText(s);
}


void myConfImpl::slotBApache()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineApache->setText(s);
}


void myConfImpl::slotBNetwork()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linenetw->setText(s);
}


void myConfImpl::slotBPackage()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linepkgs->setText(s);
}

void myConfImpl::slotBSecurity()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineSecur->setText(s);
}


void myConfImpl::slotBRoot()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineroot->setText(s);
}


void myConfImpl::slotBServices()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineservic->setText(s);
}


void myConfImpl::slotBSyslog()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linesyslog->setText(s);
}


void myConfImpl::slotBDate()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linedata->setText(s);
}


void myConfImpl::slotBUser()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineuser->setText(s);
}

void myConfImpl::slotAply()
{
	if(bLang)
		QMessageBox::information(this,tr("Information"),tr("Change laguange after restart program"));	
  aplyParameter();
  accept();
}

void myConfImpl::slotProxy()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineproxy->setText(s);
}


void myConfImpl::slotScaner()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linescaner->setText(s);
}


void myConfImpl::slotMouse()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linemouse->setText(s);
}


void myConfImpl::slotDNS()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  linedns->setText(s);
}


void myConfImpl::slotDHCP()
{
  QString s = browseMetod();
  if(!s.isEmpty())
  lineDHCP->setText(s);
}








/*!
    \fn myConfImpl::browseMetod()
 */
QString myConfImpl::browseMetod()
{
	QFileDialog* fd = new QFileDialog( this,"file dialog", TRUE );
  fd->setMode(QFileDialog::ExistingFile);
  //fd->setDir("/");
  
  fd->setFilter( tr("Any files (*.*)") );
  
  fd->setViewMode( QFileDialog::Detail );
  fd->setCaption(tr("Select Execute file"));
  //fd->setPreviewMode(QFileDialog::Contents);
  //fd->selectAll(true); 
  QString s;
  if ( fd->exec() == QDialog::Accepted )
  {
       s = fd->selectedFile();
  }
   
  delete fd; 
  return s; 
}


/*!
    \fn myConfImpl::aplyParameter()
 */
void myConfImpl::aplyParameter()
{
  QSettings settings;
  /*settings.writeEntry( APP_KEY + "WindowWidth", width() );
  settings.writeEntry( APP_KEY + "WindowHeight", height() );
  settings.writeEntry( APP_KEY + "WindowX", x() );
  settings.writeEntry( APP_KEY + "WindowY", y() );*/
  
  QDir d = QDir::home();
  
  if(!d.exists(".SCT"))
    d.mkdir(".SCT");
  QString s = d.absPath();
  
   
 settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
  
    
  settings.writeEntry(APP_KEY+"Keyboard",linekeyboard->text());
  settings.writeEntry(APP_KEY+"Mount",linemount->text());
  settings.writeEntry(APP_KEY+"Sound",linesound->text());
  settings.writeEntry(APP_KEY+"Boot",lineboot->text());
  settings.writeEntry(APP_KEY+"XServer",linexserver->text());
  settings.writeEntry(APP_KEY+"Hardware",linehwinfo->text());
  settings.writeEntry(APP_KEY+"Printer",lineprinter->text());
  settings.writeEntry(APP_KEY+"Mouse",linemouse->text());
  settings.writeEntry(APP_KEY+"Scaner",linescaner->text());
  settings.writeEntry(APP_KEY+"FTPSrv",lineftp->text());
  settings.writeEntry(APP_KEY+"WebSrv",lineApache->text());
  settings.writeEntry(APP_KEY+"NFS",linenfs->text());
  settings.writeEntry(APP_KEY+"Network",linenetw->text());
  settings.writeEntry(APP_KEY+"MailSrv",linemail->text());
  settings.writeEntry(APP_KEY+"SambaSrv",linesmb->text());
  settings.writeEntry(APP_KEY+"PackageManager",linepkgs->text());
  settings.writeEntry(APP_KEY+"DNSSrv",linedns->text());
  settings.writeEntry(APP_KEY+"DHCPSrv",lineDHCP->text());
  settings.writeEntry(APP_KEY+"ProxySrv",lineproxy->text());
  settings.writeEntry(APP_KEY+"Config_User",lineuser->text());
  settings.writeEntry(APP_KEY+"Conf_Root_Psw",lineroot->text());
  settings.writeEntry(APP_KEY+"Conf_Service",lineservic->text());
  settings.writeEntry(APP_KEY+"Conf_Data_Time",linedata->text());
  settings.writeEntry(APP_KEY+"Conf_Security",lineSecur->text());
  settings.writeEntry(APP_KEY+"ViewLog",linesyslog->text());
  settings.writeEntry(APP_KEY+"Lang",lineLang->text());
    
  settings.writeEntry(APP_KEY+"Language_UI",langBox->currentText());
  
  QString pathApp; // = qApp->applicationDirPath();
  
  setAplDir(pathApp);
  
  settings.writeEntry(APP_KEY+"Install_Dir", pathApp+"/");
}

void myConfImpl::slotAddSysMenu()
{
  QDir d = QDir::home();
  
  QString s = d.absPath();
  
  QString bodyf = "[Desktop Entry]\nEncoding=UTF-8\nExec=\"";
  QString s1 = qApp->applicationDirPath()+"/sct_center\"";
  bodyf+=s1+"\nType=Application\nX-Created-by=SCT\n";
  bodyf+="Icon="+qApp->applicationDirPath()+"/pixmaps/tux_config.png";
  bodyf+"\nName=SCT control center";
  
  QString bodyd = "[Desktop Entry]\nType=Directory\nX-Created-by=SKsoft\n";
  bodyd+="Icon=\nName=SKsoft";
  bodyd+="\nX-KDE-Username=root";
  
  d.setPath(QDir::homeDirPath()+"/.kde/share/applnk");
  QFile file;
  
  if(d.exists())
  {
    d.mkdir("sksoft");
  
   d.cd("sksoft");
  
   QString path = d.absPath();
       
   s = path+"/.directory";
  
   file.setName( s );
   if ( file.open( IO_WriteOnly ) ) {
    QTextStream stream( &file );
    stream << bodyd;
    file.close();
  }
  
  s = path+"/SCT.desktop";
  
  file.setName( s );
  if ( file.open( IO_WriteOnly ) ) {
    QTextStream stream( &file );
    stream << bodyf;
    file.close();
  }
  }
  
  d.setPath(QDir::homeDirPath()+"/.gnome/apps");
  
  if(d.exists())
  {
    d.mkdir("sksoft");
  
    d.cd("sksoft");
  
    QString path = d.absPath();
  
    s = path+"/.directory";
  
    file.setName( s );
    if ( file.open( IO_WriteOnly ) ) {
      QTextStream stream( &file );
      stream << bodyd;
      file.close();
    }
  
    s = path+"/SCT.desktop";
  
    file.setName( s );
    if ( file.open( IO_WriteOnly ) ) {
      QTextStream stream( &file );
      stream << bodyf;
      file.close();
    }
  }        
}


void myConfImpl::slotAddDesktop()
{
  QString body = "[Desktop Entry]\nComment=Configure tool\n";
  
  //QString s = "Exec=";
  QString t = qApp->applicationDirPath();
  
  int i = t.findRev("/bin");
  
  t = t.mid(0,i);
  QString t1 = t;
  
	t += "/sct_center";
  
  t.insert(0,"Exec=");
  
  body+=t;
  
  
  body+="\nIcon="+t1+"/pixmaps/tux_config.png\n";
  body+="MimeType=\n";
  body+="Name=SCT control center\n";
  body+="Path=\n";
  body+="StartupNotify=true\n";
  body+="Terminal=false\n";
  body+="TerminalOptions=\n";
  body+="Type=Application\n";
  body+="X-DCOP-ServiceType=none\n";
  body+="X-KDE-SubstituteUID=true\n";
  body+="X-KDE-Username=";
    
  QDir d = QDir::home();
  
  QString path = d.absPath();  
  path+="/Desktop/SCT.desktop";
  
  QFile file( path );
  if ( file.open( IO_WriteOnly ) ) {
    QTextStream stream( &file );
    stream << body;
    file.close();
  }    
}



/*!
    \fn myConfImpl::readConfig()
 */
void myConfImpl::readConfig()
{
  QSettings settings;
  /*settings.writeEntry( APP_KEY + "WindowWidth", width() );
  settings.writeEntry( APP_KEY + "WindowHeight", height() );
  settings.writeEntry( APP_KEY + "WindowX", x() );
  settings.writeEntry( APP_KEY + "WindowY", y() );*/
  
  QDir d = QDir::home();
  
  
  
  QString s = d.absPath();
  
  settings.removeSearchPath( QSettings::Unix, s+"/.qt");
  settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
  
  /*settings.writeEntry( APP_KEY + "WindowWidth", width() );
  settings.writeEntry( APP_KEY + "WindowHeight", height() );
  settings.writeEntry( APP_KEY + "WindowX", x() );
  settings.writeEntry( APP_KEY + "WindowY", y() );*/
  
  s = settings.readEntry(APP_KEY+"Keyboard");
  
  linekeyboard->setText(settings.readEntry(APP_KEY+"Keyboard"));
  linemount->setText(settings.readEntry(APP_KEY+"Mount",""));
  linesound->setText(settings.readEntry(APP_KEY+"Sound",""));
  lineboot->setText(settings.readEntry(APP_KEY+"Boot",""));
  linexserver->setText(settings.readEntry(APP_KEY+"XServer",""));
  linehwinfo->setText(settings.readEntry(APP_KEY+"Hardware",""));
  lineprinter->setText(settings.readEntry(APP_KEY+"Printer",""));
  linemouse->setText(settings.readEntry(APP_KEY+"Mouse",""));
  linescaner->setText(settings.readEntry(APP_KEY+"Scaner",""));
  lineftp->setText(settings.readEntry(APP_KEY+"FTPSrv",""));
  lineApache->setText(settings.readEntry(APP_KEY+"WebSrv",""));
  linenfs->setText(settings.readEntry(APP_KEY+"NFS",""));
  linenetw->setText(settings.readEntry(APP_KEY+"Network",""));
  linemail->setText(settings.readEntry(APP_KEY+"MailSrv",""));
  linesmb->setText(settings.readEntry(APP_KEY+"SambaSrv",""));
  linepkgs->setText(settings.readEntry(APP_KEY+"PackageManager",""));
  linedns->setText(settings.readEntry(APP_KEY+"DNSSrv",""));
  lineDHCP->setText(settings.readEntry(APP_KEY+"DHCPSrv",""));
  lineproxy->setText(settings.readEntry(APP_KEY+"ProxySrv",""));
  lineuser->setText(settings.readEntry(APP_KEY+"Config_User",""));
  lineroot->setText(settings.readEntry(APP_KEY+"Conf_Root_Psw",""));
  lineservic->setText(settings.readEntry(APP_KEY+"Conf_Service",""));
  linedata->setText(settings.readEntry(APP_KEY+"Conf_Data_Time",""));
  lineSecur->setText(settings.readEntry(APP_KEY+"Conf_Security",""));
  linesyslog->setText(settings.readEntry(APP_KEY+"ViewLog",""));
  lineLang->setText(settings.readEntry(APP_KEY+"Lang",""));
  
  QString pathApp = qApp->applicationDirPath();
  
  int i;
  if(pathApp.right(1)=='/')
  {
	  i = pathApp.findRev('/');
    
	  pathApp.remove(i,3);
  }
  pathApp+="+";
  i = pathApp.findRev("/bin+");
  pathApp.remove(i,20);
  
  pathApp+="/lang";
  
  langBox->insertItem("en");
   
  QDir dir(pathApp); 
  QStringList fileNames = dir.entryList("sct_*.qm"); 
  
  for (int k = 0; k < (int)fileNames.size(); ++k) {
	  int j;
	  
	  i = fileNames[k].find("_");
	  j = fileNames[k].find(".",i+1);
	  
	  QString tmp = fileNames[k].mid(i+1,j-(i+1));
	  
	  langBox->insertItem(tmp);  
  }
  
  QString slang = settings.readEntry(APP_KEY+"Language_UI","en");
  
  langBox->setCurrentText(slang); 
}


/*!
    \fn myConfImpl::setEmptyConfig()
 */
void myConfImpl::setEmptyConfig()
{
  QSettings settings;
  /*settings.writeEntry( APP_KEY + "WindowWidth", width() );
  settings.writeEntry( APP_KEY + "WindowHeight", height() );
  settings.writeEntry( APP_KEY + "WindowX", x() );
  settings.writeEntry( APP_KEY + "WindowY", y() );*/
  
  int os=0; //=0 Red hat, =1 Mandrake, =2 Suse
  
  
  QDir d = QDir::home();
  
  
  //if(d.exists(".SCT"))
   // return;
  
  d.mkdir(".SCT");
  
  QString s = d.absPath();
  
   
  settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
   
  
  
  QFile file( "/etc/mandrakelinux-release");
  
  if(file.exists())
    os = 1;
  else{
    
    file.setName("/etc/mandriva-release");
      
    if(file.exists())
      os = 1; 
    else{
     file.setName("/etc/redhat-release");
    
     if(file.exists())
       os = 0;
     else{
      
       file.setName("/etc/SuSE-release");
      
       if(file.exists())
         os = 2;
      
      }
   }
  }
  
  QString linekeyboard1, linemount1, linesound1, lineboot1;
  QString linexserver1, linehwinfo1, lineprinter1, linemouse1, linescaner1;
  QString lineftp1, lineApache1, linenfs1, linenetw1, linemail1, linesmb1, linepkgs1;
  QString linedns1, lineDHCP1, lineproxy1, lineuser1, lineroot1, lineservic1, linedata1;
  QString lineSecur1, linesyslog1, lineLang1; //, connectInet;
  
  if(os == 0) //red hat and other
  {
    linekeyboard1="/usr/bin/system-config-keyboard";
    linemount1="usermount";
    linesound1="/usr/bin/system-config-soundcard";
    lineboot1="system-config-boot";
    linexserver1="/usr/bin/system-config-display";
    linehwinfo1="hwbrowser";
    lineprinter1="/usr/bin/printconf-gui";
    linemouse1="";
    linescaner1="";
    lineftp1="";
    lineApache1="system-config-httpd";
    linenfs1="/usr/bin/system-config-nfs";
    linenetw1="/usr/bin/system-config-network";
    linemail1="";
    linesmb1="/usr/bin/system-config-samba";
    linepkgs1="/usr/sbin/system-config-packages";
    linedns1="/usr/sbin/system-config-bind";
    lineDHCP1="";
    lineproxy1="";
    lineuser1="/usr/bin/system-config-users";
    lineroot1="/usr/bin/system-config-rootpassword";
    lineservic1="/usr/bin/system-config-services";
    linedata1="/usr/bin/system-config-date";
    lineSecur1="/usr/bin/system-config-securitylevel";
    linesyslog1="/usr/bin/system-logviewer";
    lineLang1 = "/usr/bin/system-config-language";
    //connectInet="/usr/bin/internet-druid"; 
  }
  else if(os == 1) //Mandrake or Mandriva
  {
    linekeyboard1="/usr/sbin/drakkeyboard";
    linemount1="/usr/sbin/drakdisk";
    linesound1="/usr/sbin/draksound";
    lineboot1="/usr/sbin/drakboot";
    linexserver1="/usr/sbin/drakx11";
    linehwinfo1="/usr/sbin/drakhardware";
    lineprinter1="/usr/sbin/drakprinter";
    linemouse1="/usr/sbin/drakmouse";
    linescaner1="/usr/sbin/drakscanner";
    lineftp1="/usr/sbin/drakwizard";
    lineApache1="/usr/sbin/drakwizard";
    linenfs1="/usr/sbin/drakwizard";
    linenetw1="/usr/sbin/drakconnect";
    linemail1="/usr/sbin/drakwizard";
    linesmb1="/usr/sbin/drakwizard";
    linepkgs1="/usr/sbin/rpmdrake";
    linedns1="/usr/sbin/drakwizard";
    lineDHCP1="/usr/sbin/drakwizard";
    lineproxy1="/usr/sbin/drakproxy";
    lineuser1="/usr/sbin/drakuser";
    lineroot1="";
    lineservic1="/usr/sbin/drakxservices";
    linedata1="/usr/sbin/drakclock";
    lineSecur1="/usr/sbin/draksec";
    linesyslog1="/usr/sbin/draklog";
    lineLang1 = "/usr/bin/system-config-language";
    //connectInet="/usr/bin/internet-druid";
  }
  else if(os == 2) //Suse
  {
    linekeyboard1="/sbin/yast2 keyboard";
    linemount1="/sbin/yast2 disk";
    linesound1="/sbin/yast2 sound";
    lineboot1="/sbin/yast2 bootloader";
	linexserver1="/usr/sbin/SaX2";
    linehwinfo1="/sbin/yast2 hwinfo";
    lineprinter1="/sbin/yast2 printer";
    linemouse1="/sbin/yast2 mouse";
    linescaner1="/sbin/yast2 scanner";
    lineftp1="/sbin/yast2 tftp-server";
    lineApache1="/sbin/yast2 http-server";
    linenfs1="/sbin/yast2 nfs_server";
    linenetw1="/sbin/yast2 lan";
    linemail1="/sbin/yast2 mail";
    linesmb1="/sbin/yast2 samba-server";
    linepkgs1="/sbin/yast2 -i";
    linedns1="/sbin/yast2 dns-server";
    lineDHCP1="/sbin/yast2 dhcp-server";
    lineproxy1="/sbin/yast2 proxy";
    lineuser1="/sbin/yast2 users";
    lineroot1="";
    lineservic1="/sbin/yast2 runlevel";
    linedata1="/sbin/yast2 timezone";
    lineSecur1="/sbin/yast2 security";
    linesyslog1="/sbin/yast2 view_anymsg";
    lineLang1 = "/sbin/yast2 language";
    //connectInet="/usr/bin/internet-druid";
  }
   
   
  settings.writeEntry(APP_KEY+"Keyboard",linekeyboard1);
  settings.writeEntry(APP_KEY+"Mount",linemount1);
  settings.writeEntry(APP_KEY+"Sound",linesound1);
  settings.writeEntry(APP_KEY+"Boot",lineboot1);
  settings.writeEntry(APP_KEY+"XServer",linexserver1);
  settings.writeEntry(APP_KEY+"Hardware",linehwinfo1);
  settings.writeEntry(APP_KEY+"Printer",lineprinter1);
  settings.writeEntry(APP_KEY+"Mouse",linemouse1);
  settings.writeEntry(APP_KEY+"Scaner",linescaner1);
  settings.writeEntry(APP_KEY+"FTPSrv",lineftp1);
  settings.writeEntry(APP_KEY+"WebSrv",lineApache1);
  settings.writeEntry(APP_KEY+"NFS",linenfs1);
  settings.writeEntry(APP_KEY+"Network",linenetw1);
  settings.writeEntry(APP_KEY+"MailSrv",linemail1);
  settings.writeEntry(APP_KEY+"SambaSrv",linesmb1);
  settings.writeEntry(APP_KEY+"PackageManager",linepkgs1);
  settings.writeEntry(APP_KEY+"DNSSrv",linedns1);
  settings.writeEntry(APP_KEY+"DHCPSrv",lineDHCP1);
  settings.writeEntry(APP_KEY+"ProxySrv",lineproxy1);
  settings.writeEntry(APP_KEY+"Config_User",lineuser1);
  settings.writeEntry(APP_KEY+"Conf_Root_Psw",lineroot1);
  settings.writeEntry(APP_KEY+"Conf_Service",lineservic1);
  settings.writeEntry(APP_KEY+"Conf_Data_Time",linedata1);
  settings.writeEntry(APP_KEY+"Conf_Security",lineSecur1);
  settings.writeEntry(APP_KEY+"ViewLog",linesyslog1);
  settings.writeEntry(APP_KEY+"Lang",lineLang1);
  //settings.writeEntry(APP_KEY+"ConnectInetViewLog",connectInet);
  
  settings.writeEntry(APP_KEY+"Language_UI","en");
  
  QString pathApp; // = qApp->applicationDirPath();
  
  setAplDir(pathApp);
  
  settings.writeEntry(APP_KEY+"Install_Dir", pathApp+"/");
}

void myConfImpl::slotBLang()
{
  QString s = browseMetod();
  if(!s.isEmpty())
    lineLang->setText(s);
}

void myConfImpl::langChange(const QString& )
{
	//
	bLang = true;
}


