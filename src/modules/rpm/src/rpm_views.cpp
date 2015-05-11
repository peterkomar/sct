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

#include "rpm_views.h"
#include "selectsourceimpl.h"
#include "add_rem_dlg.h"

#include <qlabel.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qobjectlist.h>
#include <qpopupmenu.h>
#include <qheader.h>
#include <qregexp.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qfont.h>

#include <qprogressdialog.h>
#include <qimage.h>

#include "../pixmaps/item.xpm"
#include "../pixmaps/item_ch.xpm"

// -----------------------------------------------------------------

RPM_Views::RPM_Views( QWidget *parent, const char *name )
    : QSplitter( Qt::Horizontal, parent, name )
    ,b_mode_inst_pkgd(true)
{
	
	load_icons();
	
	//rpm groups
	sd = new QSplitter(Qt::Vertical, this);
	group_list = new QListView( sd/*this*/ );
	group_list->header()->setClickEnabled( FALSE );
	group_list->addColumn(tr("Groups rpm"));
	//QFont sf( "Times", 12, QFont::Bold );
	//group_list->setFont(sf);
	//group_list->setTreeStepSize(50);
	group_list->setRootIsDecorated( TRUE );
	group_list->setColumnWidthMode( 0, QListView::Maximum);
	group_list->setResizeMode( QListView::AllColumns);
	sd->setResizeMode( group_list, QSplitter::KeepSize );
	
	find_dlg = new finddlgImpl(sd);
	find_dlg->hide();
	
	l_panel = new SCT_Left_Panel(sd);
	sd->setResizeMode( l_panel, QSplitter::KeepSize );
		
	vsplitter = new QSplitter( Qt::Vertical, this );
		
	//rpm packages
		rpm_list = new QListView( vsplitter );
		rpm_list->addColumn("C");
		rpm_list->addColumn(tr("Name"));
		rpm_list->addColumn(tr("Description"));
		rpm_list->addColumn(tr("Version"));
		rpm_list->addColumn(tr("Platform"));
		rpm_list->setAllColumnsShowFocus( TRUE );
    //rpm_list->setShowSortIndicator( TRUE );
		rpm_list->setResizeMode( QListView::LastColumn );
		rpm_list->setResizePolicy( QScrollView::Manual );
		rpm_list->setVScrollBarMode( QListView::Auto );
		rpm_list->setHScrollBarMode( QListView::Auto );
		rpm_list->setColumnWidthMode( 0, QListView::Maximum);
		QHeader *h  = rpm_list->header();
		h->setResizeEnabled(FALSE, 0);
		//h->setStretchEnabled(TRUE);
		//h->adjustHeaderSize();
		//palette
		QPalette pal( rpm_list->palette() );

		pal.setColor( QColorGroup::Highlight,rpm_list->palette().active().base() );
		//pal.setColor( QColorGroup::Base,rpm_list->palette().active().background() );
		pal.setColor( QColorGroup::HighlightedText,rpm_list->palette().active().text() );
		rpm_list->setPalette( pal );
		
				
		vsplitter->setResizeMode( rpm_list, QSplitter::KeepSize );

		b_panel = new QInfoPanel(vsplitter);

    
		//rpm_list->setSelectionMode( QListView::Extended );
    // some preparations
    QValueList<int> lst;
    lst.append( 270);
    setSizes( lst );
	QValueList<int> lst1;
	lst1.append( 380 );
	vsplitter->setSizes(lst1);
	QValueList<int> lst2;
	lst2.append( 550 );
	sd->setSizes(lst2);
		
	setMargin( 5 );
		//setHandleWidth(14);
		
	manager = new rpm_manager(this);
	rpm_dlg = NULL;
        //build_tree();
	init_conections();
}



/*!
    \fn RPM_Views::slot_find()
 */
void RPM_Views::slot_find(bool b)
{
	find_dlg->setShown(b);
	group_list->setShown(!b);
}


/*!
    \fn RPM_Views::slot_edit_src()
 */
void RPM_Views::slot_edit_src()
{
	selectsourceImpl *dlg = new selectsourceImpl(this);
	
	dlg->exec();
	
	delete dlg;
	
	if(!b_mode_inst_pkgd)
		build_tree(false);
	calculate_status_panel();
}


/*!
    \fn RPM_Views::slot_install()
 */
void RPM_Views::slot_install()
{
	
	if(listRPM.isEmpty())
		return;
	
	rpm_dlg = new Add_Rem_dlg(this);
	
	init_local_connections();
			
	emit sendMessageStatus(tr("Preparing install ..."));
	qApp->processEvents();
	
	
	rpm_dlg->set_add_mode(true);
	
	rpm_dlg->showMaximized();
	qApp->processEvents();
	
	manager->preparing_rpm(listRPM,true);
			
	emit sendMessageStatus(tr("Ready"));
}


/*!
    \fn RPM_Views::slot_notinstall()
 */
void RPM_Views::slot_notinstall()
{
	if(listRPM.isEmpty())
		return;
	
	rpm_dlg = new Add_Rem_dlg(this);
	
	init_local_connections();
			
	emit sendMessageStatus(tr("Preparing remove ..."));
	qApp->processEvents();
	
	
	rpm_dlg->set_add_mode(false);
	
	rpm_dlg->showMaximized();
	qApp->processEvents();
	
	manager->preparing_rpm(listRPM,false);
		
			
	emit sendMessageStatus(tr("Ready"));
}


/*!
    \fn RPM_Views::load_icons()
 */
bool RPM_Views::load_icons()
{
	bool b = true;
	QString path = qApp->applicationDirPath();
	setAplDir(path);
	
	path += "pixmaps/";
	pix_p.load(path+"package.png");
	if(pix_p.isNull())
		b = false;
	
	pix_p_apl.load(path+"p_application.png");
	if(pix_p_apl.isNull())
		b = false;
	
	pix_p_fav.load(path+"p_favorite.png");
	if(pix_p_fav.isNull())
		b = false;
	
	pix_p_games.load(path+"p_games.png");
	if(pix_p_games.isNull())
		b = false;
	
	pix_p_multimedia.load(path+"p_multimedia.png");
	if(pix_p_multimedia.isNull())
		b = false;
	
	pix_p_network.load(path+"p_network.png");
	if(pix_p_network.isNull())
		b = false;
	
	pix_p_settings.load(path+"p_settings.png");
	if(pix_p_settings.isNull())
		b = false;
	
	pix_p_toys.load(path+"p_toys.png");
	if(pix_p_toys.isNull())
		b = false;
	
	pix_p_util.load(path+"p_utilities.png");
	if(pix_p_util.isNull())
		b = false;
	
	pix_rpm.load(path+"rpm_p.png");
	if(pix_rpm.isNull())
		b = false;
	
	pix_p_development.load(path+"p_development.png");
	if(pix_p_development.isNull())
		b = false;
	
	pix_p_editors.load(path+"p_editors.png");
	if(pix_p_editors.isNull())
		b = false;
	
	pix_p_edutainment.load(path+"p_edutainment.png");
	if(pix_p_edutainment.isNull())
		b = false;
	
	pix_p_games_arcade.load(path+"p_games_arcade.png");
	if(pix_p_games_arcade.isNull())
		b = false;
	
	pix_p_games_board.load(path+"p_games_board.png");
	if(pix_p_games_board.isNull())
		b = false;
	
	pix_p_games_card.load(path+"p_games_card.png");
	if(pix_p_games_card.isNull())
		b = false;
	
	pix_p_games_strategy.load(path+"p_games_strategy.png");
	if(pix_p_games_strategy.isNull())
		b = false;
	
	pix_p_misc.load(path+"p_misc.png");
	if(pix_p_misc.isNull())
		b = false;
	
	pix_p_security.load(path+"p_security.png");
	if(pix_p_security.isNull())
		b = false;
	
	pix_p_shells.load(path+"p_shells.png");
	if(pix_p_shells.isNull())
		b = false;
	
	pix_p_system.load(path+"p_system.png");
	if(pix_p_system.isNull())
		b = false;
	
	pix_p_graphics.load(path+"p_graphics.png");
	if(pix_p_graphics.isNull())
		b = false;
	  
	return b;
}


/*!
    \fn RPM_Views::set_group_icon(QString& name)
 */
QPixmap RPM_Views::set_group_icon(QString& name)
{
	if(name == "Utilities")
	{
		//name = tr("Utilities");
		return pix_p_util;
	}
	
	if(name == "Multimedia")
	{
		//name = tr("Multimedia");
		return pix_p_multimedia;
	}
	
	if(name == "Games")
	{
		//name = tr("Games");
		return pix_p_games;
	}
	
	if(name == "Applications")
	{
		//name = tr("Applications");
		return pix_p_apl;
	}
	
	if(name == "Toys")
	{
		//name = tr("Toys");
		return pix_p_toys;
	}
	
	if(name == "Networking")
	{
		//name = tr("Networking");
		return pix_p_network;
	}
	
	if(name == "System")
	{
		//name = tr("System");
		return pix_p_system;
	}
	
	if(name == "Misc")
	{
		//name = tr("Misc");
		return pix_p_misc;
	}
	
	if(name == "Action")
	{
		//name = tr("Action");
		return pix_p;
	}
	
	if(name == "Amusements")
	{
		//name = tr("Amusements");
		return pix_p;
	}
	
	if(name == "Board")
	{
		//name = tr("Board");
		return pix_p_games_board;
	}
	
	if(name == "Startegy")
	{
		//name = tr("Startegy");
		return pix_p_games_strategy;
	}
	
	if(name == "Arcade")
	{
		//name = tr("Arcade");
		return pix_p_games_arcade;
	}
	
	if(name == "Card")
	{
		//name = tr("Card");
		return pix_p_games_card;
	}
	
	
	if(name == "Screensavers")
	{
		//name = tr("Screensavers");
		return pix_p;
	}
	
	if(name == "Emulators")
	{
		//name = tr("Emulators");
		return pix_p;
	}
	
	if(name == "Development")
	{
		//name = tr("Development");
		return pix_p_development;
	}
	
	if(name == "Languages")
	{
		//name = tr("Languages");
		return pix_p;
	}
	
	if(name == "Libraries")
	{
		//name = tr("Libraries");
		return pix_p;
	}
	
	if(name == "Sources")
	{
		//name = tr("Sources");
		return pix_p;
	}
	
	if(name == "Tools")
	{
		//name = tr("Tools");
		return pix_p_settings;
	}
	
	if(name == "Documentation")
	{
		//name = tr("Documentation");
		return pix_p;
	}
	
	if(name == "Hardware")
	{
		//name = tr("Hardware");
		return pix_p;
	}
	
	if(name == "Mobile")
	{
		//name = tr("Mobile");
		return pix_p;
	}
	
	if(name == "Modem")
	{
		//name = tr("Modem");
		return pix_p;
	}
	
	
	if(name == "Other")
	{
		//name = tr("Other");
		return pix_p;
	}
	
	if(name == "Palm")
	{
		//name = tr("Palm");
		return pix_p;
	}
	
	if(name == "Printing")
	{
		//name = tr("Printing");
		return pix_p;
	}
	
	if(name == "Scaner")
	{
		//name = tr("Scaner");
		return pix_p;
	}
	
	if(name == "Productivity")
	{
		//name = tr("Productivity");
		return pix_p;
	}
	
	if(name == "Archiving")
	{
		//name = tr("Archiving");
		return pix_p;
	}
	
	if(name == "Databases")
	{
		//name = tr("Databases");
		return pix_p;
	}
	
	if(name == "Editors")
	{
		//name = tr("Editors");
		return pix_p_editors;
	}
	
	if(name == "File utilities")
	{
		//name = tr("File utilities");
		return pix_p;
	}
	
	if(name == "Graphics")
	{
		//name = tr("Graphics");
		return pix_p_graphics;
	}
	
	if(name == "Office")
	{
		//name = tr("Office");
		return pix_p;
	}
	
	if(name == "Publishing")
	{
		//name = tr("Publishing");
		return pix_p_edutainment;
	}
	
	if(name == "Scientific")
	{
		//name = tr("Scientific");
		return pix_p;
	}
	
	if(name == "Security")
	{
		//name = tr("Security");
		return pix_p_security;
	}
	
	if(name == "Telephony")
	{
		//name = tr("Telephony");
		return pix_p;
	}
	
	if(name == "Text")
	{
		//name = tr("Text");
		return pix_p;
	}
	
	if(name == "Base")
	{
		//name = tr("Base");
		return pix_p;
	}
	
	if(name == "Boot")
	{
		//name = tr("Boot");
		return pix_p;
	}
	
	if(name == "Console")
	{
		//name = tr("Console");
		return pix_p_shells;
	}
	
	if(name == "Daemons")
	{
		//name = tr("Daemons");
		return pix_p;
	}
	
	if(name == "Filesystems")
	{
		//name = tr("Filesystems");
		return pix_p;
	}
	
	if(name == "Kernel")
	{
		//name = tr("Kernel");
		return pix_p;
	}
	
	if(name == "Management")
	{
		//name = tr("Management");
		return pix_p;
	}
	
	if(name == "Monitoring")
	{
		//name = tr("Monitoring");
		return pix_p;
	}
	
	if(name == "Packages")
	{
		//name = tr("Packages");
		return pix_p;
	}
	
	if(name == "Shells")
	{
		//name = tr("Shells");
		return pix_p_shells;
	}
	
	if(name == "Sound Daemons")
	{
		//name = tr("Sound Daemons");
		return pix_p;
	}
	
	if(name == "Displaymangers")
	{
		//name = tr("Displaymangers");
		return pix_p;
	}
	
	if(name == "Fonts")
	{
		//name = tr("Fonts");
		return pix_p;
	}
	
	if(name == "Icons")
	{
		//name = tr("Icons");
		return pix_p;
	}
	
	if(name == "Servers")
	{
		//name = tr("Servers");
		return pix_p;
	}
	
	if(name == "Terminals")
	{
		//name = tr("Terminals");
		return pix_p_shells;
	}
		
	
	return pix_p;
}


/*!
    \fn RPM_Views::build_tree()
 */
void RPM_Views::build_tree(bool b)
{
	emit sendMessageStatus(tr("Building tree group packages ..."));
	b_mode_inst_pkgd = b;
	rpm_list->clear();
	listRPM.clear();
	group_list->clear();
	
	QApplication::setOverrideCursor( waitCursor );
	
	//QColor col_txt;
	
	//QPixmap splsh_pix = create_spash_dialog(col_txt);
	
	//scr = new QSplashScreen(splsh_pix);
	scr = new QProgressDialog("Please wait. Getting data base RPM ....", "Abort get", 100,
				  this, "rpm", TRUE);
	scr->setCaption("Query rpm");
	scr->setCancelButton(NULL);
	scr->setAutoClose(TRUE);
	scr->setProgress( 1 );
	qApp->processEvents();
	
	//while(1);
	
  QPtrList<infoRpm> listRpm;
  if(b)
	{
	 manager->get_i_packages();
	 listRpm = manager->getListRpm();
	}
	else
	{
                manager->get_i_packages();
                manager->get_u_packages();
		listRpm = manager->getListURpm();
	}
	QListViewItem* item=NULL;
	if(!listRpm.isEmpty())
	{  
	infoRpm* data=NULL;
	QListViewItem *i=NULL;
	QStringList Groups;
	int k=0;
	data = (infoRpm*)listRpm.first();  
	QString tmp;
	QPixmap pix;
	
	scr->setLabelText(tr("Please wait. Building group tree RPM ...."));
	qApp->processEvents();
	
		while(data!=NULL)
		{
			Groups.clear();
					
			//QMessageBox::about(this, "sd", data->group);
              
			Groups=QStringList::split("/",data->group);
                  
			for ( QStringList::Iterator it = Groups.begin(); it != Groups.end(); ++it )
			{
				i = findItem(*it,item,0,group_list);
				if(i==NULL){
        
					if(k==0)
					{
						tmp = *it;
						pix = set_group_icon(tmp);
						item = new QListViewItem(group_list,tmp);
						item->setPixmap(0,pix);
						k++; 
					}
					else{
						tmp = *it;
					  pix = set_group_icon(tmp);
						item = new QListViewItem(item,tmp);
						item->setPixmap(0,pix);
					}
				}
			  else{
					item = i;
					k++;
				}
			}
			k=0;
			item = NULL;
			data = (infoRpm*)listRpm.next();
	 }
	 
	 item = new QListViewItem(group_list,tr("z All"));
	 item->setPixmap(0,pix_p);
	  
	}
	
	item = group_list->firstChild();
	if(item!= NULL);
	  group_list->setSelected(item, TRUE);
	
	QApplication::restoreOverrideCursor();
        calculate_status_panel();
	//scr->finish(this);
	delete scr;
        
        if(listRpm.isEmpty())
          QMessageBox::information(this, tr("Empty"),tr("No packages to install. Please select Rpm/Edit sources to add rpm source."));
	
	emit sendMessageStatus(tr("Ready"));
}


/*!
    \fn RPM_Views::findItem(QString name, QListViewItem* item)
 */
QListViewItem* RPM_Views::findItem(QString name, QListViewItem* item, int c , QListView* parent)
{
	QListViewItem* i=NULL;
	if(item == NULL)
		i = parent->firstChild();
	else
		i = item->firstChild();
	while( i ) {
		if(i->text(c) == name)
		{
			return i;
		}
		i = i->nextSibling();
	}
	return NULL;
}


/*!
    \fn RPM_Views::slot_sel_group(QListViewItem * item )
 */
void RPM_Views::slot_sel_group(QListViewItem * item )
{
    if(item == NULL)
			return;
		rpm_list->clear();
		
		QApplication::setOverrideCursor( waitCursor );
		
		//QHeader* h = rpm_list->header();
		
		QString str, s;
		QListViewItem *i_tmp = item->parent();
		str = item->text(0);
		
		while( i_tmp != NULL)
		{
			s = i_tmp->text(0);
			
			str.insert(0,s+"/");
			
			i_tmp = i_tmp->parent();
    }
		
		QPtrList<infoRpm> listRpm;
		if(b_mode_inst_pkgd)
			listRpm = manager->getListRpm();
		else
			listRpm = manager->getListURpm();
  
		infoRpm* data=NULL;
						
		data = (infoRpm*)listRpm.first();
			
		while(data!=NULL)
		{
			if((data->group.find(str) == 0) || (item->text(0) == tr("z All")))
			{
				i_tmp = new QListViewItem(rpm_list, "",data->name, data->summary,data->version+"-"+data->release,data->arch);
				i_tmp->setPixmap(1,pix_rpm);
												
				if(!find_in_list(data->name+data->version+"-"+data->release+data->arch))
					i_tmp->setPixmap(0,QPixmap(item_xpm));
				else
				  i_tmp->setPixmap(0, QPixmap(item_ch_xpm));
			}
			
			data = (infoRpm*)listRpm.next();
		}
		
		item = rpm_list->firstChild();
		if(item!= NULL);
		rpm_list->setSelected(item, TRUE);
		QApplication::restoreOverrideCursor();
}


/*!
    \fn RPM_Views::init_conections()
 */
void RPM_Views::init_conections()
{
connect(group_list, SIGNAL(clicked( QListViewItem* )),this, SLOT(slot_sel_group( QListViewItem* )));
connect(group_list, SIGNAL(selectionChanged( QListViewItem* )),this, SLOT(slot_sel_group( QListViewItem* )));
connect(rpm_list, SIGNAL(clicked( QListViewItem*, const QPoint&, int )),
				this,SLOT(slot_clicked( QListViewItem*, const QPoint&, int )));
connect(rpm_list, SIGNAL(selectionChanged( QListViewItem* )),this,
				SLOT(slot_select( QListViewItem* )));
connect(rpm_list, SIGNAL(spacePressed( QListViewItem* )),this,
				SLOT(slotPressSpace( QListViewItem* )));
connect(find_dlg, SIGNAL(sendName(find_struct )), this, SLOT(slot_get_find(find_struct )));
connect(manager, SIGNAL(send_parcent( int ,int )), this, SLOT(get_parcent( int , int )));

}


/*!
    \fn RPM_Views::slot_clicked( QListViewItem * item, const QPoint & pnt, int c )
 */
void RPM_Views::slot_clicked( QListViewItem * item, const QPoint& , int c )
{
	if(item == NULL)
		return;
		
	QApplication::setOverrideCursor( waitCursor );
	QString s = item->text(1)+item->text(3)+item->text(4);;
	
	if(!find_in_list(s))
	{
        if(c == 0)
	{
			item->setPixmap(0,QPixmap(item_ch_xpm));
			
			listRPM += s;
			calculate_status_panel();
		}
	}
	else
	{
		if(c == 0)
		{
			item->setPixmap(0,QPixmap(item_xpm));
			
			listRPM.remove(s);
			calculate_status_panel();
		}
	}
	infoRpm* data = get_data_rpm(s);
	b_panel->show_info_package(*data);
	QApplication::restoreOverrideCursor();
}


/*!
    \fn RPM_Views::find_in_list(const QString& s)
 */
bool RPM_Views::find_in_list(const QString& s)
{
	for ( QStringList::Iterator it = listRPM.begin(); it != listRPM.end(); ++it ) {
		if(*it == s)
			return true;
	}
	
	return false;
}


/*!
    \fn RPM_Views::get_data_rpm(const QString& )
 */
infoRpm* RPM_Views::get_data_rpm(const QString& s)
{
	QPtrList<infoRpm> listRpm;
	if(b_mode_inst_pkgd)
		listRpm = manager->getListRpm();
	else
		listRpm = manager->getListURpm(true);
  
	infoRpm* data=NULL;
						
	data = (infoRpm*)listRpm.first();  
			
	while(data!=NULL)
	{
		if(data->name+data->version+"-"+data->release+data->arch == s)
			return data;
		data = (infoRpm*)listRpm.next();
	}
        
        return NULL;
}


/*!
    \fn RPM_Views::slot_get_find(QString ,bool )
 */
void RPM_Views::slot_get_find(find_struct f_find)
{
	rpm_list->clear();
	QPtrList<infoRpm> listRpm;
	if(b_mode_inst_pkgd)
		listRpm = manager->getListRpm();
	else
		listRpm = manager->getListURpm();
		
  
		infoRpm* data=NULL;
		bool fin=false;
						
		data = (infoRpm*)listRpm.first(); 
		QListViewItem *i_tmp=NULL; 
			
		QString name, summ, desk;
		
		bool b_file = f_find.b_file;
		QString name1 = f_find.name;
		bool b_absolutely	= f_find.b_absol;
		bool b_cmp_case	 = f_find.b_case;
		bool b_name = f_find.b_name;
		bool b_description = f_find.b_descrip;
		bool b_summary = f_find.b_summary;
		
		QPtrList<infoRpm> list_F;
		
    
		if(!b_file)
		{             
				while(data!=NULL)
				{
					name="";
					summ="";
					desk="";
					
					//find by name
					if(b_name)
						name = data->name+data->version+"-"+data->release;
					if(b_summary)
						summ = data->summary;
					if(b_description)
						desk = data->Description;
						
					if(cmp_str(name1, name, b_cmp_case, b_absolutely))
					{
						list_F.append(data);
						fin = true;
					}
					else
						if(cmp_str(name1, summ, b_cmp_case, b_absolutely))
					  {
						  list_F.append(data);
							fin = true;
						}
						else
							if(cmp_str(name1, desk, b_cmp_case, b_absolutely))
						  {
							  list_F.append(data);
							  fin = true;
						  }
					data = (infoRpm*)listRpm.next();
				}
		}
		else
		{
			//findes files in packages
			QStringList filesList;
			while(data!=NULL)
			{
			//name = data->name+"-"+data->version+"-"+data->release;
				QString file;
                                if(b_mode_inst_pkgd)
				  filesList = data->files;
                                else
                                  filesList = data->names_files;
                                
      				for ( QStringList::Iterator it = filesList.begin(); it != filesList.end(); ++it )
				{
					file = *it;
					if(b_cmp_case)
					{
						if(b_absolutely)
						{
							if(!file.compare(name1))
							{
								list_F.append(data);
								fin=true;
							}
						}
						else{  
					    if(file.find(name1,0,FALSE)!=-1)
					    {
						    list_F.append(data);
						   fin=true;
					    }
						}
					}
					else
					{
						if(b_absolutely)
						{
							if(file==name1)
							{
		 					list_F.append(data);
								fin=true;
							}
						}
						else{  
							if(file.find(name1)!=-1)
							{
								list_F.append(data);
								fin=true;
							}
						}
					}
				}
				data = (infoRpm*)listRpm.next();
			}
		}
			
			//list_F.setAutoDelete(TRUE);
			data = list_F.first();
						
			while(data!=NULL)
			{
				QListViewItem* mt=NULL;
				
				mt=findItem(data->name,NULL,1,rpm_list);
				if(mt==NULL){
					i_tmp = new QListViewItem(rpm_list, "",data->name, data->summary,data->version+"-"+data->release, data->arch);
				i_tmp->setPixmap(1,pix_rpm);
								
					if(!find_in_list(data->name))
							i_tmp->setPixmap(0,QPixmap(item_xpm));
					else
						i_tmp->setPixmap(0, QPixmap(item_ch_xpm));
					}
					data = (infoRpm*)list_F.next();
			}
		
		if(!fin){
			new QListViewItem(rpm_list, "",tr("Not found"));   
			return;
		}
}


/*!
    \fn RPM_Views::slot_select(QListViewItem* )
 */
void RPM_Views::slot_select(QListViewItem* item)
{
	if(item == NULL)
		return;
	
	QApplication::setOverrideCursor( waitCursor );
	infoRpm* data = get_data_rpm(item->text(1)+item->text(3)+item->text(4));
	b_panel->show_info_package(*data);
	QApplication::restoreOverrideCursor();
}


/*!
    \fn RPM_Views::slotPressSpace(QListViewItem* )
 */
void RPM_Views::slotPressSpace(QListViewItem* item)
{
	if(item == NULL)
		return;
	
	QString s = item->text(1)+item->text(3)+item->text(4);;
		
	if(!find_in_list(s))
	{
			item->setPixmap(0,QPixmap(item_ch_xpm));
			listRPM += s;
		
	}
	else
	{
		item->setPixmap(0,QPixmap(item_xpm));
		listRPM.remove(s);
	}
}


/*!
    \fn RPM_Views::slot_updateTree()
 */
void RPM_Views::slot_updateTree()
{
	build_tree(b_mode_inst_pkgd);
}


/*!
    \fn RPM_Views::create_spash_dialog()
 */
/*
QPixmap RPM_Views::create_spash_dialog(QColor& col_text)
{
	
	QPixmap pix(400,50);
	//pix.fill(QColor(243,255,231));
	
	QPalette pal(qApp->palette());

	QColor col_bag = pal.color(QPalette::Active,QColorGroup::Background);
	
	pix.fill(col_bag);
	QImage img;
	img = pix;
	
	int i;
	
	QColor col = pal.color(QPalette::Active,QColorGroup::Shadow);
	QColor col_txt = pal.color(QPalette::Active, QColorGroup::Text);
	 
	int f_r = col_bag.red()-40;
	int f_g = col_bag.green()-40;
	int f_b = col_bag.blue()-40;
	
	if((f_r<0) || (f_g<0) || (f_b<0))
	{
		f_r = col_bag.red();
		f_g = col_bag.green();
		f_b = col_bag.blue();
	}
	
	for(int y=0;y<img.height();y++) //left, rights
	{
		if(y<(img.height()-2))
		{
			img.setPixel(0,y,qRgb(f_r,f_g,f_b));
			
			img.setPixel(img.width()-3,y,qRgb(f_r,f_g,f_b));
		}
		
		img.setPixel(img.width()-1,y,col.rgb());
		img.setPixel(img.width()-2,y,col.rgb());
	}
		
	for(int x=0;x<img.width();x++) //top, bottom
	{
		if(x<(img.width()-2))
		{
			img.setPixel(x,0,qRgb(f_r,f_g,f_b));
			
			img.setPixel(x,1,qRgb(75,121,200));
			img.setPixel(x,2,qRgb(75,121,200));
			img.setPixel(x,3,qRgb(75,121,200));
			img.setPixel(x,4,qRgb(75,121,200));
			img.setPixel(x,5,qRgb(75,121,200));
			img.setPixel(x,6,qRgb(75,121,200));
					
			img.setPixel(x, img.height()-3,qRgb(f_r,f_g,f_b));
		}	
		
		img.setPixel(x, img.height()-1,col.rgb());
		img.setPixel(x,img.height()-2,col.rgb());
	}
	
	pix.convertFromImage(img);
	
	return pix;
}
*/

/*!
    \fn RPM_Views::get_parcent(int )
 */
void RPM_Views::get_parcent(int i, int p_count)
{
	//QPalette pal(qApp->palette());
	//QColor col_txt = pal.color(QPalette::Active, QColorGroup::Text);
	
	if(scr->isShown())
	{
	   scr->setLabelText(tr("Please wait. Get records RPM .... read: %1 reacords in %2").arg(i).arg(p_count));
	   scr->setProgress(i, p_count);
	   qApp->processEvents();
	}
}


/*!
    \fn RPM_Views::calculate_status_panel(bool bremove=true)
 */
void RPM_Views::calculate_status_panel()//show size and count selected packages
{
	int size_p=0;
	
	for ( QStringList::Iterator it = listRPM.begin(); it != listRPM.end(); ++it ) {
		infoRpm* data = get_data_rpm(*it);
		if(data != NULL)
		{
			size_p += data->isize;
		}
	}
	
	QString s = convert_size(size_p);
		
	l_panel->update_info(listRPM.count(), s);
}


/*!
    \fn RPM_Views::slot_finish_rpm_dialog( )
 */
void RPM_Views::slot_finish_rpm_dialog(bool b)
{
	if(rpm_dlg)
	{
	  rem_local_connections();
	  
	  if(!b)
	  {
	    listRPM.clear();
	    calculate_status_panel();
	    build_tree(b_mode_inst_pkgd);
	  }		
	  
	  delete rpm_dlg;
	}
}


/*!
    \fn RPM_Views::init_local_connections()
 */
void RPM_Views::init_local_connections()
{
connect(manager, SIGNAL(first_report(const QString& , bool )), rpm_dlg, SLOT(slot_get_first_report(const QString& ,bool )));
connect(manager, SIGNAL(count_all_items(int )),rpm_dlg,SLOT(slot_count_all_items(int )));
 connect(manager, SIGNAL(current_all_items(int , const QString& )),rpm_dlg, SLOT(slot_current_all_items(int ,const QString& )));
 connect(manager, SIGNAL(send_check_parcent(int, int)), rpm_dlg, SLOT(slot_progress_checked( int, int )));
 connect(manager, SIGNAL(send_message( const QString& )), rpm_dlg, SLOT(slot_message( const QString& )));
 connect(manager, SIGNAL(current_parcent_rpm(int )),rpm_dlg, SLOT(slot_current_parcent_rpm(int )));
 connect(manager, SIGNAL(finish_rpm(const QString& , bool )),rpm_dlg, SLOT(slot_finish_rpm(const QString& , bool )));
 connect(rpm_dlg, SIGNAL(finish_dlg(bool )),this, SLOT(slot_finish_rpm_dialog(bool )));
 connect(rpm_dlg, SIGNAL(terminate_rpm()),manager, SLOT(terminate_process()));
 connect(rpm_dlg, SIGNAL(start_rpm(bool , bool )),manager, SLOT(start_process( bool, bool )));
}


/*!
    \fn RPM_Views::rem_local_connections()
 */
void RPM_Views::rem_local_connections()
{
disconnect(manager, SIGNAL(first_report(const QString& , bool )), rpm_dlg, SLOT(slot_get_first_report(const QString& ,bool )));
disconnect(manager, SIGNAL(count_all_items(int )),rpm_dlg,SLOT(slot_count_all_items(int )));
disconnect(manager, SIGNAL(current_all_items(int , const QString& )),rpm_dlg, SLOT(slot_current_all_items(int ,const QString& )));
disconnect(manager, SIGNAL(send_check_parcent(int, int)), rpm_dlg, SLOT(slot_progress_checked( int, int )));
disconnect(manager, SIGNAL(send_message( const QString& )), rpm_dlg, SLOT(slot_message( const QString& )));
disconnect(manager, SIGNAL(current_parcent_rpm(int )),rpm_dlg, SLOT(slot_current_parcent_rpm(int )));
connect(manager, SIGNAL(finish_rpm(const QString& , bool )),rpm_dlg, SLOT(slot_finish_rpm(const QString& , bool )));
disconnect(rpm_dlg, SIGNAL(finish_dlg(bool )),this, SLOT(slot_finish_rpm_dialog(bool )));
disconnect(rpm_dlg, SIGNAL(terminate_rpm()),manager, SLOT(terminate_process()));
disconnect(rpm_dlg, SIGNAL(start_rpm(bool , bool )),manager, SLOT(start_process( bool, bool )));
}
