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
#include "lilomain.h"
#include "propertiesitemimpl.h"
#include "mypartitions.h"
#include "typeitem.h"

#include <qmessagebox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlistview.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qtoolbutton.h>
#include <qaction.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qwhatsthis.h>
#include <qtoolbox.h>
#include <qspinbox.h>

#include <qlayout.h>
#include <qsettings.h>

//extern liloMain *lilo_main;

liloMain::liloMain(QWidget *parent, const char *name)
	:lilo_view(parent, name)
{
	QToolBar * fileTools = new QToolBar( this, "file operations" );
	fileTools->setLabel( tr("File Operations") );
	addDockWindow(fileTools, Bottom, TRUE);
	fileTools->move(100,10);

	QString s = qApp->applicationDirPath();
        int i;
        if(s.right(1)=='/')
        {
          i = s.findRev('/');
    
          s.remove(i,3);
        }
        s+="+";
        i = s.findRev("/bin+");
        s.remove(i,20);
        s += "/pixmaps/tux_config.png";
        QPixmap pix(s);
	Lilo->insertItem(pix,tr("Set Parameters"),this, SLOT(slotSetParameter()),0,-1,0);
	
	QToolButton * fileAply = new QToolButton( pix, tr("Set Parameters"), QString::null,
					   this, SLOT(slotSetParameter()), fileTools, "Set parameters" );
	//fileAply->setUsesBigPixmap(TRUE);
	fileAply->setUsesTextLabel(TRUE);
	fileAply->setTextLabel(tr("Set parameters"),TRUE);
	fileAply->setTextPosition(QToolButton::BesideIcon);
    
	QString fileOpenText = tr("Click this button to run a <em>lilo command</em>. <br>"
			"You can also select the <b>Set Patrameters</b> command "
			"from the <b>File</b> menu.</p>");
		
	conf = new EditLiloConf(this);
	
	v_box = new QVBox(centralWidget());
	v_box->setFrameStyle (QFrame::NoFrame);
	lilo_viewLayout->addWidget( v_box, 0, 1 );
	
	tex_lab = new QLabel(tr("Progress window:"),v_box);	
  out_message = new QTextEdit(v_box);
  out_message->setMinimumSize( QSize( 500, 400 ) );
  out_message->setReadOnly(TRUE);
  out_message->setFocusPolicy(NoFocus);
  out_message->setMargin(8);
  out_message->zoomTo (12);
    
  finishBtn = new QPushButton(tr("Finish"),v_box);
  
  v_box->hide();
    
  init_connections();
  
  loadData();
  
  slotChangeTabs(0);
  setCaption( "SCT lilo @Beta2" );
}

liloMain::~ liloMain()
{
  delete conf;
}

void liloMain::closeEvent ( QCloseEvent * e )
{
	e->accept();
}

void liloMain::fileExit()
{
	close();
}

void liloMain::helpAbout()
{
	QMessageBox::about(this, tr("About SCT lilo"), tr("Configure tool for <lilo> loader.\n"
			"Module for SCT.\ndevelopment: sksoft@i.ua"));
}

void liloMain::slotSetParameter()
{
	slotChangeTabs(3);//help panel
	toolBox1->hide();
	out_message->clear();
	
	finishBtn->setEnabled(FALSE);
	v_box->show();
	
	if(disbTimer->isChecked())
		conf->setParameters(selDev->currentText(),
								  "node",listThemes->currentText());
	else
		conf->setParameters(selDev->currentText(),
											 selPause->text(),listThemes->currentText());
	
	finishBtn->setEnabled(TRUE);
}	

void liloMain::slotEditItem(QListViewItem* )
{
}


void liloMain::slotAddNewItem()
{
	typeItem* m_Select = new typeItem(this);
  
	connect(m_Select,SIGNAL(sendSelectedItem(int )),this,SLOT(slotSelect(int )));
  
	m_Select->exec();
	
	disconnect(m_Select,SIGNAL(sendSelectedItem(int )),this,SLOT(slotSelect(int )));
  
	delete m_Select;
  
	myMenuLiloData data;
  
	if(b_win==1)
		data.type_os = "windows family";
	if(b_win==0)
		data.type_os = "linux family";
	if(b_win==-1)
		return;
    
	propertiesItemImpl *dlg = new propertiesItemImpl(data, true,this);
    
	connect(dlg,SIGNAL(sendData(myMenuLiloData
			, QString )),this,SLOT(slotNewItem( myMenuLiloData ))); 
 
	dlg->exec();
	
	disconnect(dlg,SIGNAL(sendData(myMenuLiloData
			, QString )),this,SLOT(slotNewItem( myMenuLiloData ))); 
	
	delete dlg;
}

void liloMain::slotEditItem( )
{
	QListViewItem *item =  listOS->currentItem();
	
	if(!item)
		return;
	QString s_name = item->text(0);
	QPtrList<myMenuLiloData> list_os = conf->getBootMenu();
    
	myMenuLiloData *p_data_os = NULL;
  
	for(p_data_os = list_os.first();p_data_os; p_data_os = list_os.next() )
	{
		if(!p_data_os->label.compare(s_name))
			break;
	}
    
	if(p_data_os==NULL)
		return;
	
	  
	propertiesItemImpl *dlg = new propertiesItemImpl(*p_data_os,false,this);
    
	connect(dlg,SIGNAL(sendData( myMenuLiloData ,QString )),this,SLOT(slotEditItem( myMenuLiloData ,QString )));
    
	dlg->exec();
    
	disconnect(dlg,SIGNAL(sendData( myMenuLiloData ,QString )),this,SLOT(slotEditItem( myMenuLiloData ,QString )));
    
	delete dlg;
}


void liloMain::slotRemoveItem()
{
	QListViewItem *item = listOS->currentItem();
	
	if(!item)
		return;
	
	switch( QMessageBox::warning( this, tr("Remove"),
			  tr("Do you want realy remove: %1").arg(item->text(0)),
			  tr("Yes"),
			  tr("No"), 0, 0, 1 ) ) {
				  case 0: 
					  conf->removeOS(item->text(0));
					  listOS->takeItem(item);
					  	 break;
				  case 1: 
						  break;
			  }
}





/*!
    \fn liloMain::slotEditItem(myMenuLiloData )
 */
void liloMain::slotEditItem(myMenuLiloData data, QString oldLabel)
{
	conf->editExistOS( data, oldLabel);
	
	loadMenu();
}


/*!
    \fn liloMain::slotNewItem(myMenuLiloData )
 */
void liloMain::slotNewItem(myMenuLiloData data)
{
	conf->addNewOS(data);
	
	loadMenu();
}


/*!
    \fn liloMain::loadData()
 */
void liloMain::loadData()
{
  QString dev, time;
  
  conf->getMainParameter(&dev, &time);
  QPtrList<theme_data>lis = conf->getListThemes();
  
  theme_data *p_data = NULL;
  
  for(p_data = lis.first();p_data; p_data = lis.next() )
  {
	listThemes->insertItem(p_data->name);
  }
  
  QString name = conf->getCurrentTheme();
  
  if(name.isEmpty())
	listThemes->setCurrentItem(0);
  else
  {
	QListBoxItem* itm = listThemes->findItem(name);
	listThemes->setCurrentItem(itm);
  }
  
  selDev->insertStringList(myPartitions::getPartitions()); 
  
  selDev->setCurrentText(dev);
  
  if(time.isEmpty())
	  disbTimer->setChecked(TRUE);
  else
    selPause->setValue(time.toInt());
  
  loadMenu();
}


/*!
    \fn liloMain::init_connections()
 */
void liloMain::init_connections()
{
  connect(listThemes, SIGNAL(highlighted(QListBoxItem * )), this, SLOT(slotClickTheme(QListBoxItem* )));
  connect(conf, SIGNAL(sendOut(QString ,int )), this, SLOT(slotReadOut(QString ,int )));
  connect(finishBtn, SIGNAL(clicked()), this, SLOT(slotClickFin()));
}


/*!
    \fn liloMain::slotClickTheme(QListBoxItem* )
 */
void liloMain::slotClickTheme(QListBoxItem* item)
{
  theme_data *p_data = NULL;
  
  QPtrList<theme_data>lis = conf->getListThemes();
  
  for(p_data = lis.first();p_data; p_data = lis.next() )
  {
	if(p_data->name == item->text())
	{
	  pixTheme->setPixmap(QPixmap(p_data->path_pixmap));
	  break;
	}
  } 
}


/*!
    \fn liloMain::slotReadOut(QString )
 */
void liloMain::slotReadOut(QString s, int mode)
{
  if(mode == 0)//normal
	  out_message->setColor(black);
  else if(mode == 1)//program output
	  out_message->setColor(darkGreen);
  else if(mode == -1)//error output
	  out_message->setColor(red);
  out_message->append(s);
	qApp->processEvents(); 
}

void liloMain::slotChangeTabs(int i)
{
	QString page1 = tr("<h3>Main parameters.</h3>Select device in <b>Device boot:</b> where you want set lilo loader."
			"select time in <b>Pause:</b> what want show boot menu, or click <b>disable timer</b>"
			" for remove timer");
	QString page2 = tr("<h3>Configure boot menu.</h3>"
			"The button of <b>add new</b> allows to add new load menu items. Accessible two types of <b>linux</b> and"
			" <b>other</b>. At the choice of element as linux it will be needed to fill"
			"6 fields:<br><b>Label</b> - is the name of element of menu (name of the"
			"operating system).<br><b>Image</b> - is a path and name of appearance of kernel of linux.<br>"
			"<b>Root</b> - is section linux is set in which.<br>"
			"<b>Add</b> - not obligatory for filling. The additional parameters of load are here specified for a kernel<br>"
			"<b>Video</b> - video is specified mode which for the load of linux.<br>"
			"<b>Initrd</b> - is a path and kernel of linux.<br>"
			"Option of <b>set default</b> sets the set operating system load on saying nothing."
			"At the choice of element <b>other</b> it will be needed to fill 2 fields:<br>"
			"<b>Label</b> - is the name of element of menu (name of the operating system).<br>"
			"<b>Root</b> - is section in which the set operating system.<br>"
			"Button of <b>Edit</b> - allows to edit the existent elements of menu of load.<br>"
			"Button of <b>Remove</b> - allows to delete the existent elements of menu of load.");
	QString page3 = tr("<h3>Select theme.</h3>Select baground pixmap for boot menu.");
	QString page4 = tr("<h3>Apling data.</h3>Apling changes. Please wait, this take one or several seconds.<br>"
			"Errors show red color. If you see messages red color please checkes parameters.");
	switch(i)
	{
		case 0: helpLabel->setText(page1); break;
		case 1: helpLabel->setText(page2); break;
		case 2: helpLabel->setText(page3); break;
		case 3: helpLabel->setText(page4); break;
	}
}




/*!
    \fn liloMain::slotClickFin()
 */
void liloMain::slotClickFin()
{
	v_box->hide();
	
	toolBox1->show();
	
	slotChangeTabs(0);
	toolBox1->setCurrentItem(0);
}


/*!
    \fn liloMain::slotSelect(int )
 */
void liloMain::slotSelect(int i)
{
	b_win = i;
}


/*!
    \fn liloMain::loadMenu()
 */
void liloMain::loadMenu()
{
	QPtrList<myMenuLiloData> list_os = conf->getBootMenu();
	
	listOS->clear();
  
	myMenuLiloData *p_data_os = NULL;
  
	for(p_data_os = list_os.first();p_data_os; p_data_os = list_os.next() )
	{
		(void) new QListViewItem( listOS, p_data_os->label, p_data_os->type_os, p_data_os->main? tr("Yes") : tr("No"));
	}
}


/*!
    \fn liloMain::helpIndex()
 */
void liloMain::helpIndex()
{
	QProcess proc;
	proc.clearArguments();
        if(getenv("KDE_FULL_SESSION"))
        {
           proc.addArgument("kfmclient");
           proc.addArgument("openURL");
        }
        else
        {
          proc.addArgument("yelp");
          //proc.addArgument("openURL");
        }
           
	//proc.addArgument("khelpcenter");
   
	QString path = qApp->applicationDirPath();
	int i = path.findRev('/');
	path.remove(i,20);
  
	QString s;// = "file:";
	s+= path;
        
	s+="/doc/"+get_locale()+"/sct_lilo.html";
  
	proc.addArgument(s);
  
	if(!proc.start())
		QMessageBox::information(this,tr("Info"),tr("Can't find help system."));
}


/*!
    \fn liloMain::get_locale()
 */
QString liloMain::get_locale()
{
  QDir d = QDir::home();
  
  QString s = d.absPath();
	
  QSettings settings;
	 
  settings.removeSearchPath( QSettings::Unix, s+"/.qt");
  settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
   
  QString slang = settings.readEntry("/SCT/Language_UI","en"); 
  
  return slang;
}
