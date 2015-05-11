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
#include "editliloconf.h"

#include <qtextstream.h>
#include <qfile.h>
#include <qdir.h>
#include <qapplication.h>
#include <qmessagebox.h>

const char pathLilo[] = "/etc/lilo.conf";

EditLiloConf::EditLiloConf(QObject *parent, const char *name)
 : QObject(parent, name)
{
  lilo = new QProcess(this);
  parse_lilo_conf_file();
  
  default_OS = getDefaultOS();
}


EditLiloConf::~EditLiloConf()
{
  delete lilo;
}




/*!
    \fn EditLiloConf::TrimLeft(QString& str, QString )
 */
void EditLiloConf::TrimLeft(QString& str, QString pStr)
{
  while(!pStr.isEmpty())
  {
	while(str.left(1)==pStr.left(1))
	  str.remove(0,1);
	pStr.remove(0,1);
  }
}


/*!
    \fn EditLiloConf::Trim(QString& str, const char*)
 */
void EditLiloConf::Trim(QString& str, const char* pStr)
{
  TrimLeft(str,pStr);
  TrimRight(str,pStr);
}


/*!
    \fn EditLiloConf::TrimRight(QString& str, QString )
 */
void EditLiloConf::TrimRight(QString& str, QString pStr)
{
  while(!pStr.isEmpty())
  {
	while(str.right(1)==pStr.left(1))
	  str.remove(str.length()-1,1);
	pStr.remove(0,1);
  }
}


/*!
    \fn EditLiloConf::getBootMenu()
 */
QPtrList<myMenuLiloData> EditLiloConf::getBootMenu()
{
  return listOS;
}


/*!
    \fn EditLiloConf::getListThemes()
 */
QPtrList<theme_data> EditLiloConf::getListThemes()
{
  QString path;
   
  setAplPath(path);
  
 path +="/share/lilo/themes";
  
  QDir d(path);
  
  if(!d.exists()){
	QMessageBox::critical(0,tr("Error"),tr("Can't find themes directory"));
    //abort();
	return listThemes;
  }
  
  d.cd(path);
  
  d.setFilter(QDir::Dirs);
  
  //d.setNameFilter("*.dat");
  d.setSorting(QDir::Name);
  QString s;
  theme_data data;
  
  const QFileInfoList *list = d.entryInfoList();
  QFileInfoListIterator it( *list );
  QFileInfo *fi;

  while ( (fi = it.current()) != 0 ) {
	s = fi->fileName();
	if(s.find("."))
	{
	  data.name = fi->fileName();
	  data.path_data_file = fi->filePath()+"/"+fi->fileName()+".dat";
	  data.path_pixmap = fi->filePath()+"/"+fi->fileName()+".bmp";
	  
	  add_to_theme_list(data);
	}
	++it;
  }
  
  return listThemes;
}


/*!
    \fn EditLiloConf::getMainParameter(QString* dev_boot, int *i_pause, bool *b_disbTimer)
 */
void EditLiloConf::getMainParameter(QString* dev_boot, QString *i_pause)
{
  QFile fileLilo(path_my_lilo_conf);
  
  QString lines;
  int i;
  bool b=false;
	
  if ( fileLilo.open( IO_ReadOnly ) ) {
	QTextStream stream( &fileLilo );
	QString line;
	while ( !stream.atEnd() ) {
	  line = stream.readLine(); // line of text excluding '\n'
	  i = line.find("boot");
	  if(i!=-1){
		lines = line.mid(i+5,line.length()-(i+5));
		b=true;
		break;
	  }
	}
	if(!b)
	  lines = "none";
	fileLilo.close();
  }
	
  Trim(lines,"=");
  Trim(lines," ");
  Trim(lines,"\t");
  Trim(lines,"\"");
  
  *dev_boot = lines;
  
  *i_pause = getPause();
  
  QString def = getDefaultOS();
  
  getMenuData();
}


/*!
    \fn EditLiloConf::getCurrentTheme()
 */
QString EditLiloConf::getCurrentTheme()
{
  return getTheme();
}


/*!
    \fn EditLiloConf::addNewOS(myMenuLiloData )
 */
bool EditLiloConf::addNewOS(myMenuLiloData data)
{
  myMenuLiloData *p_data=NULL;
  for ( p_data = listOS.first(); p_data; p_data = listOS.next() )
  {
	if(!p_data->label.compare(data.label))
	  return false;
  }
  
  if(data.main)
	  default_OS = data.label;
  add_to_list(data);
  
  return true;
}


/*!
    \fn EditLiloConf::editExistOS(QString )
 */
bool EditLiloConf::editExistOS(myMenuLiloData data, QString old_name)
{
  bool b_findet = false;
  myMenuLiloData *p_data=NULL;
  
  if(data.main)
	for ( p_data = listOS.first(); p_data; p_data = listOS.next() )
    {
	  p_data->main = false; //only one os be default
    }
  
  for ( p_data = listOS.first(); p_data; p_data = listOS.next() )
  {
	if(!p_data->label.compare(old_name))
	{
	  p_data->label = data.label;
	  p_data->Root = data.Root;
	  p_data->append = data.append;
	  p_data->image = data.image;
	  p_data->initrd = data.initrd;
	  p_data->mode = data.mode;
	  p_data->main = data.main;
	  //p_data->type_os = data.type_os;
	  
	  b_findet = true;
	  
	  break;
	}
  }
  
  return b_findet;
}


/*!
    \fn EditLiloConf::removeOS(QString )
 */
bool EditLiloConf::removeOS(QString name)
{
  bool b_findet = false;
  myMenuLiloData *p_data=NULL;
  for ( p_data = listOS.first(); p_data; p_data = listOS.next() )
  {
	if(!p_data->label.compare(name))
	{
	  listOS.remove(p_data);
	  b_findet = true;
	  
	  break;
	}
  }
  
  return b_findet;
}


/*!
    \fn EditLiloConf::add_to_list(myMenuLiloData )
 */
void EditLiloConf::add_to_list(myMenuLiloData data)
{
  myMenuLiloData* data1 = new myMenuLiloData;
  
  data1->label = data.label;
  data1->Root = data.Root;
  data1->append = data.append;
  data1->image = data.image;
  data1->initrd = data.initrd;
  data1->mode = data.mode;
  if(!data.label.compare(default_OS))
	  data1->main = true;
  else
	  data1->main = false;
  data1->type_os = data.type_os;
  
  listOS.append(data1);
}


/*!
    \fn EditLiloConf::setParameters(QString dev_boot, int i_pase, QString name_theme)
 */
void EditLiloConf::setParameters(QString dev_boot, QString i_pase, QString name_theme)
{
  QString GlobStr, s;
  
  emit sendOut(tr(" + Create <b><i>lilo.conf</i></b> file"), 0);
  
	GlobStr="#Thih file generated by SCT lilo \n";
  GlobStr+="boot="+dev_boot+"\n";
  GlobStr+="map=/boot/map\n";
  GlobStr+="install=/boot/boot-bmp.b\n";
  
  emit sendOut(tr(" + Set theme <b><i>%1</i></b>").arg(name_theme), 0); 
   
  bool b=false;
  
  if(!i_pase.compare("node")) //chek disable timer
	  b = true;
  
  setTheme(GlobStr, name_theme,b);
  
  GlobStr+="prompt\n";
  GlobStr+="nowarn\n";
  if(!b)
	GlobStr+="timeout="+i_pase+"0\n";
  GlobStr+="lba32\n";
  GlobStr+="menu-scheme=wb:bw:wb:bw\n"; 
  QString Menustr;
  
  emit sendOut(tr("<i></i> + Create boot menu"), 0);
  myMenuLiloData *p_data=NULL;
  for ( p_data = listOS.first(); p_data; p_data = listOS.next() )
  {
	if(p_data->main)
	  GlobStr += "default=\""+p_data->label+"\"\n";
	
	if(!p_data->type_os.compare("linux family"))
	{
	  Menustr += "image="+p_data->image+"\n";
	  Menustr += "\tlabel=\""+p_data->label+"\"\n";
	  Menustr += "\troot="+p_data->Root+"\n";
	  Menustr += "\tinitrd="+p_data->initrd+"\n";
	  if(!p_data->append.isEmpty())
	    Menustr += "\tappend=\""+p_data->append+"\"\n";
	  Menustr += "\tvga="+p_data->mode+"\n";
	  Menustr += "read-only\n";
	}
	else
	{
	  Menustr += "other="+p_data->Root+"\n";
	  Menustr += "\tlabel=\""+p_data->label+"\"\n";
	}
  }
  
  GlobStr += Menustr;
      
  emit sendOut(tr("<i></i> + Write data to <b><i>lilo.conf</i></b> file"), 0);
  
  QFile file(path_my_lilo_conf );
  if ( file.open( IO_WriteOnly ) ) {
	QTextStream stream( &file );
	stream << GlobStr;
	file.close();
  }
  run_lilo_command();
  
  emit sendOut(tr("<i></i>-- Success --"), 0);
   return;
}


/*!
    \fn EditLiloConf::getPause(QString main_str)
 */
QString EditLiloConf::getPause()
{
  QFile fileLilo(path_my_lilo_conf);
  
  QString lines;
  int i;
  bool b=false;
	
  if ( fileLilo.open( IO_ReadOnly ) ) {
	QTextStream stream( &fileLilo );
	QString line;
	while ( !stream.atEnd() ) {
	  line = stream.readLine(); // line of text excluding '\n'
	  i = line.find("timeout");
	  if(i!=-1){
		lines = line.mid(i+8,line.length()-(i+8));
		lines.remove(lines.length()-1,1);
		b=true;
		break;
	  }
	}
	fileLilo.close();
	if(!b)
	  lines = "";
  }
  Trim(lines,"=");
  Trim(lines," ");
  Trim(lines,"\t");
  Trim(lines,"\"");
  return lines;	
}


/*!
    \fn EditLiloConf::getTheme(QString main_str)
 */
QString EditLiloConf::getTheme()
{
  QFile fileLilo(path_my_lilo_conf);
     
  QString lines;
  int i, j;
  bool b=false;
	
  if ( fileLilo.open( IO_ReadOnly ) ) {
	QTextStream stream( &fileLilo );
	QString line;
	while ( !stream.atEnd() ) {
	  line = stream.readLine(); // line of text excluding '\n'
	  i = line.find("#< myTheme --");
	  if(i!=-1){
		i = line.find("--");
		j = line.find("!");
		lines = line.mid(i+1,j-i+1);
                 
		Trim(lines,"!- !");
		b=true;
		break;                 
	  }
	}
	fileLilo.close();
	if(!b)
	  lines = "";
  }
  return lines;	
}


/*!
    \fn EditLiloConf::getMenuData(QString defaultOS)
 */
void EditLiloConf::getMenuData()
{
  QFile fileLilo(path_my_lilo_conf);
  
  myMenuLiloData data;
  QString tmp, s;
  int i=0;
  QStringList str;
  QStringList Resstr;
  
  data.main = false;
  
  bool finish=false;
  
  //bool b_default = false; //cheker only one os be default
  
  if ( fileLilo.open( IO_ReadOnly ) ) {
	QTextStream stream( &fileLilo );
	QString line;
	
	while ( (line = stream.readLine())!= NULL/*!stream.atEnd()*/ ) {
		str += line;
	}
	  
		  
	if(str.isEmpty())
		return;               
         
	
	for ( QStringList::Iterator it = str.begin(); it!=str.end(),!finish; ++it ) {
         
		if((it==str.end())||(it==NULL))
			finish=true;//checked end of String List
              
		if(finish)
			break;	
		s = *it;
                        
		i = s.find("image");
		data.append="";
		data.mode="";
		data.Root="";
		
		if(i!=-1) //обробка параметрів лінукса
		{
                
			tmp = s.mid(i+6,s.length()-(i+6));
			Trim(tmp,"=");
			Trim(tmp," ");
			Trim(tmp,"\t");
			Trim(tmp,"\"");
			data.image=tmp;
			data.type_os = tr("linux family");
	
			++it;
	
			while(1)
			{
				s=*it;
          /*if(s.isEmpty())
				{
				++it;
				continue;              
			}*/              
				i = s.find("image");
				
				if(i!=-1)
				{
					add_to_list(data);
					--it;
					break;
				}
				
				i = s.find("other");
	
				if(i!=-1)
				{
					add_to_list(data);
	  //--it;
					break;
				}
				
				if((it==str.end())||(it==NULL))
				{
					add_to_list(data);
					finish=true;//added 21:05 28.11 2005
					break;
				}
				
				i = s.find("label");
				
				if(i!=-1)
				{
					tmp = s.mid(i+6,s.length()-(i+6));
					Trim(tmp,"=");
					Trim(tmp," ");
					Trim(tmp,"\t");
					Trim(tmp,"\"");
					
					data.label=tmp;
					
		//if(tmp==myNameDefault)
		 //  tmp+="*";
					Resstr+=tmp;
					++it;
					continue;
				}
                    
				i = s.find("append");
                    
				if(i!=-1)
				{
					tmp = s.mid(i+7,s.length()-(i+7));
					Trim(tmp,"=");
					Trim(tmp," ");
					Trim(tmp,"\t");
					Trim(tmp,"\"");
					data.append=tmp;
					++it;
					continue;
				}
        
				i = s.find("root");
				
				if(i!=-1)
				{
					tmp = s.mid(i+5,s.length()-(i+5));
					Trim(tmp,"=");
					Trim(tmp," ");
					Trim(tmp,"\t");
					Trim(tmp,"\"");
					data.Root=tmp;
					++it;
					continue;
				}
	
				i = s.find("initrd");
		
				if(i!=-1)
				{
					tmp = s.mid(i+7,s.length()-(i+7));
					Trim(tmp,"=");
					Trim(tmp," ");
					Trim(tmp,"\t");
					Trim(tmp,"\"");
					data.initrd=tmp;
					++it;
					continue;
				}
				
				i = s.find("vga");
	
				if(i!=-1)
				{
					tmp = s.mid(i+4,s.length()-(i+4));
					Trim(tmp,"=");
					Trim(tmp," ");
					Trim(tmp,"\t");
					Trim(tmp,"\"");
					data.mode=tmp;
					++it;
					continue;
				}
				
				++it;
			}
		}
		
		i = s.find("other");  //обропка параметрів віндовс
		
		if(i!=-1)
		{
			tmp = s.mid(i+6,s.length()-(i+6));
			Trim(tmp,"=");
			Trim(tmp," ");
			Trim(tmp,"\t");
			Trim(tmp,"\"");
			data.Root=tmp;
			data.type_os = tr("windows family");
	
			++it;
	                                          
			while(1)
			{
				s=*it;
				if(s.find("#")==-1)
					if(s.find("label")!=-1)
						break;
				++it;
			}
			
			i = s.find("label");
			if(i!=-1)
			{
				tmp = s.mid(i+6,s.length()-(i+6));
				Trim(tmp,"=");
				Trim(tmp," ");
				Trim(tmp,"\t");
				Trim(tmp,"\"");
           //if(tmp==myNameDefault)
           //  tmp+="*";
           //Resstr+=tmp;
           //Trim(tmp,"*");
				data.label=tmp;
				data.append="";
				data.image="";
				data.initrd="";
				data.mode="";
                   
		      
				
			}
			else
			{
				data.label="Other";
				Resstr+="Other";
			}
			add_to_list(data);
                  //++it;
		}
	}
  } 
  fileLilo.close();
}

/*!
    \fn EditLiloConf::parse_lilo_conf_file()
 */
void EditLiloConf::parse_lilo_conf_file()
{
  set_path_my_lilo_conf();//creates original file lilo.conf in directory $HOME/.myConfig
  QFile file;
  if(!file.exists(path_my_lilo_conf))
  {
	file.setName(pathLilo);
	QString glob = "";
	if (file.open( IO_ReadOnly ) ) {
	  QTextStream stream( &file );
	  QString line;
	while ( !stream.atEnd() ) {
		line = stream.readLine(); 
		glob += line+"\n";
	  }
	  file.close();
	}
	else
	{
	  QMessageBox::critical(0, tr("Error read"), tr("File <lilo.conf> is not exist.\nOr permission denied."));
	  
	  return;
	}
		
	file.setName(path_my_lilo_conf);
			
	if(file.open(IO_WriteOnly))
	{
	  if(glob.isEmpty())
	  {
		file.close();
		return;
	  }
	  
	  QTextStream stream( &file );
	  stream<<glob;
	  file.close();
	}
	else
	{
	  QMessageBox::critical(0, tr("Error create"), tr("File <lilo.conf> is not created.\nMaybe permission denied."));
	}
  }
}


/*!
    \fn EditLiloConf::setTheme(QString& str, QString name_theme)
 */
void EditLiloConf::setTheme(QString& str, QString name_theme, bool b_disable_timer)
{
  QString dat_file="";
  QString theme_file="";
   
  theme_data *p_data=NULL;
  for ( p_data = listThemes.first(); p_data; p_data = listThemes.next() )
  {
	if(!p_data->name.compare(name_theme))
	{
	  dat_file = p_data->path_data_file;
	  theme_file = p_data->path_pixmap;
	  break;
	}
  }
   
  QString lines;
  lines="#< myTheme -- ";
  lines+=name_theme+"! This comentar generated for SCT lilo, don't change or remove him\n";
  //lines+="bitmap="
  QFile file(dat_file);
  if ( file.open( IO_ReadOnly ) ) {
	QTextStream stream( &file );
	QString line;
	while ( !stream.atEnd() ) {
	  line = stream.readLine();
	  if(b_disable_timer)//disables timer
		  if(line.find("bmp-timer")!=-1)
			  line.insert(0,"#");
	  lines += line+"\n";;
	}
	file.close();
  }
  
  copyFile(theme_file);
  
  str+=lines;
}


/*!
    \fn EditLiloConf::getDefaultOS()
 */
QString EditLiloConf::getDefaultOS()
{
  QFile fileLilo(path_my_lilo_conf);
               
  int i;
  
  QString myNameDefault;
	
  if ( fileLilo.open( IO_ReadOnly ) ) {
	QTextStream stream( &fileLilo );
	QString line;
	while ( !stream.atEnd() ) {
	  line = stream.readLine(); // line of text excluding '\n'
	  i = line.find("default");
      if(i!=-1)
	  {
		  myNameDefault = line.mid(i+8,line.length()-(i+8)); //зчитування ос за замовчуванням
		  Trim(myNameDefault,"=");
		  Trim(myNameDefault," ");
		  Trim(myNameDefault,"\t");
		  Trim(myNameDefault,"\"");
		  break;
	  }
	}
	
  }
  
  return myNameDefault;	
}


/*!
    \fn EditLiloConf::copyFile(const char* cName, const char* Name)
 */
void EditLiloConf::copyFile(const char* cName)
{
  QString s(cName);
  
  QProcess proc(this);
  proc.addArgument("cp");
  
  proc.addArgument(s);
  //s=Name;
  
  s="/boot";
  
  proc.addArgument(s);
  proc.start();
}


/*!
    \fn EditLiloConf::run_lilo_command()
 */
void EditLiloConf::run_lilo_command()
{
  lilo->clearArguments();
  
  lilo->addArgument("/sbin/lilo");
  lilo->addArgument("-C");
  lilo->addArgument(path_my_lilo_conf);
  
 if ( !lilo->start() ) {
	  emit sendOut(tr("\tCan't start lilo command. Check lilo package"),-1);
	return;
  }
  
  emit sendOut(tr("<i></i> + run: <b><i>/sbin/lilo -C %1</b></i>").arg(path_my_lilo_conf), 0);
  
  while(lilo->isRunning())
  {
    QString s = lilo->readLineStdout();
    if(!s.isEmpty())
       emit sendOut(QString("\t- %1").arg(s), 1);
    s = lilo->readLineStderr();
    if(!s.isEmpty())
       emit sendOut(QString("\t- %1").arg(s), -1);
  }
  
  //remove temp file
  
  emit sendOut(tr("<i></i> + Removes temporary files"), 0);
  
  QString s("/boot/");
  s+=getCurrentTheme()+".bmp";
  QFile f;
  f.remove(s);
  return;
}


/*!
    \fn EditLiloConf::set_path_my_lilo_conf()
 */
void EditLiloConf::set_path_my_lilo_conf()
{
  path_my_lilo_conf = QDir::homeDirPath();
  
  path_my_lilo_conf += "/.SCT/lilo.conf";
}

/*!
    \fn EditLiloConf::setAplPath(QString& path)
 */
void EditLiloConf::setAplPath(QString& path)
{
  path = qApp->applicationDirPath();
  
  int i = path.findRev("/");
  
  path.remove(i,20);
  
  //appDir = s;
}


/*!
    \fn EditLiloConf::add_to_theme_list(theme_data )
 */
void EditLiloConf::add_to_theme_list(theme_data data)
{
  theme_data* pNone = new theme_data; 
 	
  pNone->name = data.name;
  pNone->path_data_file = data.path_data_file;
  pNone->path_pixmap = data.path_pixmap;
  
  listThemes.append(pNone);
}

