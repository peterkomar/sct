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
 
#include "qinfopanel.h"

#include <qtextedit.h>
#include <qcheckbox.h>
#include <qsettings.h>
#include <qtabwidget.h>
#include <qapplication.h>

QInfoPanel::QInfoPanel(QWidget *parent, const char *name)
	:info_panel(parent, name)
{
	connect(InfoBar, SIGNAL(currentChanged(QWidget *)), this, SLOT(slotChanged(QWidget *)));	
}


/*!
    \fn QInfoPanel::show_info_package(infoRpm data)
 */
void QInfoPanel::show_info_package(infoRpm rpm_data)
{
	QString s;
	          
	data.ChangeLog=rpm_data.ChangeLog;
	data.Description=rpm_data.Description;
	data.arch=rpm_data.arch;
	data.deppend=rpm_data.deppend;
	data.files=rpm_data.files;
	data.group=rpm_data.group;
	data.name=rpm_data.name;
	data.nameSource=rpm_data.nameSource;
	data.names_files=rpm_data.names_files;
	data.release=rpm_data.release;
	data.size=rpm_data.size;
	data.summary=rpm_data.summary;
	data.version=rpm_data.version;
		            
	QWidget *w = InfoBar->currentPage();
	if(w == desk)
	{
		s = format_description();
		desc->setText(s);
	}
	else if(w == Files)
	{
		s = format_content();
		out_files->setText(s);
	}
	else if(w == chlog)
	{
		s = format_log();
		out_chlog->setText(s);
	}
	else if(w == deppends)
	{
		s = format_dep();
		out_deppends->setText(s);
	}
	
	/*desc->setText(str_descrip);
	out_files->setText(str_files);
	out_chlog->setText(str_chlog);
	out_deppends->setText(str_deppends);*/
}


/*!
    \fn QInfoPanel::format_description(QString& s)
 */
void QInfoPanel::format_description(QString& s)
{
	s.replace("\n","<br>");
}


/*!
    \fn QInfoPanel::slot_set()
 */
QString QInfoPanel::format_description()
{
	qApp->processEvents();
	QApplication::setOverrideCursor( waitCursor );
	QString str_descrip;
	QString font = "<font color=\"#55aa00\"><b>%1</b> : </font>";
	QString font2 = "<font color=\"blue\"><b>%1</b> : </font>";
            
	str_descrip = "<html><head></head><body>";
	str_descrip +="<table border=0 width=100%>"
			"<tr>"
			"<td bgcolor=\"lightGray\" colspan=2 width=100%>";
	str_descrip+=font.arg(tr("Package"));
	str_descrip+=data.name+" - ("+data.summary+")</td></tr></table>";
	
	str_descrip+="<br>";
	str_descrip += font2.arg(tr("Detailed"));
	str_descrip += "<br>";
	
	str_descrip += "<table border=0 width=100%>";
	      
	str_descrip += "<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font.arg(tr("Version"))+"</td>";
	str_descrip += "<td bgcolor=\"#EEF6FF\" width=95%>"+data.version+"-"+data.release+"</td></tr>";
	
	str_descrip+="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font.arg(tr("Size"))+"</td>";
	str_descrip+="<td bgcolor=\"#FFFFFF\" width=95%>"+data.size+"</td></tr>";
	
	str_descrip+="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font.arg(tr("Architecture"))+"</td>";
	str_descrip+="<td bgcolor=\"#EEF6FF\" width=95%>"+data.arch+"</td></tr>";
	
	bool b = true;
	
	if(!data.nameSource.isEmpty())
	{
		str_descrip+="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font.arg(tr("Source"))+"</td>";
		str_descrip+="<td bgcolor=\"#FFFFFF\" width=95%>"+data.nameSource+"</td></tr>";
		b =  false;
	}
	  	      
	//str_descrip+=QString("<font color=\"#55aa00\"><b>%1</b> : </font>").arg(tr("Summary"));
	//str_descrip+=data.summary+"<br><br>";
      
	str_descrip+="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font.arg(tr("Description"))+"</td>";
	QString s = data.Description;
	format_description(s);
	if(b)
		str_descrip+="<td bgcolor=\"#FFFFFF\" width=95%>"+s+"</td></tr>";
	else
		str_descrip+="<td bgcolor=\"#EEF6FF\" width=95%>"+s+"</td></tr>";
	str_descrip +="</table>";
	str_descrip += "</body></html>";
	
	QApplication::restoreOverrideCursor();
	
	return str_descrip;
}


/*!
    \fn QInfoPanel::disable_Log()
 */
QString QInfoPanel::format_log()
{
	qApp->processEvents();
	QApplication::setOverrideCursor( waitCursor );
	QString str_chlog;
	
	QString font = "<font color=\"#55aa00\"><b>%1</b> : </font>";
	            
	str_chlog = "<html><head></head><body>";
	str_chlog +="<table border=0 width=100%>"
			"<tr>"
			"<td bgcolor=\"lightGray\" colspan=2 width=100%>";
	str_chlog+=font.arg(tr("Package"));
	str_chlog+=data.name+" - ("+data.summary+")</td></tr></table>";
	
	if(!data.ChangeLog.isEmpty())
		{    
			str_chlog+="<br><br><hr><i>";
      
			for ( QStringList::Iterator it = data.ChangeLog.begin(); it != data.ChangeLog.end(); ++it )
				str_chlog+=*it+"<br>";
		}
		else
			str_chlog+=QString("<br><font color=\"#BEDBFF\" size=\"+4\"><i>%1</i></font>").arg(tr("Information only available for installed packages"));
		
		str_chlog += "</i></body></html>";
		
		QApplication::restoreOverrideCursor();
	
		return str_chlog;
}


/*!
    \fn QInfoPanel::disable_dep()
 */
QString QInfoPanel::format_dep()
{
	qApp->processEvents();
	QApplication::setOverrideCursor( waitCursor );
	
	QString str_deppends;
	
	QString font = "<font color=\"#55aa00\"><b>%1</b> : </font>";
	            
	str_deppends = "<html><head></head><body>";
	str_deppends +="<table border=0 width=100%>"
			"<tr>"
			"<td bgcolor=\"lightGray\" colspan=2 width=100%>";
	str_deppends+=font.arg(tr("Package"));
	str_deppends+=data.name+" - ("+data.summary+")</td></tr></table>";
	
	bool b_d=true;
	QString s1 = "<tr><td bgcolor=\"#EEF6FF\"> %1</td></tr>";
	//QString s2 = "<tr><td bgcolor=\"#FFFFFF\"> %1</td></tr>";
	QString s3 = "<tr><td bgcolor=\"#FFDEE6\"> %1</td></tr>";
	
		str_deppends += "<br><br><hr>";
		str_deppends += "<table border=0 width=100%>";
	      
		data.deppend.replace("<","&lt;");
		data.deppend.replace(">","&gt;");
	
		QStringList strL = QStringList::split(",", data.deppend);
	
		for ( QStringList::Iterator it = strL.begin(); it != strL.end(); ++it )
		{
			if((*it).find("rpmlib")!=-1)
				continue;
		
			if(b_d)
			{
				str_deppends += s1.arg(*it);
				b_d = false;
			}
			else
			{
				str_deppends += s3.arg(*it);
				b_d = true;
			}
		}
	
		str_deppends += "</table>";
		
		str_deppends += "</body></html>";
		
		QApplication::restoreOverrideCursor();
		
		return str_deppends;
}


/*!
    \fn QInfoPanel::format_content()
 */
QString QInfoPanel::format_content()
{
	qApp->processEvents();
	QApplication::setOverrideCursor( waitCursor );
	
	QString str_files;
	
	QString font = "<font color=\"#55aa00\"><b>%1</b> : </font>";
	            
	str_files = "<html><head></head><body>";
	str_files +="<table border=0 width=100%>"
			"<tr>"
			"<td bgcolor=\"lightGray\" colspan=2 width=100%>";
	str_files+=font.arg(tr("Package"));
	str_files+=data.name+" - ("+data.summary+")</td></tr></table>";
			            
	bool b_d=true;
	QString s1 = "<tr><td bgcolor=\"#EEF6FF\"> %1</td></tr>";
	QString s2 = "<tr><td bgcolor=\"#FFFFFF\"> %1</td></tr>";
	//QString s3 = "<tr><td bgcolor=\"#FFDEE6\"> %1</td></tr>";
	int n_count;
	
	if(!data.files.isEmpty())
		{
			str_files+=QString("<br><font color=\"#55aa00\"><b>%1</b> :</font><br><hr><i>").arg(tr("Files"));
			str_files += "<table border=0 width=100%>";
                        n_count = data.files.count();
                        int n=100, i_n=0; /** code for optimizes work manager*/
					  /** if You want see all list files - set n = 10000000000000*/
		
			for ( QStringList::Iterator it = data.files.begin(); it != data.files.end(); ++it )
			{
                                ++i_n;
				if(b_d)
				{
					str_files += s1.arg(*it);
					b_d = false;
				}
				else
				{
					str_files += s2.arg(*it);
					b_d = true;
				}
				
				qApp->processEvents();
                              if(i_n == n)
			      {
				str_files += "<tr><td>...</td></tr>";
				      
     				break;
			      }
			}
			
			str_files += "</table>";
			
			str_files += "<br><br>";
			str_files += QString("<center>%1 - %2</center>").arg(n_count).arg(tr("files"));
		}
		else if(!data.names_files.isEmpty())
		{
			str_files+=QString("<br><font color=\"#55aa00\"><b>%1</b> :</font><br><hr><i>").arg(tr("Provide names"));
			str_files += "<table border=0 width=100%>";
			
			n_count = data.names_files.count();
		
			for ( QStringList::Iterator it = data.names_files.begin(); it != data.names_files.end(); ++it )
			{
				if(b_d)
				{
					str_files += s1.arg(*it);
					b_d = false;
				}
				else
				{
					str_files += s2.arg(*it);
					b_d = true;
				}
				qApp->processEvents();
			}
			str_files += "</table>";
			
			str_files += "<br><br>";
			str_files += QString("%1 - %2").arg(n_count).arg(tr("provides"));
		}
		else
			str_files += QString("<i>%1</i>").arg(tr("Empty"));
		
		str_files+="<br></i>";
		
		str_files+="</body></html>";
				
		QApplication::restoreOverrideCursor();
		
		return str_files;
}


/*!
    \fn QInfoPanel::slotChanged( QWidget * )
 */
void QInfoPanel::slotChanged( QWidget *w)
{
	disconnect(InfoBar, SIGNAL(currentChanged(QWidget *)), this, SLOT(slotChanged(QWidget *)));
	QString s;
	
	if(w == desk)
	{
		out_files->hide();
		out_chlog->hide();
		out_deppends->hide();
		
		//desc->setText("<html><head></head><body><center>Please wait. Listen information...</center></body></html>");
		s = format_description();
		desc->setText(s);
		desc->show();
		InfoBar->setCurrentPage(0);
	}
	else if(w == Files)
	{
		out_chlog->hide();
		out_deppends->hide();
		desc->hide();
		//out_files->setText("<html><head></head><body><center>Please wait. Listen information...</center></body></html>");
		s = format_content();
		out_files->setText(s);
		out_files->show();
		//qApp->processEvents();
		InfoBar->setCurrentPage(1);
	}
	else if(w == chlog)
	{
		//out_chlog->setText("<html><head></head><body><center>Please wait. Listen information...</center></body></html>");
		out_deppends->hide();
		desc->hide();
		out_files->hide();
		
		s = format_log();
		out_chlog->setText(s);
		out_chlog->show();
		
		InfoBar->setCurrentPage(2);
	}
	else if(w == deppends)
	{
		desc->hide();
		out_files->hide();
		out_chlog->hide();
		
		//out_deppends->setText("<html><head></head><body><center>Please wait. Listen information...</center></body></html>");
		s = format_dep();
		out_deppends->setText(s);
		out_deppends->show();
		
		InfoBar->setCurrentPage(3);
	}
	connect(InfoBar, SIGNAL(currentChanged(QWidget *)), this, SLOT(slotChanged(QWidget *)));
}
