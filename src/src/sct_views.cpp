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
#include "sct_views.h"

#include <qiconview.h>
#include <qlistview.h>
#include <qlistbox.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qprocess.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qtextedit.h>
#include <qfile.h>
#include <qsize.h>
#include <qtimer.h>

#include <qtabwidget.h>
#include <qtoolbox.h>

#include <qlayout.h>
//#include <qtextedit.h>


myconfviews::myconfviews(QWidget* parent, const char* name): QVBox(parent, name)
{
  loader = new myQLoader();
  
  listModules = loader->getNamesModules();
  
  if(!load_Icons())
	QMessageBox::critical(this,tr("Error Load"), tr("Can't load icon"));
  create_GUI();
  //createInfoPanel();
  init_actions();
     
  //setMinimumSize(800,450);
}


myconfviews::~myconfviews()
{
  //free memory
  delete g_tools;
  delete tools_full;
  delete tools_short;
  delete l_info;
  delete panel_tools;
  delete panel_group_tools;
  delete loader;
}




/*!
    \fn myconfviews::create_GUI()
 */
void myconfviews::create_GUI()
{
  //setMargin( 5 );
  
  panel_group_tools = new QHBox(this);
  panel_group_tools->setFrameStyle(QFrame::NoFrame/*QFrame::StyledPanel|QFrame::Raised*/);
  panel_group_tools->setPaletteBackgroundColor(QColor("#EDEDEC"));
		  
  create_Left_Panel();
  create_Right_Panel();
	
	  
  loadModulesToRightPanel(g_tools->currentText());
  
  l_info->setText(get_boot_statistic());
}


/*!
    \fn myconfviews::create_Left_Panel()
 */
void myconfviews::create_Left_Panel()
{
  g_tools = new QListBox(panel_group_tools);
  
  g_tools->setFrameStyle(QFrame::NoFrame);
  //g_tools->setCursor(QCursor(PointingHandCursor));
  //g_tools->setBackgroundMode( PaletteDark );
  g_tools->setFixedWidth (200);
  //setFrameStyle (QFrame::StyledPanel|QFrame::Raised);
  
  // change some colors to make it look nice
  QPalette pal( g_tools->palette() );

  pal.setColor( QColorGroup::Highlight,g_tools->palette().active().base() );
  pal.setColor( QColorGroup::Base,QColor("#EDEDEC")/*g_tools->palette().active().background() */);
  pal.setColor( QColorGroup::HighlightedText,g_tools->palette().active().text() );
  g_tools->setPalette( pal );
  
  g_tools->setFocus();
   
  new QListBoxPixmap(g_tools,pixBootL, tr("Boot config"));
 
  new QListBoxPixmap(g_tools,pixMnt,tr("Hard Disk"));
 
  new QListBoxPixmap(g_tools,pixHwBr,tr("Devices"));
 
  new QListBoxPixmap(g_tools,pixConfigNetwork,tr("Network"));
 
  new QListBoxPixmap(g_tools,pixConfigPackades,tr("Software"));
 
  new QListBoxPixmap(g_tools,pixConfig, tr("System"));
  
  //g_tools->setSelected(0,true);
  g_tools->setFocus();
  g_tools->setCurrentItem(0);
  
  g_tools->setMargin(14);
  //g_tools->setPaletteBackgroundColor(Qt::black);
}


/*!
    \fn myconfviews::create_Right_Panel()
 */
void myconfviews::create_Right_Panel()
{
  panel_tools = new QVBox(panel_group_tools);
  
  QToolBox *tab_w = new QToolBox(panel_tools);
              
  
  QWidget* w1 = new QWidget(tab_w);
  QGridLayout *tabLayout1 = new QGridLayout( w1, 1, 1, 11, 6, "tabLayout1"); 
	  
  tools_full = new QListView(w1/*panel_tools*/);
  tools_full->addColumn(tr("Configure Tools"));
  tools_full->addColumn(tr("Information"));
  tools_full->setColumnAlignment( 1, Qt::AlignLeft );
  tools_full->setAllColumnsShowFocus( TRUE );
  tools_full->setShowSortIndicator( TRUE );
  tools_full->setResizeMode(QListView::AllColumns);
  tools_full->setSorting(0,false);
  //tools_full->setPaletteBackgroundPixmap(pixGrad);
  tools_full->setSelectionMode(QListView::NoSelection);
   
  //tools_full->resize(130,100);
  
  tools_short = new QIconView(w1/*panel_tools*/);
  tools_short->setItemsMovable(FALSE);
  
  tools_short->setWordWrapIconText(FALSE);
  tools_short->setArrangement(QIconView::LeftToRight);
  tools_short->setSpacing(35);
  tools_short->setResizeMode(QIconView::Adjust);
  tools_short->setGridX(150);
  tools_short->setGridY(90);
  tools_short->setMaxItemWidth(400);
  //tools_short->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
  
  tabLayout1->addWidget( tools_full, 0, 0 );
  tabLayout1->addWidget( tools_short, 0, 0 );
  
  QSpacerItem *spacer1 = new QSpacerItem( 40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
  tabLayout1->addItem( spacer1, 1, 0 );
  
  //tab_w->insertTab(w1, QString::fromLatin1("") );
  tab_w->addItem(w1,tr( "Tools" ));
  //tab_w->changeTab( w1, tr( "Tools" ) );
  
  QWidget* w2 = new QWidget(tab_w);
  QGridLayout *tabLayout2 = new QGridLayout( w2, 1, 1, 11, 6, "tabLayout2"); 
  
  l_info = new QTextEdit(w2);
  l_info->setReadOnly(TRUE);
  l_info->setWordWrap(QTextEdit::WidgetWidth);
  l_info->setWrapPolicy(QTextEdit::Anywhere);
  l_info->setHScrollBarMode(QScrollView::AlwaysOff);
    //l_info->setFixedHeight(205);
    
  l_info->setFrameShape(QFrame::NoFrame);
  l_info->setPaletteBackgroundColor(QColor("#EDEDEC"));
  //l_info->setMargin(3);
  
  tabLayout2->addWidget( l_info, 0, 0 );
  
  QSpacerItem *spacer2 = new QSpacerItem( 40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
  tabLayout2->addItem( spacer2, 1, 0 );
  
  tab_w->addItem( w2,tr( "System information" ));
  
  w2->setPaletteBackgroundColor(QColor("#EDEDEC"));
  w1->setPaletteBackgroundColor(QColor("#EDEDEC"));
  
  tools_full->setPaletteBackgroundColor(Qt::white);
  tools_short->setPaletteBackgroundColor(Qt::white);
  
  tab_w->setPaletteBackgroundColor(QColor("#EDEDEC"));
  
  //tab_w->setFrameShape(QFrame::NoFrame);
  tab_w->setBackgroundColor(QColor("#EDEDEC"));
}  



/*!
    \fn myconfviews::loadModulesToRightPanel(QString name_g (name group in left panel))
*/
void myconfviews::loadModulesToRightPanel(QString name_g/*name group in left panel*/)
{
  tools_full->clear();
  tools_short->clear();
	  
  QIconViewItem *iv = NULL;
  QListViewItem *i = NULL;
  
  myInfo_modules *p_data=NULL;
  for ( p_data = listModules.first(); p_data; p_data = listModules.next() )
  {
	if(!name_g.compare(p_data->group))
	{
	  i = new QListViewItem(tools_full,p_data->name,p_data->description);
	  i->setPixmap(0,p_data->pixModule);
	  		
	  iv = new QIconViewItem(tools_short,p_data->name, p_data->pixModule);
	  iv->setDragEnabled(FALSE);
	  iv->setDropEnabled(FALSE);
	}
  }
}


/*!
    \fn myconfviews::setFullModeView(bool )
 */
void myconfviews::setFullModeView(bool b)
{
  if(b)
  {
	tools_full->show();
	tools_short->hide();
  }
  else
  {
	tools_full->hide();
	tools_short->show();
  }
}


/*!
    \fn myconfviews::slotClickFullMode(QListViewItem* )
 */
void myconfviews::slotClickFullMode(QListViewItem* item)
{
  if(!item)
	return;
  
  load_Mod_App(item->text(0));
}


/*!
    \fn myconfviews::slotClickLeftPanel(QListBoxItem* )
 */
void myconfviews::slotClickLeftPanel(QListBoxItem* item)
{
  if(!item)
	return;
    
  loadModulesToRightPanel(item->text());
  
  if(item->text() == tr("Boot config"))
  {
    l_info->setText(get_boot_statistic());
  }
  else if(item->text() == tr("Hard Disk"))
  {
    l_info->setText(get_disk_statistic());
  }
  else if(item->text() == tr("Devices"))
  {
    l_info->setText(get_devices_statistic());
  }
  else if(item->text() == tr("Network"))
  {
    l_info->setText(get_network_statistic());
  }
  else if(item->text() == tr("Software"))
  {
    l_info->setText(get_software_statistic());
  }
  else if(item->text() == tr("System"))
  {
    l_info->setText(get_system_statistic());
  }
}


/*!
    \fn myconfviews::slotClickShortMode(QIconViewItem*  )
 */
void myconfviews::slotClickShortMode(QIconViewItem*  item)
{
  if(!item)
	return;
  
  load_Mod_App(item->text());
}


/*!
    \fn myconfviews::load_Mod_App(QString ) //load modules or programs after click mouse button
 */
void myconfviews::load_Mod_App(QString name) //load modules or programs after click mouse button
{
  QApplication::setOverrideCursor( Qt::waitCursor );
  
  emit statusMsg(name);
  
  QTimer::singleShot( 2*1000, this, SLOT(restoreCursor()));
  
  loader->loadModule(name);
}


/*!
    \fn myconfviews::init_actions()
 */
void myconfviews::init_actions()
{
  connect(g_tools, SIGNAL(highlighted(QListBoxItem* )), this, SLOT(slotClickLeftPanel(QListBoxItem* )));
  connect(g_tools, SIGNAL(onItem(QListBoxItem* )), this, SLOT(slotOnItem(QListBoxItem* )));
  connect(g_tools, SIGNAL(onViewport()), this, SLOT(slotOnViewPort()));
  connect(tools_full, SIGNAL(clicked(QListViewItem* )), this, SLOT(slotClickFullMode(QListViewItem* )));
  connect(tools_full, SIGNAL(onItem(QListViewItem* )), this, SLOT(slotOnItem(QListViewItem* )));
  connect(tools_full, SIGNAL(onViewport()), this, SLOT(slotOnViewPort()));
  connect(tools_short, SIGNAL(clicked(QIconViewItem* )), this, SLOT(slotClickShortMode(QIconViewItem* )));
  connect(tools_short, SIGNAL(onItem( QIconViewItem * )), this, SLOT(slotOnItem(QIconViewItem* )));
  connect(tools_short, SIGNAL(onViewport ()), this, SLOT(slotOnViewPort()));
}


/*!
    \fn myconfviews::load_Icons()
 */
bool myconfviews::load_Icons()
{
  bool b = true;
  QString path; // = qApp->applicationDirPath();
  setAplDir(path);
  path += "/pixmaps/";
  //pixBoot.load(path+"boot.png");
    
  pixBootL.load(path+"boot_sect.png");
  
  if(pixBootL.isNull())
	b = false;
  
  pixMnt.load(path+"hdd_sect.png");
  if(pixMnt.isNull())
	b = false;
   
  pixHwBr.load(path+"hwinfo_sect.png");
  if(pixHwBr.isNull())
	b = false;
  
  pixConfigNetwork.load(path+"net_sect.png");
  if(pixConfigNetwork.isNull())
	b = false;
  
  pixConfigPackades.load(path+/*"system-config-packages.png"*/"packages_sect.png");
  if(pixConfigPackades.isNull())
	b = false;
  
  pixConfig.load(path+"config_sect.png");
  if(pixConfig.isNull())
	b = false;
    
  
  return b;
}


/*!
    \fn myconfviews::resoreCursor()
 */
void myconfviews::restoreCursor()
{
  QApplication::restoreOverrideCursor();
  emit msgClr();
}


/*!
    \fn myconfviews::slotOnItem(QIconViewItem* )
 */
void myconfviews::slotOnItem(QIconViewItem* item)
{
  if(!item)
	return;
  tools_short->setCursor(QCursor(PointingHandCursor));
  emit status_msg_info(loader->get_module_info(item->text()));
}


/*!
    \fn myconfviews::slotOnViewPort()
 */
void myconfviews::slotOnViewPort()
{
  tools_short->setCursor(QCursor(ArrowCursor));
  tools_full->setCursor(QCursor(ArrowCursor));
  g_tools->setCursor(QCursor(ArrowCursor));
  emit msgClr();
}


/*!
    \fn myconfviews::slotOnItem(QListViewItem* )
 */
void myconfviews::slotOnItem(QListViewItem* item)
{
  if(!item)
	return;
  tools_full->setCursor(QCursor(PointingHandCursor));
}


/*!
    \fn myconfviews::slotOnItem(QListBoxItem* )
 */
void myconfviews::slotOnItem(QListBoxItem* item)
{
  if(!item)
	return;
  g_tools->setCursor(QCursor(PointingHandCursor));
}


/*!
    \fn myconfviews::setGradient( QWidget * widget)
 */

