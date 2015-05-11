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
 
#include "rpm_ui.h"

#include <qpopupmenu.h>
#include <qaction.h>
#include <qapplication.h>
#include <qprocess.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qtooltip.h>
#include <qstatusbar.h>
#include <qiconset.h>
#include <qsettings.h>

//unix include
//#include <unistd.h>
//#include <sys/types.h>


rpm_ui::rpm_ui(bool i, QWidget *parent, const char *name)
	:QMainWindow(parent, name)
	,b_installed(i)
{
	uid = geteuid();
        
	//uid=0;  //for debug program
        
	//setMinimumSize(800,533);
	rpm_main = new RPM_Views(this);
		
	if(!loadIcons())
		QMessageBox::critical(this,tr("Error"), tr("Can't load icons."));
	
	load_menu();
	greate_gui();
	init_connections();
	showMaximized();
	
	if(i)
		uninstalled_act->setOn(TRUE);
	else
	 installed_act->setOn(TRUE);
	
	init_connections();
	
	if(uid != 0)
	 QMessageBox::information(this, tr("Information"),
 tr("You execute package manager like not root.\n If you want install, remove packages or edit sources, please run like root."));
}


/*!
    \fn rpm_ui_ui::greate_gui()
 */
void rpm_ui::greate_gui()
{
	(void)statusBar();
	setMinimumSize( QSize( 728, 467 ) );
	setCentralWidget(rpm_main);
	
	addDockWindow(irpmTools, Bottom, FALSE);
	addDockWindow(urpmTools, Bottom, FALSE);
	
	if(uid != 0)
	{
		menu_rpm->setItemEnabled(12, FALSE);
	}
}


/*!
    \fn rpm_ui::init_connections()
 */
void rpm_ui::init_connections()
{
	connect(rpm_main, SIGNAL(sendMessageStatus( const QString& )),this, SLOT(slotMessageStatus( const QString& )));
}


/*!
    \fn rpm_ui::fileExit()
 */
void rpm_ui::fileExit()
{
	close();
}


/*!
    \fn rpm_ui::helpAbout()
 */
void rpm_ui::helpAbout()
{
	QMessageBox::about(this, tr("About SCT rpm BETA1"),tr("RPM package manager BETA1.\n"
			"Module for SCT.\ndevelopment: sksoft@i.ua"));
}


/*!
    \fn rpm_ui::helpIndex()
 */
void rpm_ui::helpIndex()
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
        
  s+="/doc/"+get_locale()+"/sct_rpm.html";
  
  proc.addArgument(s);
  
  if(!proc.start())
    QMessageBox::information(this,tr("Info"),tr("Can't find help system."));
}


/*!
    \fn rpm_ui::load_menu()
 */
void rpm_ui::load_menu()
{
  menu_rpm = new QPopupMenu(this);
  QPopupMenu *menu_view = new QPopupMenu(this);
  QPopupMenu *menu_help = new QPopupMenu(this);
  
  menuBar()->insertItem(tr("Rpm"), menu_rpm);
  menuBar()->insertItem(tr("View"), menu_view);
  menuBar()->insertItem(tr("Help"), menu_help);
 		
	QActionGroup *mode_group = new QActionGroup( this );
	mode_group->setExclusive( TRUE );	
	installed_act = new QAction(tr("View installed packages"),tr("Installed packages"),
					0, mode_group,tr("View installed packages"));
	installed_act->setToggleAction(TRUE);
	installed_act->addTo(menu_view);
	uninstalled_act = new QAction(tr("View not installed packages"),tr("Not installed packages")
			,0, mode_group,tr("View not installed packages"));
	uninstalled_act->setToggleAction(TRUE);
	uninstalled_act->addTo(menu_view);
	connect(mode_group, SIGNAL( selected(QAction*) ), this, SLOT(slot_mode(QAction*)));
	menu_view->insertSeparator();
	reload_act = new QAction(tr("Reload database rpm"),tr("Reload"),0, mode_group,tr("Reload database rpm"));
	reload_act->addTo(menu_view);
	connect(reload_act, SIGNAL(activated ()), rpm_main, SLOT(slot_updateTree()));
		
	//menu_rpm->insertSeparator();
	menu_rpm->insertItem(tr("Install"), rpm_main, SLOT(slot_install()));
	
	find_act = new QAction(tr("Find"),0, this);
	find_act->setToggleAction(TRUE);
	find_act->addTo(menu_rpm);
	find_act->setToolTip(tr("Find package or file"));
	connect(find_act, SIGNAL(toggled(bool )), rpm_main, SLOT(slot_find(bool )));
	
	menu_rpm->insertItem(tr("Edit sources"), rpm_main, SLOT(slot_edit_src()),0,12);
	menu_rpm->insertSeparator();
	menu_rpm->insertItem(tr("Exit"), this, SLOT(fileExit()));
   
        menu_help->insertItem(tr("Index..."), this, SLOT(helpIndex()));
        menu_help->insertSeparator(1);
        menu_help->insertItem(tr("About"),this, SLOT(helpAbout()));
			
	//one panel
	irpmTools = new QToolBar(this);
	irpmTools->setLabel(tr("Rpm remove operations"));
	//irpmTools->setMovingEnabled(FALSE);
	
	QIconSet u_pix, i_pix, f_pix, e_pix;
		
	u_pix.setPixmap(pixUnist,QIconSet::Automatic);
	i_pix.setPixmap(pixIns,QIconSet::Automatic);
	f_pix.setPixmap(pixFind,QIconSet::Automatic);
	e_pix.setPixmap(pixEditSrc,QIconSet::Automatic);
				
	if(uid == 0)
	{	
		rem_act = new QToolButton( u_pix, tr("Remove packages"), QString::null,
		 rpm_main, SLOT(slot_notinstall()), irpmTools, "Remove packages" );
		rem_act->setTextLabel(tr("Remove"),TRUE);
		set_par(rem_act);
		
                find_btn = new QToolButton(irpmTools);
		find_btn->setIconSet(f_pix);
		find_btn->setToggleButton(TRUE);
		find_btn->setTextLabel(tr("Find"),TRUE);
		set_par(find_btn);
		connect(find_btn, SIGNAL(toggled(bool )), rpm_main, SLOT(slot_find(bool )));
	
		irpmTools->addSeparator();
	
		edit_src_act = new QToolButton( e_pix, tr("Edit sources"), QString::null,
																	rpm_main, SLOT(slot_edit_src()),
																	irpmTools, "Edit sources" );
			edit_src_act->setTextLabel(tr("Edit source"),TRUE);
			set_par(edit_src_act);
	
			QToolTip::add( edit_src_act, tr("Edit sources packages"));
			QToolTip::add( rem_act, tr("Remove selected packages"));
		}
		else
		{
			find_btn = new QToolButton(irpmTools);
			find_btn->setIconSet(f_pix);
			find_btn->setToggleButton(TRUE);
			find_btn->setTextLabel(tr("Find"),TRUE);
			set_par(find_btn);
			connect(find_btn, SIGNAL(toggled(bool )), rpm_main, SLOT(slot_find(bool )));
		}
		QToolTip::add( find_btn, tr("Find package or file"));
		connect(find_act, SIGNAL(toggled(bool )), find_btn, SLOT(setOn(bool )));
		connect(find_btn, SIGNAL(toggled(bool )), find_act, SLOT(setOn(bool )));
		
	//two panel
	urpmTools = new QToolBar( this);
	urpmTools->setLabel(tr("Rpm install operations"));
	//urpmTools->setMovingEnabled(FALSE);
	
	if(uid == 0)
	{	
		inst_act = new QToolButton( i_pix, tr("Install packages"), QString::null,
                           rpm_main, SLOT(slot_install()), urpmTools, "Install packages" );
			inst_act->setTextLabel(tr("Install"),TRUE);
			set_par(inst_act);
		
			find_btn = new QToolButton(urpmTools);
			find_btn->setIconSet(f_pix);
			find_btn->setToggleButton(TRUE);
			find_btn->setTextLabel(tr("Find"),TRUE);
			set_par(find_btn);
			connect(find_btn, SIGNAL(toggled(bool )), rpm_main, SLOT(slot_find(bool )));
	
			urpmTools->addSeparator();
	
			edit_src_act = new QToolButton( e_pix, tr("Edit sources"), QString::null,
                            rpm_main, SLOT(slot_edit_src()), urpmTools, "Edit sources" );
			edit_src_act->setTextLabel(tr("Edit sources"),TRUE);
			set_par(edit_src_act);
			
			QToolTip::add( inst_act, tr("Install selected packages"));
			QToolTip::add( edit_src_act, tr("Edit sources packages"));
	}
	else
	{
		find_btn = new QToolButton(urpmTools);
		find_btn->setIconSet(f_pix);
		find_btn->setToggleButton(TRUE);
		find_btn->setTextLabel(tr("Find"),TRUE);
		set_par(find_btn);
		connect(find_btn, SIGNAL(toggled(bool )), rpm_main, SLOT(slot_find(bool )));
	}
	
	
	QToolTip::add( find_btn, tr("Find package or file"));
	connect(find_act, SIGNAL(toggled(bool )), find_btn, SLOT(setOn(bool )));
	connect(find_btn, SIGNAL(toggled(bool )), find_act, SLOT(setOn(bool )));
}



/*!
    \fn rpm_ui::slot_mode(QAction* )
 */
void rpm_ui::slot_mode(QAction* act)
{
	if ( act == installed_act ) {
		b_installed = true;
	}
	else if ( act == uninstalled_act ) {
		b_installed = false;
	}
	
	set_mode();
}


/*!
    \fn rpm_ui::set_mode() set mode installed or nor not installed
 */
void rpm_ui::set_mode()//set mode installed or nor not installed
{
	if(b_installed)
	{
		menu_rpm->insertItem(tr("Remove"), rpm_main, SLOT(slot_notinstall()), 0,11, 0);
		
		if(uid != 0)
		  menu_rpm->setItemEnabled(11, FALSE);
		
  	menu_rpm->removeItemAt(1);
		installed_act->setOn(TRUE);
		urpmTools->hide();
		irpmTools->show();
		rpm_main->build_tree();
	}
	else
	{
		menu_rpm->insertItem(tr("Install"), rpm_main, SLOT(slot_install()), 0, 13, 0);
		
		if(uid != 0)
		   menu_rpm->setItemEnabled(13, FALSE);
		
		menu_rpm->removeItemAt(1);
		uninstalled_act->setOn(TRUE);
		irpmTools->hide();
		urpmTools->show();
		rpm_main->build_tree(false);
	}
}


/*!
    \fn rpm_ui::loadIcons()
 */
bool rpm_ui::loadIcons()
{
	bool b = true;
	QString path = qApp->applicationDirPath();
	setAplDir(path);
	
	path += "pixmaps/";
		
	pixIns.load(path+"addrpm.png");
	  
	if(pixIns.isNull())
		b = false;
	  
	pixUnist.load(path+"remrpm.png");
	if(pixUnist.isNull())
		b = false;
   
	pixFind.load(path+"find.png");
	if(pixFind.isNull())
		b = false;
  
	pixEditSrc.load(path+"src.png");
	if(pixEditSrc.isNull())
		b = false;
	
	return b;
}


/*!
    \fn rpm_ui::set_par(QToolButton* )
 */
void rpm_ui::set_par(QToolButton* btn)
{
	if(btn->textLabel()==tr("Find"))
	{
		btn->setToggleButton(TRUE);
	}
	//btn->setUsesBigPixmap(TRUE);
	btn->setUsesTextLabel(TRUE);
	btn->setTextPosition(QToolButton::BesideIcon);
}




/*!
    \fn rpm_ui::slotMessageStatus(const QString& message)
 */
void rpm_ui::slotMessageStatus(const QString& message)
{
	statusBar()->message(message); 
}


/*!
    \fn rpm_ui::get_locale()
 */
QString rpm_ui::get_locale()
{
  QDir d = QDir::home();
  
  QString s = d.absPath();
	
  QSettings settings;
	 
  settings.removeSearchPath( QSettings::Unix, s+"/.qt");
  settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
   
  QString slang = settings.readEntry("/SCT/Language_UI","en"); 
  
  return slang;
}
