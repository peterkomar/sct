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
#include "myqloader.h"
#include "str_tools.h"

#include <qdir.h>
#include <qsettings.h>
#include <qlibrary.h>
#include <qapplication.h>
#include <qstringlist.h>
#include <qtimer.h>

myQLoader::myQLoader()
{
  findModules();
  load_Icons();
  loadList();
  
  worker = new QProcess(this);
}


myQLoader::~myQLoader()
{
  delete worker;
}




/*!
    \fn myQLoader::findModules()
 */
void myQLoader::findModules()
{
  QString s; // = qApp->applicationDirPath();
  setAplDir(s);
  QDir d(s+"/modules");
  
  d.setFilter(QDir::Files);
  d.setNameFilter("*.so");
  
  const QFileInfoList *list = d.entryInfoList();
  QFileInfoListIterator it( *list );
  QFileInfo *fi;
  
  myInfo_modules one_module;

  while ( (fi = it.current()) != 0 ) {
	
	one_module.exec = fi->absFilePath();
	
	if(getModule_Info(one_module))
	  addToList(one_module);
	++it;
  }
}


/*!
    \fn myQLoader::loadList()
 */
void myQLoader::loadList()
{
	QSettings settings;
	QDir d = QDir::home();
  
	QString s = d.absPath();
	settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
   
	//read settings
	QString boot1 = settings.readEntry(APP_KEY+"Boot");
	
	QString mount = settings.readEntry(APP_KEY+"Mount");
	
	QString keyboard1 = settings.readEntry(APP_KEY+"Keyboard");
	QString sound1 = settings.readEntry(APP_KEY+"Sound","");
	QString xserver1 = settings.readEntry(APP_KEY+"XServer","");
	QString hwinfo1 = settings.readEntry(APP_KEY+"Hardware","");
	QString printer1 = settings.readEntry(APP_KEY+"Printer","");
	QString mouse1 = settings.readEntry(APP_KEY+"Mouse","");
	QString scaner1 = settings.readEntry(APP_KEY+"Scaner","");
	
	QString ftp1 = settings.readEntry(APP_KEY+"FTPSrv","");
	QString Apache1 = settings.readEntry(APP_KEY+"WebSrv","");
	QString nfs1 = settings.readEntry(APP_KEY+"NFS","");
	QString netw1 = settings.readEntry(APP_KEY+"Network","");
	QString mail1 = settings.readEntry(APP_KEY+"MailSrv","");
	QString smb1 = settings.readEntry(APP_KEY+"SambaSrv","");
	QString dns1 = settings.readEntry(APP_KEY+"DNSSrv","");
	QString DHCP1 = settings.readEntry(APP_KEY+"DHCPSrv","");
	QString proxy = settings.readEntry(APP_KEY+"ProxySrv","");
	
	QString package1 = settings.readEntry(APP_KEY+"PackageManager","");
        
        QString sct_rpm; // = qApp->applicationDirPath();
        setAplDir(sct_rpm);
        sct_rpm += "/modules/rpm";
        QFile fi(sct_rpm);
        if(!fi.exists())
          sct_rpm = "";
        	
	QString user1 = settings.readEntry(APP_KEY+"Config_User","");
	QString root1 = settings.readEntry(APP_KEY+"Conf_Root_Psw","");
	QString servic = settings.readEntry(APP_KEY+"Conf_Service","");
	QString date = settings.readEntry(APP_KEY+"Conf_Data_Time","");
	QString Secur = settings.readEntry(APP_KEY+"Conf_Security","");
	QString syslog = settings.readEntry(APP_KEY+"ViewLog","");
	QString lang = settings.readEntry(APP_KEY+"Lang","");
  
	myInfo_modules data;	
	
	if(!boot1.isEmpty()) 
	{
	  data.description = tr("Configure your boot loader");
	  data.name = tr("boot");
	  data.group = tr("Boot config");
	  data.exec = boot1;
	  data.module = false;
	  data.pixModule = pixBootRedHat;
	  
	  addToList(data);
	}
		 
	if(!hwinfo1.isEmpty())
	{
	  data.description = tr("Listen information of your computer");
	  data.name = tr("Hardware");
	  data.group = tr("Devices");
	  data.exec = hwinfo1;
	  data.module = false;
	  data.pixModule = pixHwBrowser;
	  
	  addToList(data);
	}
 
	if(!xserver1.isEmpty())
	{
	  data.description = tr("Configure your display");
	  data.name = tr("Display");
	  data.group = tr("Devices");
	  data.exec = xserver1;
	  data.module = false;
	  data.pixModule = pixDisplay;
	  
	  addToList(data);
	}
  
	if(!sound1.isEmpty())
	{
	  data.description = tr("Configure your sound cart");
	  data.name = tr("Soundcart");
	  data.group = tr("Devices");
	  data.exec = sound1;
	  data.module = false;
	  data.pixModule = pixSoundcard;
	  
	  addToList(data);
	}
 
	if(!keyboard1.isEmpty())
	{
	  data.description = tr("Configure your keyboard");
	  data.name = tr("Keyboard");
	  data.group = tr("Devices");
	  data.exec = keyboard1;
	  data.module = false;
	  data.pixModule = pixKeyboard;
	  
	  addToList(data);
	}
  
	if(!printer1.isEmpty())
	{
	  data.description = tr("Configure your printer");
	  data.name = tr("Printer");
	  data.group = tr("Devices");
	  data.exec = printer1;
	  data.module = false;
	  data.pixModule = pixPrinter;
	  
	  addToList(data);
	}
  
	if(!scaner1.isEmpty())
	{
	  data.description = tr("Configure your scaner");
	  data.name = tr("Scaner");
	  data.group = tr("Devices");
	  data.exec = scaner1;
	  data.module = false;
	  data.pixModule = pixScaner;
	  
	  addToList(data);
	}
  
  
	if(!mouse1.isEmpty())
	{
	  data.description = tr("Configure your mouse");
	  data.name = tr("Mouse");
	  data.group = tr("Devices");
	  data.exec = mouse1;
	  data.module = false;
	  data.pixModule = pixMouse;
	  
	  addToList(data);
	}
	
	if(!mount.isEmpty())
	{
	  data.description = tr("Your mount manager");
	  data.name = tr("Mount Points");
	  data.group = tr("Hard Disk");
	  data.exec = mount;
	  data.module = false;
	  data.pixModule = pixRedHatUserMount;
	  
	  addToList(data);
	} 
	
	if(!netw1.isEmpty()){
	  data.description = tr("Configure your network");
	  data.name = tr("Network");
	  data.group = tr("Network");
	  data.exec = netw1;
	  data.module = false;
	  data.pixModule = pixNet;
	  
	  addToList(data);
	}
   
	if(!Apache1.isEmpty()){
	  data.description = tr("Configure Web server");
	  data.name = tr("Web Server");
	  data.group = tr("Network");
	  data.exec = Apache1;
	  data.module = false;
	  data.pixModule = pixHTTP;
	  
	  addToList(data);
	}
  
	if(!smb1.isEmpty()){
	  data.description = tr("Configure Samba server");
	  data.name = tr("Samba");
	  data.group = tr("Network");
	  data.exec = smb1;
	  data.module = false;
	  data.pixModule = pixSamba;
	  
	  addToList(data);
	}
 
	if(!nfs1.isEmpty()){
	  data.description = tr("Configure NFS server");
	  data.name = tr("NFS");
	  data.group = tr("Network");
	  data.exec = nfs1;
	  data.module = false;
	  data.pixModule = pixNFS;
	  
	  addToList(data);
	}
  
	if(!ftp1.isEmpty()){
	  data.description = tr("Configure FTP server");
	  data.name = tr("FTP");
	  data.group = tr("Network");
	  data.exec = ftp1;
	  data.module = false;
	  data.pixModule = pixFTP;
	  
	  addToList(data);
	}
  
	if(!mail1.isEmpty()){
	  data.description = tr("Configure mail in your computer");
	  data.name = tr("Mail");
	  data.group = tr("Network");
	  data.exec = mail1;
	  data.module = false;
	  data.pixModule = pixMail;
	  
	  addToList(data);
	}
  
	if(!dns1.isEmpty()){
	  data.description = tr("Configure DNS Server");
	  data.name = tr("DNS");
	  data.group = tr("Network");
	  data.exec = dns1;
	  data.module = false;
	  data.pixModule = pixDNS;
	  
	  addToList(data);
	}
  
	if(!DHCP1.isEmpty()){
	  data.description = tr("Configure DHCP Server");
	  data.name = tr("DHCP");
	  data.group = tr("Network");
	  data.exec = DHCP1;
	  data.module = false;
	  data.pixModule = pixDHCP;
	  
	  addToList(data);
	}
  
	if(!proxy.isEmpty()){
	  data.description = tr("Configure Proxy");
	  data.name = tr("Proxy");
	  data.group = tr("Network");
	  data.exec = proxy;
	  data.module = false;
	  data.pixModule = pixProxy;
	  
	  addToList(data);
	}
	  
	if(!package1.isEmpty())
	{
	  data.description = tr("Your package manager");
	  data.name = tr("Packages");
	  data.group = tr("Software");
	  data.exec = package1;
	  data.module = false;
	  data.pixModule = pixPackage;
	  
	  addToList(data);
	}
        
        if(!sct_rpm.isEmpty())
        {
          data.description = tr("(Add packages)");
          data.name = tr("Add rpm");
          data.group = tr("Software");
          data.exec = sct_rpm+" -i";
          data.module = false;
          data.pixModule = pixRpm_add;
	  
          addToList(data);
          
          data.description = tr("(Remove packages)");
          data.name = tr("Remove rpm");
          data.group = tr("Software");
          data.exec = sct_rpm;
          data.module = false;
          data.pixModule = pixRpm_rem;
	  
          addToList(data);
        }
		
	if(!user1.isEmpty()){
	  data.description = tr("Configure users or groups in your computer");
	  data.name = tr("Users and Groups");
	  data.group = tr("System");
	  data.exec = user1;
	  data.module = false;
	  data.pixModule = pixUserConfig;
	  
	  addToList(data);  
	}
  
	if(!date.isEmpty()){
	  data.description = tr("Configure date and time");
	  data.name = tr("Date and Time");
	  data.group = tr("System");
	  data.exec = date;
	  data.module = false;
	  data.pixModule = pixDateTime;
	  
	  addToList(data);
	}
  
	if(!syslog.isEmpty()){
	  data.description = tr("Listen System Log files");
	  data.name = tr("Sys Log");
	  data.group = tr("System");
	  data.exec = syslog;
	  data.module = false;
	  data.pixModule = pixSysLog;
	  
	  addToList(data);
	}
  
	if(!servic.isEmpty()){ 
	  data.description = tr("Configure Services in your Computer");
	  data.name = tr("Services");
	  data.group = tr("System");
	  data.exec = servic;
	  data.module = false;
	  data.pixModule = pixServicess;
	  
	  addToList(data);
	}
  
	if(!root1.isEmpty()){
	  data.description = tr("Change root password");
	  data.name = tr("Root Password");
	  data.group = tr("System");
	  data.exec = root1;
	  data.module = false;
	  data.pixModule = pixRoot;
	  
	  addToList(data);
	}
  
	if(!Secur.isEmpty()){
	  data.description = tr("Configure Security in your Computer");
	  data.name = tr("Security");
	  data.group = tr("System");
	  data.exec = Secur;
	  data.module = false;
	  data.pixModule = pixSequrytiLevel;
	  
	  addToList(data);
	}
  	
	if(!lang.isEmpty()){
	  data.description = tr("Select languange in your computer");
	  data.name = tr("Language");
	  data.group = tr("System");
	  data.exec = lang;
	  data.module = false;
	  data.pixModule = pixLang;
	  
	  addToList(data);
	}
}


/*!
    \fn myQLoader::getNamesModules()
 */
QPtrList<myInfo_modules> myQLoader::getNamesModules()
{
	return listModules;
}


/*!
    \fn myQLoader::loadModule(QString name)
 */
void myQLoader::loadModule(QString name)
{
	myInfo_modules *p_data=NULL;
	for ( p_data = listModules.first(); p_data; p_data = listModules.next() )
	{
		if(!name.compare(p_data->name))
		{
			load_module_ex(p_data->exec, p_data->module, p_data->name, p_data->pixModule);
			return;
		}
	}
}

void myQLoader::load_module_ex(const QString& name, bool module, const QString& nameModule, const QPixmap& pix)
{
  if(!module)
	{
	  worker->clearArguments();
	   //add code run program
	   
	  QStringList str = QStringList::split(" ",name);
	  
	   worker->setArguments(str);
	   
	   if(!worker->start());
	   		 
	}
	else
	{
		QLibrary lib(name);
		//add code run module
		
		QSettings settings;
		
		QString ApplicationPath; // = qApp->applicationDirPath();
		setAplDir(ApplicationPath);
	  
		QDir d = QDir::home();
  
		QString s1 = d.absPath();
	 
		settings.removeSearchPath( QSettings::Unix, s1+"/.qt");
		settings.insertSearchPath( QSettings::Unix, s1+"/.SCT" );
   
		QString slang = settings.readEntry("/SCT/Language_UI","en"); 
		
		QTranslator myapp( 0 );
		myapp.load( nameModule + "_" + slang, ApplicationPath+"/lang");
		qApp->installTranslator( &myapp );
	  	typedef  void (*showW)(const QPixmap& );
		showW shw = (showW)lib.resolve( "run_module" );
                		if ( shw )
                                  shw(pix);
		else
		  ;//QMessageBox::about(this,tr("Error"),tr("Can't load module"));
				
    	        lib.unload();
		//qApp->removeTranslator(&myapp);
	}
}


/*!
    \fn myQLoader::addToList()
 */
void myQLoader::addToList(myInfo_modules data)
{
	myInfo_modules *p_data = new myInfo_modules;
	
	p_data->name = data.name;
	p_data->group = data.group;
	p_data->description = data.description;
	p_data->exec = data.exec;
	p_data->module = data.module;
	p_data->pixModule = data.pixModule;
	
	listModules.append(p_data);
}


/*!
    \fn myQLoader::getModule_Info(myInfo_modules& )
 */
bool myQLoader::getModule_Info(myInfo_modules& data)
{
  QLibrary module(data.exec);
  
  bool b=true;
  
  typedef QString (*group)();
  typedef QString (*name)();
  typedef QString (*description)();
  typedef QString (*pixmap)();
	  
  group g = (group)module.resolve( "group" );
  if( !g )
	b = false;
  name n = (name)module.resolve( "name");
  if( !n )
	b = false;
  description d = (description)module.resolve( "description" );
  if( !d )
	b = false;
  pixmap p = (pixmap)module.resolve( "pixmap");
  if( !p )
	b = false;
  if(!b)
  {
	  module.unload();
	  return b;
  }
  
  QString path_pix;
  setAplDir(path_pix);
  path_pix += "/pixmaps/"+p();
  
  QPixmap pix(path_pix);
   
  data.description = d();
  data.name = n();
  data.group = g();
  data.module = true;
  data.pixModule = pix;
  
  module.unload();
  
  return true;
}


/*!
    \fn myQLoader::load_Icons()
 */
void myQLoader::load_Icons()
{
  bool b = true;
  QString path; // = qApp->applicationDirPath();
  setAplDir(path);
  path += "/pixmaps/";
  //pixBoot.load(path+"boot.png");
  
  pixBootRedHat.load(path+"boot.png");
  if(pixBootRedHat.isNull())
	b = false;
  
  //pixmyMount.load(path+"my_hd.png");
  pixRedHatUserMount.load(path+"disks.png");
  if(pixRedHatUserMount.isNull())
	b = false;
  
   pixHwBrowser.load(path+"hwbrowser.png");
  if(pixHwBrowser.isNull())
	b = false;
  
  pixDisplay.load(path+"display.png");
  if(pixDisplay.isNull())
	b = false;
  
  pixSoundcard.load(path+"soundcard.png");
  if(pixSoundcard.isNull())
	b = false;
  
  pixKeyboard.load(path+"keyboard.png");
  if(pixKeyboard.isNull())
	b = false;
  
  pixPrinter.load(path+"printer.png");
  if(pixPrinter.isNull())
	b = false;
  
  pixMouse.load(path+"mouse.png");
  if(pixMouse.isNull())
	b = false;
  
  pixScaner.load(path+"scanner.png");
  if(pixScaner.isNull())
	b = false;
  
  pixFTP.load(path+"ftp.png");
  if(pixFTP.isNull())
	b = false;
  
  pixNet.load(path+"network.png");
  if(pixNet.isNull())
	b = false;
  
  pixMail.load(path+"mail-server.png");
  if(pixMail.isNull())
	b = false;
  
  pixHTTP.load(path+"http-server.png");
  if(pixHTTP.isNull())
	b = false;
   
  pixNFS.load(path+"nfs_server.png");
  if(pixNFS.isNull())
	b = false;
  
  pixSamba.load(path+"samba.png");
  if(pixSamba.isNull())
	b = false;
  
  pixDNS.load(path+"dns.png");
  if(pixDNS.isNull())
	b = false;
  
  pixDHCP.load(path+"dhcp.png");
  if(pixDHCP.isNull())
	b = false;
  
  pixProxy.load(path+"proxy.png");
  if(pixProxy.isNull())
	b = false;
  
  pixPackage.load(path+"packages.png");
  if(pixPackage.isNull())
	b = false;
  
  pixRpm_rem.load(path+"rpm_rem.png");
  if(pixRpm_rem.isNull())
    b = false;
  
  pixRpm_add.load(path+"rpm_add.png");
  if(pixRpm_add.isNull())
	b = false;
     
  pixDateTime.load(path+"date.png");
  if(pixDateTime.isNull())
	b = false;
  
  pixSysLog.load(path+"logviewer.png");
  if(pixSysLog.isNull())
	b = false;
  
  pixUserConfig.load(path+"users.png");
  if(pixUserConfig.isNull())
	b = false;
  
  pixServicess.load(path+"service.png");
  if(pixServicess.isNull())
	b = false;
  
  pixRoot.load(path+"root-password.png");
  if(pixRoot.isNull())
	b = false;
  
  pixSequrytiLevel.load(path+"password.png");
  if(pixSequrytiLevel.isNull())
	b = false;
  
 pixLang.load(path+"language.png");
 if(pixLang.isNull())
   b = false;
     
  return;// b;
}


/*!
    \fn myQLoader::get_module_info(const QString& )
 */
QString myQLoader::get_module_info(const QString& name)
{
  myInfo_modules *p_data=NULL;
  for ( p_data = listModules.first(); p_data; p_data = listModules.next() )
  {
    if(!name.compare(p_data->name))
    {
          return p_data->description;
    }
  }
  return QString("none");
}
