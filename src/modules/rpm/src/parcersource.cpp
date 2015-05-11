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
#include "parcersource.h"
#include "str_tools.h"

#include <qprocess.h>
#include <qmessagebox.h>


//extern int i_rpm; //1 = name rpm, 2 = group, 3 = version, 4 = release 5 = summary 6 = description 7 = files 8 = size;

ParcerSource::ParcerSource(QPtrList<infoRpm> *list/*, const QString& name*/)
{
	listR = list;
	
	/*if(!name.isNull())
	{
		comprs_decopmrs_heder(name, false);
		name_src = name;
}*/
}


ParcerSource::~ParcerSource()
{
	//comprs_decopmrs_heder(name_src, true);
}




/*!
    \fn ParcerSource::characters(const QString &str)
 */
bool ParcerSource::characters(const QString &str)
{
   //1 = name rpm, 2 = group, 3 = version, 4 = release 5 = summary 6 = description 7 = files;
	switch(i_rpm){
		case 1: data.name = str; break;
		case 2: data.group = str; break;
		case 3: data.version = str; break;
		case 4: data.release = str; break;
		case 5: data.summary = str; break;
		case 6: data.Description = str; break;
		case 7: data.names_files = string2stringlist(str); break;
		case 8: data.isize = str.toInt(); data.size = convert_size(str.toInt()); break;
		case 9: data.arch = str; break;
		case 10: data.deppend = str; break;
		//case 11: data.pathRPM = str; break;
	}
	
	i_rpm = 0;
	return true;
}


/*!
    \fn ParcerSource::endElement(const QString &namespaceURI, const QString &localName, const QString &qName) 
 */
bool ParcerSource::endElement(const QString &namespaceURI, const QString &localName, const QString &qName) 
{
	if (qName == "rpm") 
		add_List();
	return true; 
}


/*!
    \fn ParcerSource::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
 */
bool ParcerSource::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
	//int i_rpm; //1 = name rpm, 2 = group, 3 = version, 4 = release 5 = summary 6 = description 7 = files 8 = size;
	QString s;
	if (qName == "rpm") { 
		data.nameSource = attribs.value("src"); 
		data.pathSource = attribs.value("src_path");
		data.pathRPM = attribs.value("rpm_path");
		s = attribs.value("removable");
		if(s == "true")
			data.cd = true;
		else
			data.cd = false;
                
                s = attribs.value("on");
                if(s == "true")
                  data.b_on = true;
                else
                  data.b_on = false;
		
	}
	else
	   if (qName == "name")
	   {  
		     i_rpm = 1; 
				 
	   }
	else
	   if (qName == "group") 
		   i_rpm = 2; 
	else
	   if (qName == "version")  
		    i_rpm = 3; 
	else
	  if (qName == "release")
			i_rpm = 4; 
	else
	   if (qName == "summary")  
		   i_rpm = 5; 
	else
	    if (qName == "description")  
		    i_rpm = 6; 
	else
	    if (qName == "files")  
		    i_rpm = 7; 
	else
	    if (qName == "size")  
		    i_rpm = 8; 
	else
		if (qName == "arch")  
			i_rpm = 9; 
	else
		if (qName == "dep")  
			i_rpm = 10; 
	
	return true; 
}


/*!
    \fn ParcerSource::fatalError(const QXmlParseException &exception)
 */
bool ParcerSource::fatalError(const QXmlParseException &exception)
{
	qWarning("Line %d, column %d: %s", exception.lineNumber(), 
					 exception.columnNumber(), exception.message().ascii()); 
	return false; 
}


/*!
    \fn ParcerSource::add_List()
 */
void ParcerSource::add_List()
{
	infoRpm* dat = new infoRpm;
  	
	dat->name = data.name;
	dat->group = data.group;
	dat->Description = data.Description;
	dat->names_files = data.names_files;
	dat->release = data.release;
	dat->summary = data.summary;
	dat->version = data.version;
	dat->isize = data.isize;
	dat->size = data.size;
	dat->nameSource=data.nameSource;
	dat->pathSource=data.pathSource;
	dat->pathRPM = data.pathRPM;
	dat->cd = data.cd;
	dat->arch = data.arch;
	dat->deppend = data.deppend;
        dat->b_on = data.b_on;
		
	listR->append(dat);
	return;
}


/*!
    \fn ParcerSource::write_source()
 */
void ParcerSource::write_source(QString name)
{
	QString glob = "<xml version=\"1.0\" encoding=\"utf8\">\n";
	QString tmp;
	
	infoRpm* dat = (infoRpm*)listR->first();
	
	while(dat != NULL)
	{
		tmp = QString("<rpm src=\"%1\" src_path=\"%2\" rpm_path=\"%3\"").arg(dat->nameSource)
				.arg(dat->pathSource).arg(dat->pathRPM);
		
		if(dat->cd)
			tmp+=" removable=\"true\"";
		else
			tmp+=" removable=\"false\"";
                if(dat->b_on)
                  tmp+=" on=\"true\">\n";
                else
                  tmp+=" on=\"false\">\n";
		
		tmp += QString("<name>%1</name>\n").arg(convert_chars(dat->name));
		tmp += QString("<group>%1</group>\n").arg(convert_chars(dat->group));
		tmp += QString("<version>%1</version>\n").arg(convert_chars(dat->version));
		tmp += QString("<release>%1</release>\n").arg(convert_chars(dat->release));
		tmp += QString("<summary>%1</summary>\n").arg(convert_chars(dat->summary));
		tmp += QString("<description>%1</description>\n")
				.arg(convert_chars(dat->Description));
		tmp += QString("<files>%1</files>\n").arg(stringList2String(dat->names_files));
		tmp += QString("<size>%1</size>\n").arg(dat->isize);
		tmp += QString("<arch>%1</arch>\n").arg(dat->arch);
		tmp += QString("<dep>%1</dep>\n").arg(convert_chars(dat->deppend));
		
		tmp += "</rpm>\n";
		
		glob+=tmp;
		
		dat = (infoRpm*)listR->next();		
	}
	
	glob += "</xml>\n";
			
	QFile file(name);
	
	if(file.open(IO_WriteOnly))
	{
	  QTextStream out(&file); 
	  out.setEncoding(QTextStream::UnicodeUTF8); 
	  out << glob;
	 file.close();
	}
	
	//comprs_decopmrs_heder(name);
}


/*!
    \fn ParcerSource::stringList2String(const QStringList& )
 */
QString ParcerSource::stringList2String(const QStringList& str1)
{
	QStringList str = str1;
	QString s, t;
	for ( QStringList::Iterator it = str.begin(); it != str.end(); ++it )
	{
		t = *it;
		
		t.replace("&","[]");
		s += t+"|";
	}
	
	return s;
}


/*!
    \fn ParcerSource::string2stringlist(QString s)
 */
QStringList ParcerSource::string2stringlist(QString s)
{
	QStringList list;
	list = QStringList::split("|", s);
	
	return list;
}


/*!
    \fn ParcerSource::convert_chars(const QString& )
 */
QString ParcerSource::convert_chars(const QString& s)
{
	QString tmp = s; 
	tmp.replace("&", "&amp;"); 
	tmp.replace("<", "&lt;"); 
	tmp.replace(">", "&gt;"); 
	//tmp.replace(" ", "&apos;"); 
	tmp.replace("\"", "&quot;");
	
	return tmp;
}

/*
/*!
    \fn ParcerSource::comprs_decopmrs_heder(bool compress=true)
 /*
void ParcerSource::comprs_decopmrs_heder(const QString& name, bool compress)
{
	QProcess *arh = new QProcess();
	
	if(compress)//compress
	{
			arh->addArgument("bzip2");
			arh->addArgument("-z");
			arh->addArgument("--best");
			arh->addArgument(name);
				
			if(!arh->start())
			{
				return;
			}
			while(arh->isRunning());
					
			arh->clearArguments();
	}
	else//decompress
	{
		QString s = name+".bz2";
		
		arh->addArgument("bzip2");
		arh->addArgument("-d");
		arh->addArgument(s);
			
		if(!arh->start())
		{
			qWarning("Can't find bzib2 decompressor.");
			return;
		}
		
		while(arh->isRunning());
					
		arh->clearArguments();
	}
		
	delete arh;
}*/
