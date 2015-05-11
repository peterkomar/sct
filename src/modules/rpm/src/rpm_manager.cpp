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
 
#include "rpm_manager.h"
#include "parcersource.h"

#include <qmessagebox.h>
#include <qtextcodec.h>


rpm_manager::rpm_manager(QObject *parent, const char *name)
 : QObject(parent, name)
 ,path_src("")
 ,b_termit(false)
{
  ::rpmReadConfigFiles(NULL,NULL);
	
	name_src = qApp->applicationDirPath();
	setAplDir(name_src);
	name_src += "share/rpm/heder.xml";
	
	i_scaler = 0;
	p_rpm = new QProcess(this);
	
	listRpm.setAutoDelete(TRUE);
	listUnRpm.setAutoDelete(TRUE);
	tmp_list.setAutoDelete(TRUE);
	list_rpm.setAutoDelete(TRUE);
}


rpm_manager::~rpm_manager()
{
}




/*!
    \fn rpm_manager::get_info_rpm(Header h)
 */
infoRpm rpm_manager::get_info_rpm(Header h)
{
	int_32 type,count;
	char *k;
	infoRpm data;
	
	QString s;
	int n;
                                                         
       ::headerGetEntry(h, RPMTAG_NAME, &type, (void **) &k, &count); //return name rpm           
	if(k)                                                          
		data.name = QString::fromLocal8Bit(k); 
  
        ::headerGetEntry(h, RPMTAG_GROUP, &type, (void **) &k, &count); //return group rpm
	if(k)                                                          
		data.group = tr(QString::fromLocal8Bit(k));
  
        ::headerGetEntry(h, RPMTAG_VERSION, &type, (void **) &k, &count); //return version rpm
	if(k)                                                          
		data.version = QString::fromLocal8Bit(k);

        ::headerGetEntry(h, RPMTAG_RELEASE, &type, (void **) &k, &count); //return release rpm
	if(k)                                                          
		data.release = QString::fromLocal8Bit(k);
        ::headerGetEntry(h, RPMTAG_SUMMARY, &type, (void **) &k, &count); //return SUMMAry rpm
	if(k)                                                          
		data.summary = QString::fromLocal8Bit(k);
        ::headerGetEntry(h,   RPMTAG_ARCH, &type, (void **) &k, &count); //return arhitecture rpm
	if(k)                                                          
		data.arch = QString::fromLocal8Bit(k);     
  
	const char *error;
	//:depflags
	char *str;
	str = ::headerSprintf(h, "[%{REQUIRENAME} (%{REQUIREFLAGS:depflags}"
			" %{REQUIREVERSION}), ]",
			rpmTagTable, rpmHeaderFormats, &error);
			
	if (str) {
		QString req = str;
		req.remove(req.length() - 2, 2);
		req.replace("( )","");
		data.deppend =  req; //depends  rpm package
	}
  
        ::headerGetEntry(h, RPMTAG_DESCRIPTION, &type, (void **) &k, &count);
	if(k) {
		data.Description=QString::fromLocal8Bit(k); //�� rpm
	}
  
	format_descr(data.Description);
	
  //return list files
  
	QStringList files;
	int i;
	char **fileList;
	
	if(::rpmHeaderGetEntry(h, RPMTAG_OLDFILENAMES, &type, (void **) &fileList, &count))
	{
		for(i=0; i<count; i++)
		{
			files.append(fileList[i]);
		}
                ::free(fileList);
	}
	else
	{
		char **dir_names, **base_names;
		
		int *index;
				
		if(::headerGetEntry(h,RPMTAG_BASENAMES,&type,(void **)&base_names,&count))
		{
			n = count;
				
                    ::headerGetEntry(h,RPMTAG_DIRNAMES,&type,(void **)&dir_names,&count);
			
                    ::headerGetEntry(h,RPMTAG_DIRINDEXES,&type,(void **)&index,&count);
			
				
			for(i=0; i<n; i++)
			{
				s = dir_names[index[i]];
				s += base_names[i];
				//QMessageBox::about(0,"df",s);
				files.append(s);
			}
                    ::free(dir_names);
                    ::free(base_names);
		}
	}
	data.files = files;
	
	files.clear();
	
	if(::rpmHeaderGetEntry(h, RPMTAG_PROVIDENAME, &type, (void **) &fileList, &count))
	{
		for(i=0; i<count; i++)
		{
			files.append(fileList[i]);
		}
            ::free(fileList);
	}
	data.names_files = files;
	
        ::headerGetEntry(h, RPMTAG_SIZE, &type, (void **) &k, &count);
	if(k) {
    
		int size = *(int_32 *)k;
		data.isize = size;
    
		s = convert_size(size); //convert size to byte kilobyte megabyte gigabyte
		data.size = s;
	}
	qApp->processEvents();
	char **changeLog, **logAu;
	QStringList Log;
	int *times;
	QDateTime dt;
		
	if(::headerGetEntry(h,RPMTAG_CHANGELOGTEXT,&type,(void **)&changeLog,&count))
	{
		n = count;
				
                ::headerGetEntry(h,RPMTAG_CHANGELOGNAME,&type,(void **)&logAu,&count);
			
		if(n<count)
			count = n;
		else
			n = count;
			
                        ::headerGetEntry(h,RPMTAG_CHANGELOGTIME,&type,(void **)&times,&count);
			
			if(n<count)
				count = n;
			else
				n = count;
			
			///optimize for speed if you want see full change log, remove this code: 1. 2.
			///(I find problem worken with QTextEdit. I can't unleash this problem :-(. Maybe you can? )
			/// 1.
			bool bn=false;
			if(n > 400)
			{
				n = 400;
				bn = true;
			}
			///
				
			for(i=0; i < n; i++)
			{
				dt.setTime_t(times[i]);
				s = " + "+dt.toString() +logAu[i];
				Log.append("<br><font color=\"#241cb0\"> "+s+"</font>");
				s = changeLog[i];
				s.replace("<","&lt;");
		                s.replace(">","&gt;");
				s.replace("\n","<br>");
				Log.append(s);
			}
			///2.	
			if(bn)
				Log.append("<br><center><b>...</b></center>");
			///	
                        ::free(logAu);
                        ::free(changeLog);
				
		data.ChangeLog = Log;
}
   return data;               
}


/*!
    \fn rpm_manager::get_i_packages()
 */
void rpm_manager::get_i_packages()
{
	listRpm.clear();
	infoRpm data;
        ::rpmdb db;
        ::Header h;
	QString prefix("");
    
	if ( ::rpmdbOpen(const_cast<char*>(prefix.ascii()), &db, O_RDONLY, 0644) )
		return;
  
        ::rpmdbMatchIterator mi;
	
	/* Count all packages */
	
	mi = ::rpmdbInitIterator(db, (rpmTag)RPMDBI_PACKAGES, NULL, 0);
	int i_c=0;
	int n_count=0;
	
	char *group;
	
	while((h = ::rpmdbNextIterator(mi)) != NULL) {
		if (!::headerGetEntry(h, RPMTAG_GROUP, NULL, (void **) &group, NULL))
		{
			qFatal(tr("ERROR:could not read database record\n"));
			return;
		}
     	 ++n_count;
	}
        ::rpmdbFreeIterator(mi);
	
	/* get info packages*/
	mi = ::rpmdbInitIterator(db, (rpmTag)RPMDBI_PACKAGES, NULL, 0);
		
	
	while((h = ::rpmdbNextIterator(mi)) != NULL) {
		if (!::headerGetEntry(h, RPMTAG_GROUP, NULL, (void **) &group, NULL))
		{
			qFatal(tr("ERROR:could not read database record\n"));
			return;
		}
		data = get_info_rpm(h);
		++i_c;
		
		emit send_parcent(i_c, n_count);
		qApp->processEvents();
		add_to_rem_rpm_list( data );
		qApp->processEvents();
    
	}
        ::rpmdbFreeIterator(mi);
        ::rpmdbClose(db);  
}


/*!
    \fn rpm_manager::get_u_packages()
 */
void rpm_manager::get_u_packages()
{
	listUnRpm.clear();
	
	QFile file(name_src); 
	if(!file.exists())
		return;
	
		
	QXmlSimpleReader reader; 
  
	ParcerSource handler(&listUnRpm); 
	reader.setContentHandler(&handler); 
	reader.setErrorHandler(&handler); 
	reader.parse(&file); 
	
	//listUnRpm = handler.getList();
}

/*!
    \fn rpm_manager::getListRpm(bool b = true)
 */
QPtrList<infoRpm> rpm_manager::getListRpm()
{
	return listRpm;
}


/*!
    \fn rpm_manager::getListURpm()
 */
QPtrList<infoRpm> rpm_manager::getListURpm(bool full)
{
	if(full)
		return listUnRpm;
        		
		
	infoRpm* data_u = (infoRpm*)listUnRpm.first();
	
	QPtrList<infoRpm> list;
	
	while(data_u != NULL)
	{
          infoRpm* data_i = (infoRpm*)listRpm.first();
          
          bool b_find = false;
          
          while(data_i != NULL)
          {
            if(!data_u->b_on)
            {
             b_find = true;
             break; 
            }
            
            if(data_u->name == data_i->name)
            {
              QString t1 = data_u->version;
              QString t2 = data_i->version;
              t1 = t1.remove(".");
              t2 = t2.remove(".");
              
              if( t1.toInt() > t2.toInt())
              {
                b_find = false;
                break;
              }
              else
              {
                if(t1.toInt() == t2.toInt())
                {
                  t1 = data_u->release;
                  t2 = data_i->release;
                  
                  if( t1 > t2 )
                  {
                    b_find = false;
                    break;
                  }
                }
                b_find = true;
                break;
              }
            }
            
            data_i = (infoRpm*)listRpm.next();
          }
          
          if(!b_find)
	     list.append(data_u);
		
	   data_u = (infoRpm*)listUnRpm.next();
	}
	
	return list;
}

/*!
    \fn rpm_manager::write_media_data()
 */
void rpm_manager::write_media_data()
{
	if(!b_termit)
	{
			infoRpm *data = NULL;
			while((data = (infoRpm*)tmp_list.first())!=NULL)
			{
				add_to_add_rpm_list(*data);
				tmp_list.removeFirst();
			}
	}
	
	tmp_list.clear();
	
	ParcerSource parcer(&listUnRpm/*,name_src*/);
	
	parcer.write_source(name_src);
}


/*!
    \fn rpm_manager::remove_autors(QString )
 */
void rpm_manager::format_descr(QString& s)
{
	s.replace("&","and");
	s.replace("<","&lt;");
	s.replace(">","&gt;");
}


/*!
    \fn rpm_manager::add_rpm_media(const QString& name, const QString& path, bool brem)
 */
int rpm_manager::add_rpm_media(const QString& name, const QString& path_rpm, bool brem, bool b_on)
{
	
	if(b_termit)
		return -1;
	
	
	infoRpm data;
	Header h;
	int  rc=0;
	//rpmts t=NULL;
	//t = rpmtsCreate();
	FD_t fd;
	b_termit = false;
	//int n_count;
	
	rpmts t = ::rpmtsCreate();
	unsigned vsflag = RPMVSF_DEFAULT;
	//if ( verification_r & NODIGEST )
		vsflag |= _RPMVSF_NODIGESTS;
	//if ( verification_r & NOSIGNATURE )
	//	vsflag |= _RPMVSF_NOSIGNATURES;
	
	::rpmtsSetVSFlags( t, rpmVSFlags(vsflag) );

			
	QString packagePattern="*.rpm;*.RPM;*.Rpm;*.rPm;*.rpM;*.RPm;*.rPM;*,RpM";
				
	QDir dir_rpm(path_rpm,packagePattern);
	
	dir_rpm.setMatchAllDirs( TRUE );        // list contains subdirs
		
	
	if (dir_rpm.exists()) {
		if ( dir_rpm.isReadable() ) {
			//n_count = d_files.count();
			//if(n_count>0)
			 // emit send_total_add(n_count);
			QString pn;
			const QFileInfoList *list1 = dir_rpm.entryInfoList();
						
			if(list1 == NULL)
				return -2;
			
			QFileInfoListIterator it( *list1 );      
			QFileInfo *fi;                          
			while ( (fi=it.current()) && (!b_termit)) {
				if ( fi->isDir() ) {
	  
					if ( fi->fileName() != QString::fromLatin1(".") &&
										fi->fileName() != QString::fromLatin1("..") )
					{
	      
	      
						add_rpm_media(name, path_rpm + "/" + fi->fileName(), brem, b_on );
					} else {
	      
	      
						;
					}
				} else {
	  
					if((fd = ::Fopen(QFile::encodeName(fi->absFilePath()), "r.fdio")) < 0) {
							::Fclose(fd);   
							emit send_cur_add_str((fi->fileName()),3);
							++i_scaler;
							emit send_cur_add(i_scaler);
							++it;
								
							continue; //can't open file
					}
  
					if(fd!=0)     {
						rc = ::rpmReadPackageFile(t,fd, QFile::encodeName(fi->absFilePath()),&h);
      
							::Fclose(fd);
							qApp->processEvents();
							
							if(h == NULL)
							{
								emit send_cur_add_str((fi->fileName()),2);//can't read header package
								++i_scaler;
								emit send_cur_add(i_scaler);
								++it;
								qApp->processEvents();
								continue;
							}
					}
    
					if(h != NULL){
						data = get_info_rpm(h);
			
						data.nameSource = name;
						data.pathSource = path_src;
						data.pathRPM = QFile::encodeName(fi->absFilePath());//path_rpm
						data.cd = brem;
                                                data.b_on = b_on;
			
						add_to_tmp_rpm_list(data);
						++i_scaler;
						
						emit send_cur_add_str(fi->fileName(),1);
						emit send_cur_add(i_scaler);
						qApp->processEvents();
					}
			}
				++it;                               // goto next list element
			}
		} else {
			qWarning("Directory is not found!");
	}
	
			::rpmtsFree(t);
  }
  if(b_termit)
  		return -1;
	  
	return 0;
}


/*!
    \fn rpm_manager::rem_rpm_media(const QString& name)
 */
void rpm_manager::rem_rpm_media(const QString& name)
{
	infoRpm *data = (infoRpm*)listUnRpm.first();
	
	while(data != NULL)
	{
		if(data->nameSource == name)
		{
			listUnRpm.remove(data);
			
			data = (infoRpm*)listUnRpm.current();
			continue;
		}
		
		data = (infoRpm*)listUnRpm.next();
	}
}



/*!
    \fn rpm_manager::edit_rpm_media(const QString& nameMedia, bool removable, QString oldName = nameMedia)
 */
void rpm_manager::edit_rpm_media(const QString& nameMedia, bool removable, bool b_on, const QString& oldName)
{
	infoRpm *data = (infoRpm*)listUnRpm.first();
	int n = listUnRpm.count();
	int i=0;
	
	emit send_total_add(n);
	
	while(data != NULL)
	{
		if(oldName == "none")
		{
			if(data->nameSource == nameMedia)
                        {
				data->cd = removable;
                                data->b_on = b_on;
                        }
		}
		else
		{
			if(data->nameSource == oldName)
			{
				data->nameSource = nameMedia;
			        data->cd = removable;
                                data->b_on = b_on;
			}
		}		
		i++;
		emit send_cur_add(i);
		data = (infoRpm*)listUnRpm.next();
	} 
}


/*!
    \fn rpm_manager::find_item(const QString& ,bool installed = true)
 */
bool rpm_manager::find_item(const QString& s,bool installed)
{
	infoRpm* data=NULL;
	
	if(installed)
		data = (infoRpm*)listRpm.first();
	else
		data = (infoRpm*)listUnRpm.first();
	
	while(data != NULL)
	{
		if(data->name == s)
			return true;
		
		if(installed)
			data = (infoRpm*)listRpm.next();
		else
			data = (infoRpm*)listUnRpm.next();
	}
	
	return false;
}


/*!
    \fn rpm_manager::setNameSrc(const QString& )
 */
void rpm_manager::set_path_src(const QString& s)
{
	path_src = s;
}


/*!
    \fn rpm_manager::unleash_depends(infoRpm )
 */
bool rpm_manager::unleash_depends(add_rem_data *data1, QString& s)
{
	QStringList list = QStringList::split(",", data1->deppend);
	
	QStringList list_prov;
	bool b_ret=true, b;
	QString t,s_t;
	QFile file;
	infoRpm* data=NULL;
	
	QStringList::Iterator it = list.begin();
	QStringList::Iterator it1;
	
	while(it != list.end())
	{
		s_t = *it;
		s_t = s_t.stripWhiteSpace();
		
		if(s_t.find("rpmlib")!=-1)
		{
			it = list.remove(it);
			continue;
		}
				
				
		b = false;
		data = (infoRpm*)listRpm.first();
			//find in installed packages
		 
		while(data != NULL)
		{
			it1 = data->names_files.begin();
		
			while(it1 != data->names_files.end())
			{
				if(s_t == (*it1).stripWhiteSpace())
				{
					b = true;
					it = list.remove(it);
					break;
				}
				it1++;
			}
						
			if(b)
				break;
			
			//find in (installed) packages (and check version)
                        
			if(find_in_package_version(s_t, data))
			{
				b=true;
				it = list.remove(it);
			}
			
			if(b)
				break;	
			
			data = (infoRpm*)listRpm.next();
		}
			
		if(b)
			continue;
		
		//find in files and in packages and check version
		
		file.setName(s_t);
		
		//find in files
		if(file.exists())
		{
			it = list.remove(it);
			continue;
		}
		
	//find in not installed packages
		
		b = false;
                
                QPtrList<infoRpm> list_src = getListURpm(false);
                
                data = (infoRpm*)list_src.first();
		 
		while(data != NULL)
		{
			it1 = data->names_files.begin();
		
			while(it1 != data->names_files.end())//find in names files
			{
				t = (*it1).stripWhiteSpace();
				if(s_t == t)
				{
					b = true;
					it = list.remove(it);
					
					b_ret = add_to_rpm_list(*data,true);
					b_ret = !b_ret;
					  
					break;
				}
					it1++;
			}
						
			if(b)
				break;
				
			 //find in packages (and check version)
		  if(find_in_package_version(s_t, data))
		  {
			  b=true;
			  it = list.remove(it);
			
                          b_ret = add_to_rpm_list(*data,true);
			  b_ret = !b_ret;
				  //badd=true;
		  }
			
		  if(b)
			 break;
						
                  data = (infoRpm*)list_src.next();
	        }
	
	if(b)
		continue;
			
		it++;
 }
		
 QString font = "<font color=\"#55aa00\"><b>%1</b></font>";
 QString font2 = "<font color=\"blue\"><b>%1</b></font>";
 
	if(!list.isEmpty())
	{
		//b_ret = false;
			for(QStringList::Iterator it = list.begin(); it != list.end(); ++it)
			{
				t = *it;
				t.replace("<","&lt;");
				t.replace(">","&gt;");
				s += QString("<font color=\"red\">%1</font> %2<br>").arg(t).arg(tr("not found"));
				//s += QString("<font color=\"blue\">%1</font>").arg(tr(" is needed by "))
				//		+ QString("<font color=\"black\">%1</font><br>").arg(data1->name);
			}
			QString s1;      
				s1+="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font2.arg(data1->name+data1->version)+"</td>";
				s1+="<td bgcolor=\"#EEF6FF\" width=95%>"+font.arg(s)+"</td></tr>";
				
				s = s1;
	}
	else
	{
		s+="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font2.arg(data1->name+data1->version)+"</td>";
		s+="<td bgcolor=\"#EEF6FF\" width=95%><font color=\"green\"><b>Succes</b></font></td></tr>";
						
		//b_ret = true;
	}
			
	return b_ret;	
}


/*!
    \fn rpm_manager::check_preparing_remove(infoRpm )
 */
bool rpm_manager::check_preparing_remove(add_rem_data *data1, QString& s)
{
	
	QStringList list = data1->names_files;
	
	QString font = "<font color=\"#55aa00\"><b>%1</b></font>";
	QString font2 = "<font color=\"blue\"><b>%1</b></font>";
	
	QStringList list_dep;
	bool b_ret=false, b;
	QString t,s_t,s1;
	
	infoRpm* data=NULL;
	
	QStringList::Iterator it = list.begin();
	QStringList::Iterator it1;
	
	while(it != list.end())
	{
		s_t = *it;
		s_t = s_t.stripWhiteSpace();
		
		//display_list(list);
		
		b = false;
		data = (infoRpm*)listRpm.first();
		//find in installed packages
		 
		while(data != NULL)
		{
			if(data->name+data->version+data->arch == data1->name+data1->version+data1->arch)
			{
				data = (infoRpm*)listRpm.next();
				continue;
			}
			list_dep = QStringList::split(",", data->deppend);
			
			it1 = list_dep.begin();
		
			while(it1 != list_dep.end())
			{
				if(s_t == (*it1).stripWhiteSpace())
				{
					b = true;
					
					if(add_to_rpm_list(*data,true))
					  b_ret=true;
					break;
				}
				it1++;
			}
						
			if(b)
			{
				t += QString("<font color=\"red\">%1</font><br>").arg(data->name);
				
				data = (infoRpm*)listRpm.next();
				b=false;
				continue;
			}
			
			//find in (installed) packages (and check version)
			if(find_in_package_version(s_t, data))
			{
				b=true;
				//it = list.remove(it);
				//it++;
				add_to_rpm_list(*data,true);
				b_ret=true;
			}
			
			if(b)
			{
				t += QString("<font color=\"red\">%1</font><br>").arg(data->name);
				b=false;
				data = (infoRpm*)listRpm.next();
				continue;
			}
				
			
			data = (infoRpm*)listRpm.next();
		}
			
		if(!t.isEmpty()){
			//s1="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font2.arg(s_t)+QString("</td>");
			//s1+="<td bgcolor=\"#EEF6FF\" width=95%>"+font.arg(t)+"</td></tr>";
			//t = "";
			s+=t;
			t="";
			++it;
			continue;
		}
				
		it++;
		//it = list.remove(it);
	}
		 
				
	return b_ret;	
}


/*!
    \fn rpm_manager::set_path_name_rpm(infoRpm )
 */
QString rpm_manager::set_path_name_rpm(infoRpm* data)
{
	QString name =data->name+"-"+data->version+"-"+data->release;
	QString path = data->pathSource+"/"+name+"."+data->arch+".rpm";
      
	return path;
}


/*!
    \fn rpm_manager::copyTmpDirFiles(QStringList listFiles)
 */
/*bool rpm_manager::copyTmpDirFiles(QStringList listFiles)
{
	QProcess *proc = new QProcess(this);
	QString pathTo=qApp->applicationDirPath();
	setAplDir(pathTo);
	pathTo+=".tmp/";
      
	QStringList newListFiles;
  
	QFile f;
  
	for ( QStringList::Iterator it = listFiles.begin(); it != listFiles.end(); ++it )
	{
		proc->clearArguments();
    
		f.setName(*it);
    
		if(!f.exists())
			return false;
		proc->addArgument("cp");
		proc->addArgument(*it);
		proc->addArgument(pathTo);
    
		if(!proc->start())
			return false; //newListFiles;
    
		while(proc->isRunning());
    
   // newListFiles+=pathTo+"/"+getNameFile(*it);    
    
	}
  
	return true;
}
*/

/*!
    \fn rpm_manager::removeTmpDirFiles()
 */
/*
void rpm_manager::removeTmpDirFiles()
{
	QString pathTo=qApp->applicationDirPath();
	setAplDir(pathTo);
	pathTo+=".tmp";
  
	QDir d(pathTo);
  
	const QFileInfoList *list = d.entryInfoList();
	QFileInfoListIterator it( *list );
	QFileInfo *fi;

	while ( (fi = it.current()) != 0 ) {
		d.remove(fi->filePath().latin1());
		++it;
	}
  
	d.rmdir(pathTo);
}

*/
/*!
    \fn rpm_manager::get_type_item(const QString& ,QString& type) //return type depend
 */
void rpm_manager::get_type_item(const QString& name,char* type)/*return type depend*/
{
	if(name.find("/")!=-1)
		type = "z_file";
	else if(name.find(".so.")!=-1)
		type = "z_lib";
	else
		type = "z_pack";

}


/*!
    \fn rpm_manager::get_symbol_compare(QString& name,QString& symbol, QString& version)//compare version package
 */
bool rpm_manager::get_symbol_compare(QString& name,QString& symbol, QString& version)/*compare version package*/
{
	name = name.stripWhiteSpace();
	
	int i = name.find("(");
	int j = name.find(")");
	
	QString s_tmp = name.mid(i,j-i+1);
	
	name.remove(i,j-i+1);
	
	trimStr(s_tmp,"()");
	
	i = s_tmp.find(" ");
	
	if(i == -1)
		return false;
	
	symbol = s_tmp.mid(0,i);
	s_tmp.remove(0,i);
	
	version = s_tmp;
	
	name = name.stripWhiteSpace();
	symbol = symbol.stripWhiteSpace();
	version = version.stripWhiteSpace();
	
	return true;
}


/*!
    \fn rpm_manager::find_in_package_version(const QString& ,bool installed=true, infoRpm* data=NULL)
 */
bool rpm_manager::find_in_package_version(const QString& name, infoRpm* dat)
{
	QString n = name, symb, ver;
	
	if(!get_symbol_compare(n, symb, ver))
		return false;
        
        //int i_ver = ver;//.remove(".")).toInt();
        QString i_ver_d = dat->version;//(dat->version.remove(".")).toInt();
      
		
	if(dat->name.stripWhiteSpace() == n)
	{
				if(symb == "="){
                                  QString t = dat->version;
                                  if(ver.find("-") != -1)
                                    t += "-"+dat->release;
                                    
                                     if( t.find(ver) == 0 )
					return true;
				 else
					 return false;
				}
				if(symb == "<="){
                                  if(i_ver_d <= ver)
						return true;
					else
						return false;
				}
				if(symb == ">="){
                                  if(i_ver_d >= ver)
						return true;
					else
						return false;
				}
				if(symb == "<"){
                                  if(i_ver_d < ver)
						return true;
					else
						return false;
				}
				if(symb == ">"){
                                  if(i_ver_d > ver)
						return true;
					else
						return false;
				}
	}

	return false;
}


/*!
    \fn rpm_manager::display_list(/*debug metod show display list)
 */
/*
void rpm_manager::display_list(QStringList& list)//debug metod show display list
{
	QStringList::Iterator it = list.begin();
	QString s;
		
	while(it != list.end())//find in installed packages
	{
		s += *it+"\n";
		it++;
	} 
	
	QMessageBox::about(0,"",s);
}
*/


/*!
    \fn rpm_manager::find_in_inst_list(infoRpm& )
 */
bool rpm_manager::find_in_inst_list(infoRpm* d, bool in_tmp_list)
{
	infoRpm* data = NULL;
	if(in_tmp_list)
		data = (infoRpm*)tmp_list.first();
	else
		data = (infoRpm*)listUnRpm.first();
	
	QString s1, s2 = d->name+d->version+d->release+d->arch;
	
	while(data != NULL)
	{
		s1 = data->name+data->version+data->release+data->arch;
		if(s1 == s2)
			return true;
		if(in_tmp_list)
			data = (infoRpm*)tmp_list.next();
		else
			data = (infoRpm*)listUnRpm.next();
	}
	return false;
}

template <class type1, class type2>
		bool rpm_manager::find_in_list(type1 *item, type2& list)
{
	type1* data = NULL;
	
	data = (type1*)list.first();
	
	QString s1, s2 = item->name+item->version+item->release+item->arch;
	
	while(data != NULL)
	{
		s1 = data->name+data->version+data->release+data->arch;
		if(s1 == s2)
			return true;
		
		data = (type1*)list.next();
	}
	return false;
}


/*!
    \fn rpm_manager::preparing_install(QString& err_msg)
 */
void rpm_manager::preparing_install_packages(const QStringList& list_rpms)
{
	QStringList listStr = list_rpms;
	infoRpm* data;
	bool b_add = false;
	
	QString rep_msg = QString("<b><i>%1<br>").arg(tr("Preparing process ..."));
	
	emit send_message(rep_msg); 
	qApp->processEvents();

	QString msg, message, s, message2, s_t;
	
	message = QString("<h2>%1</h2>").arg(tr("Selected packages"));	
	message += "<table border=0 width=100%>";
	QString font = "<font color=\"#55aa00\"><b>%1</b></font>";
	QString font2 = "<font color=\"blue\"><b>%1</b></font>";
	      
	message +="<tr><td bgcolor=\"lightGray\" width=5%>"+font.arg(tr("Package"))+"</td>";
	message +="<td bgcolor=\"lightGray\" width=95%>"+font.arg(tr("Deppends"))+"</td></tr>";
	
	message2 = QString("<h2>%1</h2>").arg(tr("Additional packages"));	
	message2 += "<table border=0 width=100%>";
	message2 +="<tr><td bgcolor=\"lightGray\" width=5%>"+font.arg(tr("Package"))+"</td>";
	message2 +="<td bgcolor=\"lightGray\" width=95%>"+font.arg(tr("Deppends"))+"</td></tr>";
	qApp->processEvents();
		
	
	rep_msg += QString("%1").arg(tr("Find package in source... "));
	emit send_message(rep_msg); 
	qApp->processEvents();
		
	for (QStringList::Iterator it = listStr.begin(); it != listStr.end(); ++it )
	{
		data = (infoRpm*)listUnRpm.first(); 
		s_t = *it; 
					
		qApp->processEvents();			
		while(data!=NULL)
		{
			if(data->name+data->version+"-"+data->release+data->arch == s_t)
			{
				add_to_rpm_list(*data,false);
				break;
			}
			qApp->processEvents();			
			data = (infoRpm*)listUnRpm.next();
		}
	}
	qApp->processEvents();
	rep_msg += QString("%1<br>").arg(tr("ok"));
	emit send_message(rep_msg); 
	qApp->processEvents();	
	rep_msg += QString("%1<br>").arg(tr("Unleash deppends ..."));
	emit send_message(rep_msg); 
	qApp->processEvents();
			
	add_rem_data *dat = NULL;
	QPtrListIterator<add_rem_data> lit( list_rpm );
	
	int in = 0;
	
	while ( (dat = lit.current()) != 0 ) {
		if(!unleash_depends(dat, msg))
			b_add = true;
		if(!msg.isEmpty())
		{
			if(dat->b_additional)
				message2 += msg;
			else
				message += msg;
			//error = true;
		}
		msg="";
		qApp->processEvents();
		++lit;
		in++;
		
		if(in == 40)
		{
		rep_msg += QString("#<br>");
			in = 0;
		}
		else
	   	  rep_msg += QString("#");
		emit send_message(rep_msg); 
		qApp->processEvents();
	}
						
	message += "</table>";
	message2 += "</table>";
	
	if(b_add)//if additional packages is added
		message += message2;
		
	emit first_report(message, b_add ); 
}

/*!
    \fn rpm_manager::remove_packages(QStringList )
 */
void rpm_manager::preparing_remove_packages(const QStringList& list_rpms)
{
	QStringList listStr = list_rpms;
	infoRpm* data;
	bool b_add = false;
	
	QString rep_msg = QString("<b><i>%1<br>").arg(tr("Preparing process ..."));
	
	emit send_message(rep_msg); 
	qApp->processEvents();

	QString msg, message, s, message2, s_t;
	
	message = QString("<h2>%1</h2>").arg(tr("Selected packages"));	
	message += "<table border=0 width=100%>";
	QString font = "<font color=\"#55aa00\"><b>%1</b></font>";
	QString font2 = "<font color=\"blue\"><b>%1</b></font>";
	      
	message +="<tr><td bgcolor=\"lightGray\" width=5%>"+font.arg(tr("Item"))+"</td>";
        message +="<td bgcolor=\"lightGray\" width=95%>"+font.arg(tr("Status"))+"</td></tr>";
	
	message2 = QString("<h2>%1</h2>").arg(tr("Additional packages"));	
	message2 += "<table border=0 width=100%>";
	message2 +="<tr><td bgcolor=\"lightGray\" width=5%>"+font.arg(tr("Item"))+"</td>";
	message2 +="<td bgcolor=\"lightGray\" width=95%>"+font.arg(tr("Status"))+"</td></tr>";
	qApp->processEvents();
		
	
	rep_msg += QString("%1").arg(tr("Find package in data base... "));
	emit send_message(rep_msg); 
	qApp->processEvents();
	      
	for (QStringList::Iterator it = listStr.begin(); it != listStr.end(); ++it )
	{
		data = (infoRpm*)listRpm.first(); 
		s_t = *it; 
		qApp->processEvents();
			
		while(data!=NULL)
		{
			if(data->name+data->version+"-"+data->release+data->arch == s_t)
			{
				add_to_rpm_list(*data,false);
				break;
			}
			qApp->processEvents();	
			data = (infoRpm*)listRpm.next();
		}
	}
	
		
	//unlead deppends
	
	qApp->processEvents();
	rep_msg += QString("%1<br>").arg(tr("ok"));
	emit send_message(rep_msg); 
	qApp->processEvents();	
	rep_msg += QString("%1 ... 0 %<br><font size=\"-1\" color=\"blue\">%2</font>")
			.arg(tr("Unleash deppends"))
			.arg(tr("Please wait. This take one or several minutes."));
	emit send_message(rep_msg); 
	qApp->processEvents();
	
	add_rem_data *dat = NULL;
	QPtrListIterator<add_rem_data> lit( list_rpm );
	
	int in=0, n_tmp=0, N;
	QString ntmp = "0 %";
	b_termit=false;
		
	while ( ((dat = lit.current()) != 0) && (!b_termit) ) {
		
		if(check_preparing_remove(dat, msg))
			b_add = true;
		
						
		if(dat->b_additional)
		{
			message2 += "<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font2.arg(dat->name)+"</td>";
			message2 += QString("<td  bgcolor=\"#EEF6FF\" width=5%><font color=\"green\">%1</font></td></tr>")
						.arg(tr("succes"));
		}
		else
		{
			message += "<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font2.arg(dat->name)+"</td>";
			message += QString("<td  bgcolor=\"#EEF6FF\" width=5%><font color=\"green\">%1</font></td></tr>")
						.arg(tr("succes"));
		}
		
		qApp->processEvents();
		++lit;
		++in;
		
		N = list_rpm.count();
		emit send_check_parcent(in, N);
		if(!N)
		  return;
		n_tmp = (in*100)/N;
		QString s1 = QString::number(n_tmp);
		QString s = QString("%1 %").arg(s1);
		
		rep_msg.replace(ntmp,s);
		
		ntmp = s;
						
		emit send_message(rep_msg); 
		qApp->processEvents();
		
	}
	
	
	message += "</table>";
	message2 += "</table>";
	
	if(b_add)//if additional packages is added
		message += message2;
		
	emit first_report(message, b_add ); 
}


/*!
    \fn rpm_manager::start_process(bool brem_all)
 */
void rpm_manager::start_process(bool brem_all, bool b_inst)
{
    if(b_inst)
    {
	rpm_i_v_h(brem_all);
    }
    else
    {
        rpm_e(brem_all);
    }
}


/*!
    \fn rpm_manager::terminate_process()
 */
void rpm_manager::terminate_process()
{
	if(p_rpm->isRunning())
		p_rpm->tryTerminate();
	
	b_termit = true;
	list_rpm.clear();
	
	QString s = QString("<font color=\"red\">%1</font>").arg(tr("User canceled"));
	
	emit finish_rpm(s, true);
}


/*!
    \fn rpm_manager::run_rpm()
 */
void rpm_manager::rpm_i_v_h(bool ball)
{
	if(list_rpm.isEmpty())
	{
		terminate_process();
			return;
	}
	
	add_rem_data *data=NULL;

	QString msg, message, s_t;
	
	list_rpm.setAutoDelete(TRUE);
	
	if(!ball)//remove additional items if user select don't install additional packages
	{
		data = (add_rem_data*)list_rpm.first();
		while(data!=NULL)
		{
			if(data->b_additional)
			{
				list_rpm.remove(data);
				data = (add_rem_data*)list_rpm.current();
			}
			else
				data = (add_rem_data*)list_rpm.next();
		}
	}
	list_rpm.setAutoDelete(FALSE);
	///begin code install
	 
	QString nameItemRpm;
	 
	QPtrList<add_rem_data> i_list_rpm;
	i_list_rpm.setAutoDelete(FALSE);
	 
	data = (add_rem_data*)list_rpm.first(); //add packages non cd/dvd sources
	while(data!=NULL)
	{
		if(!data->cd)
		{
			i_list_rpm.append(data);
			 
			list_rpm.remove();
			data = (add_rem_data*)list_rpm.current();
		}
		else
			data = (add_rem_data*)list_rpm.next();
	}
	 
	QString nameMedia, path_media;
	   
	while(!list_rpm.isEmpty())
	{
		data = (add_rem_data*)list_rpm.first(); //sort for name media
    
		nameMedia = data->nameSource;
		     
		tmp_list.removeFirst();
    
		data = (add_rem_data*)list_rpm.first();
		while(data!=NULL)
		{
			if(data->nameSource == nameMedia)
			{
				i_list_rpm.append(data);
				list_rpm.remove(data);
				data = (add_rem_data*)list_rpm.current();
			}
			else
				data = (add_rem_data*)list_rpm.next();
		}
	}
	 
	list_rpm.clear();
	 
	data = (add_rem_data*)i_list_rpm.first();
	while(data!=NULL)
	{
		list_rpm.append(data); 
		data = (add_rem_data*)i_list_rpm.next();
	} 
	 
	i_list_rpm.clear(); 	 
	
	 
	list_rpm.setAutoDelete(TRUE);
	
        int N = list_rpm.count();
        emit count_all_items(N);
        int i=1;
        b_termit = false;
        QFile f;
        
        while(!list_rpm.isEmpty())
        {
          data = (add_rem_data*)list_rpm.first();
          
          f.setName(data->pathRPM);
          
          while(!f.exists())
          {
            QString s1 = QString("%1 : %2\nNeed file : %3")
                .arg(tr("Please insert source:")).arg(data->nameSource).arg(data->pathRPM);
            int icode=QMessageBox::information(NULL,tr("Insert source"),s1,tr("&OK"), tr("&Cancel"));
            if(icode)
            {
              
              list_rpm.clear();
	
              s1 = QString("<font color=\"red\">%1</font>").arg(tr("User cancled"));
	
              emit finish_rpm(s1, true);
              
              return;
            }
            
            if(data->cd)
            {
              p_rpm->clearArguments();
              p_rpm->addArgument("mount");
              p_rpm->addArgument("/dev/cdrom");
        
              if(!p_rpm->start())
              {
                QMessageBox::warning(NULL,tr("Installation"),tr("Can't find mount command. Please mount media is manually."));
              }
    
              while(p_rpm->isRunning());
            }
          }
              
          p_rpm->clearArguments();
		
          p_rpm->addArgument("rpm");
          p_rpm->addArgument("-U");
          p_rpm->addArgument("--percent");
          p_rpm->addArgument("--nodeps");
          p_rpm->addArgument("--nosignature");
          p_rpm->addArgument("--nodigest");
          //p_rpm->addArgument("--test");
          
          data->pathRPM.replace(" ","\\ ");//unleash proglem path space folder(etc. myLinux 3.1)
          
          p_rpm->addArgument(data->pathRPM);
          		
          QString s = QString("<font>%1 </font>"
              "<font color=\"blue\">%2</font> - <font>%3</font>")
              .arg(tr("Installing")).arg(data->name+data->version).arg(data->summary);
		
          qApp->processEvents();
          emit current_all_items(i, s);
          //emit current_parcent_rpm(50);
          qApp->processEvents();
                                
          if (!p_rpm->start()){
            emit finish_rpm("FATAL ERROR: Can't find rpm command" , false );
            return;
          }
		
                           
          if(!processing_out_rpm(data->name))
            return;	
                
          while(p_rpm->isRunning())
          {
            qApp->processEvents();
            if(!processing_out_rpm(data->name))
              return;
            	
            QString s = p_rpm->readLineStdout();
				
            if(s.left(2) == "%%")
            {
              s = s.remove(0,2);	
              s = s.simplifyWhiteSpace();
			
              int i = s.find(",");
              if(i!=-1)
                s = s.mid(0,i);
			
              qApp->processEvents();
              emit current_parcent_rpm(s.toInt());
              qApp->processEvents();
			
            }
            
          }
                		
          emit current_parcent_rpm(100);
		
          list_rpm.removeFirst();
          i++;
          qApp->processEvents();
        }
         ///end code installl
        
        if(!b_termit)
          emit finish_rpm(tr("Succes") , false );
	 
        list_rpm.clear();
}

/*!
    \fn rpm_manager::rpm_e(bool ball)
 */
void rpm_manager::rpm_e(bool ball)
{
	if(list_rpm.isEmpty())
	{
		emit finish_rpm("list rpm is empty", true);
		return;
	}
	
	add_rem_data* data=NULL;
	
	if(!ball)//remove additional items if user select don't install additional packages
	{
		data = (add_rem_data*)list_rpm.first();
		while(data!=NULL)
		{
			if(data->b_additional)
			{
				list_rpm.remove(data);
				data = (add_rem_data*)list_rpm.current();
			}
			else
				data = (add_rem_data*)list_rpm.next();
		}
	}
	
	
	int N = list_rpm.count();
	emit count_all_items(N);
	int i=1;
        b_termit = false;
        
        while(!list_rpm.isEmpty())
	{
		data = (add_rem_data*)list_rpm.first(); //sort for name media
    
		p_rpm->clearArguments();
		
		p_rpm->addArgument("rpm");
		p_rpm->addArgument("-e");
		p_rpm->addArgument("--nodeps");
		//p_rpm->addArgument("--test");
		p_rpm->addArgument(data->name);
		
		QString s = QString("<font>%1 </font>"
                    "<font color=\"blue\">%2</font> - <font>%3</font>")
		.arg(tr("Remove")).arg(data->name).arg(data->summary);
		
                qApp->processEvents();
		emit current_all_items(i, s);
		emit current_parcent_rpm(50);
                qApp->processEvents();
                                
		if (!p_rpm->start()){
		  emit finish_rpm("FATAL ERROR: Can't find rpm command" , false );
		  return;
           	}
		
                           
		if(!processing_out_rpm(data->name))
			return;	
                
		while(p_rpm->isRunning())
		{
                  qApp->processEvents();
		  if(!processing_out_rpm(data->name))
		   return;	
		}
                		
		emit current_parcent_rpm(100);
		
		list_rpm.removeFirst();
		i++;
                qApp->processEvents();
    	}
        
        if(!b_termit)
	  emit finish_rpm(tr("Succes") , false );
	 
	list_rpm.clear();
}


/*!
    \fn rpm_manager::abort_added()
 */
void rpm_manager::abort_added()
{
	b_termit = true;
}


/*!
    \fn rpm_manager::reset_scaler()
 */
void rpm_manager::reset_scaler()
{
	i_scaler = 0;
	b_termit = false;
}


/*!
    \fn rpm_manager::prepar_add_rpm_src(const QString& path)/*return count all files
 */
int rpm_manager::prepar_add_rpm_src(const QString& path_rpm)/*return count all files*/
{
	FD_t fd;
	
	int n_count;
		
QString packagePattern="*.rpm;*.RPM;*.Rpm;*.rPm;*.rpM;*.RPm;*.rPM;*,RpM";
				
	QDir d(path_rpm,packagePattern);
	
	d.setMatchAllDirs( TRUE );        // list contains subdirs
		
	QDir d_files = d;
	d_files.setFilter( QDir::Files);
	
	if (d.exists()) {
		if ( d.isReadable() ) {
			n_count = d_files.count();
			if(n_count>0)
			  emit send_total_add(n_count);
			QString pn;
			
			const QFileInfoList *list = d.entryInfoList();
			
			//const QFileInfoList *listF = new QFileInfoList();
			
			//*listF = *list;
			if(list == NULL)
				return -2;
					
			QFileInfoListIterator it( *list );      // create list iterator
			QFileInfo *fi;                          // pointer for traversing
			
			while (fi=it.current()) {           // for each entry...
				if ( fi->isDir() ) {
	  // entry is a subdir
					if ( fi->fileName() != QString::fromLatin1(".") &&
										fi->fileName() != QString::fromLatin1("..") )
					{
	      // not current dir and not parent dir
	      // -> recursive call:
						prepar_add_rpm_src(path_rpm + "/" + fi->fileName());
					}
				} else {
	  // entry is a file
					if((fd = ::Fopen(QFile::encodeName(fi->absFilePath()), "r.fdio")) < 0) {
							::Fclose(fd);
						return -1;
					}
					  									
						::Fclose(fd);
				}
				
				++it;                               // goto next list element
			}
		} else {
			qWarning("Directory is not found!");
		}
	}
			 
	return 0;
}


/*!
    \fn rpm_manager::get_tmp_list()
 */
QPtrList<infoRpm> rpm_manager::get_tmp_list()
{
	return tmp_list;
}



/*!
    \fn rpm_manager::prepring_rpm(const QStringList& listRpm, bool binstall=true)
 */
int rpm_manager::preparing_rpm(const QStringList& listRpm, bool binstall)
{
	
	qApp->processEvents();
		
	if(binstall)
	{
	    preparing_install_packages(listRpm);
	}
	else
	{
	   preparing_remove_packages(listRpm);
	}
	
	//list_rpm.clear();
	return 0;
}


/*!
    \fn rpm_manager::add_to_add_rpm_list(infoRpm* )
 */
bool rpm_manager::add_to_add_rpm_list(infoRpm data)
{
	infoRpm* dat = new infoRpm;
	  
	dat->name = data.name;
	dat->group = data.group;
	dat->deppend = data.deppend;
	dat->ChangeLog = data.ChangeLog;
	dat->Description = data.Description;
	dat->files = data.files;
	dat->release = data.release;
	dat->summary = data.summary;
	dat->version = data.version;
	dat->size = data.size;
	dat->isize = data.isize;
	dat->arch = data.arch;
	dat->names_files = data.names_files;  
	dat->nameSource=data.nameSource;
	dat->pathSource=data.pathSource;
	dat->pathRPM = data.pathRPM;
	dat->cd = data.cd;
        dat->b_on = data.b_on;
	
	//if(find_in_inst_list(dat,false))
	if(find_in_list(dat,listUnRpm))
		return false;
	else
		listUnRpm.append(dat);
		
	return true;
}


/*!
    \fn rpm_manager::add_to_rem_rpm_list(infoRpm* )
 */
bool rpm_manager::add_to_rem_rpm_list(infoRpm data)
{
	if(data.group == "Public Keys")
		return true;
	
	infoRpm* dat = new infoRpm;
	  
	dat->name = data.name;
	dat->group = data.group;
	dat->deppend = data.deppend;
	dat->ChangeLog = data.ChangeLog;
	dat->Description = data.Description;
	dat->files = data.files;
	dat->release = data.release;
	dat->summary = data.summary;
	dat->version = data.version;
	dat->size = data.size;
	dat->isize = data.isize;
	dat->arch = data.arch;
	dat->names_files = data.names_files;
	
	listRpm.append(dat);
	
	return true;
}


/*!
    \fn rpm_manager::add_to_tmp_rpm_list(infoRpm* )
 */
bool rpm_manager::add_to_tmp_rpm_list(infoRpm data)
{
	infoRpm* dat = new infoRpm;
	  
	dat->name = data.name;
	dat->group = data.group;
	dat->deppend = data.deppend;
	dat->ChangeLog = data.ChangeLog;
	dat->Description = data.Description;
	dat->files = data.files;
	dat->release = data.release;
	dat->summary = data.summary;
	dat->version = data.version;
	dat->size = data.size;
	dat->isize = data.isize;
	dat->arch = data.arch;
	dat->names_files = data.names_files;
	dat->nameSource=data.nameSource;
	dat->pathSource=data.pathSource;
	dat->pathRPM = data.pathRPM;
	dat->cd = data.cd;
		        
	//if(find_in_inst_list(dat))
	if(find_in_list(dat,tmp_list))
		return false;
	else
		tmp_list.append(dat);
			
	return true;
}


/*!
    \fn rpm_manager::add_to_rpm_list(add_rem_data* )
 */
bool rpm_manager::add_to_rpm_list(infoRpm data, bool b_additional)
{
	add_rem_data* dat = new add_rem_data;
	  
	dat->name = data.name;
	dat->deppend = data.deppend;
	dat->summary = data.summary;
	dat->version = data.version;
	dat->isize = data.isize;
	dat->names_files = data.names_files;
	dat->nameSource=data.nameSource;
	dat->pathSource=data.pathSource;
	dat->pathRPM = data.pathRPM;
	dat->cd = data.cd;
	dat->b_additional = b_additional;
	dat->release = data.release;
	dat->arch = data.arch;
		        
	if(find_in_list(dat,list_rpm))
		return false;
	else
		list_rpm.append(dat);
			
	return true;
}



/*!
    \fn rpm_manager::processing_out_rpm()
 */
bool rpm_manager::processing_out_rpm(const QString& name_rpm, bool red_all)
{
	QString s1;
	if(red_all)
	  s1 += p_rpm->readStderr();
	else
	  s1 = p_rpm->readLineStderr();
        	
	if(!s1.isEmpty())
	{
		p_rpm->tryTerminate();
                QTextCodec *codec = QTextCodec::codecForLocale();
                s1 = codec->toUnicode(s1);
                
		QString t = QString("<font color=\"red\">%1 - %2 : %3").arg(tr("Error package")).arg(name_rpm).arg(s1);
                
                if(p_rpm->isRunning())
                {
                   int exit_code = QMessageBox::information(0,tr("rpm message"),QString("%1 :\n\t %2\n%3").arg(tr("Rpm out message")).arg(t).arg(tr("continue ?")), tr("Yes"), tr("No"), 0, 0, 1 );
                
                   if(!exit_code)
                     return true;
                }
                
		emit finish_rpm(t , true );
		emit current_parcent_rpm(100);
		
		list_rpm.clear();
		
		return false;
	}
	return true;
}

