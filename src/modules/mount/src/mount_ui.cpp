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
#include "mount_ui.h"

#include <qpopupmenu.h>
#include <qaction.h>
#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qiconview.h>
#include <qmenubar.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qtextedit.h>
#include <qvbox.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qsettings.h>

#include "mypartitions.h"
#include "myselectmodeimpl.h"
#include "myeditmountimpl.h"


mount_ui::mount_ui(QWidget *parent, const char *name)
	:QMainWindow(parent, name)
	,b_view_ex(false)
	,b_write_fstab(false)
{
	//setMinimumSize(800,533);
	load_menu();
	create_gui();
	init_connections();
	load_data();
	showMaximized();
	show_context_help();
        
        setCaption("SCT mount @Beta2");
	
	//status
}


/*!
    \fn mount_ui::greate_gui()
 */
void mount_ui::create_gui()
{
	(void)statusBar();
	//setMinimumSize( QSize( 728, 467 ) );
	setCentralWidget( new QWidget( this, "qt_central_widget" ) );
		
	view = new QGridLayout(centralWidget(), 2,2);
	view->setMargin( 9 );
	
	help_lab = new QTextEdit(centralWidget());
	help_lab->setMinimumSize( QSize( 250, 0 ) );
	help_lab->setMaximumSize( QSize( 250, 32767 ) );
	help_lab->setFocusPolicy( QTextEdit::NoFocus );
	help_lab->setReadOnly( TRUE );
	
	view->addWidget( help_lab, 0, 0 );
	
	vbox = new QVBox(centralWidget());
	vbox->setMargin( 9 );
	
	ex_list_part = new QListView(/*centralWidget()*/vbox);
	ex_list_part->setSorting( -1, FALSE );
	//ex_list_part->setSortColumn(-1); 
	ex_list_part->setResizeMode( QListView::AllColumns );
	ex_list_part->addColumn(tr("Partition"));
	ex_list_part->addColumn(tr("Size"));
	ex_list_part->addColumn(tr("Type"));
	ex_list_part->addColumn(tr("Mount Point"));
	ex_list_part->addColumn(tr("Mount Parameters"));
	ex_list_part->addColumn(tr("Name in DOS"));
	ex_list_part->addColumn(tr("Mounted"));
	ex_list_part->addColumn(tr("Root"));
	ex_list_part->setAllColumnsShowFocus(true);
	ex_list_part->setColumnWidthMode( 0, QListView::Maximum);
	ex_list_part->setColumnWidthMode( 1, QListView::Maximum);
	ex_list_part->setColumnWidthMode( 2, QListView::Maximum);
	ex_list_part->setColumnWidthMode( 3, QListView::Maximum);
	ex_list_part->setColumnWidthMode( 4, QListView::Maximum);
	ex_list_part->setColumnWidthMode( 5, QListView::Maximum);
	ex_list_part->setColumnWidthMode( 6, QListView::Maximum);
	ex_list_part->setColumnWidthMode( 7, QListView::Maximum);
	ex_list_part->setResizePolicy( QScrollView::Manual );
	ex_list_part->setVScrollBarMode( QListView::Auto );
	ex_list_part->setHScrollBarMode( QListView::Auto );
	//ex_list_part->setShowSortIndicator( TRUE );
	ex_list_part->setResizeMode( QListView::LastColumn );
	ex_list_part->setRootIsDecorated(TRUE);
	//ex_list_part->setSortColumn(-1);
	ex_list_part->hide();
	
	list_part = new QIconView(vbox);
	list_part->setItemsMovable(FALSE);
  //list_part->setSelectionMode(QIconView::NoSelection);
	list_part->setWordWrapIconText(TRUE);
	list_part->setArrangement(QIconView::LeftToRight);
	list_part->setSpacing(35);
	list_part->setResizeMode(QIconView::Adjust);
	list_part->setGridX(150);
	list_part->setGridY(90);
	list_part->setMaxItemWidth(400);
	list_part->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) ); 
	
	//list_part->hide();
	
	view->addWidget(vbox/*ex_list_part*/,0,1);
	
	box = new QHBox(vbox);
	box->setMargin( 7 );
	
	mountBtn = new QPushButton(tr("Mount"),box);
	umountBtn = new QPushButton(tr("Unmount"),box);
	rescanBtn = new QPushButton(tr("Scan"),box);
	
	mountAct = new QAction(tr("mount partition"),tr("mount"),0,this);
	umountAct = new QAction(tr("unmount partition"),tr("unmount"),0,this);
	
	menu = new QPopupMenu(this);
  	menu->setCaption(tr("Mount Menu"));  
	mountAct->addTo(menu);
	umountAct->addTo(menu);
	
	myManager = new myMountManager;
	
}


/*!
    \fn mount_ui::init_connections()
 */
void mount_ui::init_connections()
{
	connect(expert_w, SIGNAL(toggled(bool )),this, SLOT(slot_view( bool )));
	connect(write_fstab, SIGNAL(toggled(bool )), this, SLOT(toggle_write(bool )));
	connect(ex_list_part, SIGNAL(contextMenuRequested( QListViewItem*, const QPoint&, int )),this, SLOT(slotViewMenu(QListViewItem* ,const QPoint& )));
	connect(list_part, SIGNAL(doubleClicked(QIconViewItem *)),this, SLOT(slot_mount()));
	connect(mountBtn, SIGNAL(clicked()),this, SLOT(slot_mount()));
	connect(umountBtn, SIGNAL(clicked()), this, SLOT(slot_umount()));
	connect(rescanBtn, SIGNAL(clicked()), this, SLOT(slot_rescan()));
	connect(mountAct, SIGNAL(activated ()),this, SLOT(slot_mount()));
	connect(umountAct, SIGNAL(activated ()), this, SLOT(slot_umount()));
	connect(list_part, SIGNAL(onItem( QIconViewItem * )), this, SLOT(slotOnItem(QIconViewItem* )));
	connect(list_part, SIGNAL(onViewport ()), this, SLOT(slotOnViewPort()));
	connect(list_part, SIGNAL(rightButtonPressed(QIconViewItem* , const QPoint & )), this, SLOT(slotViewMenu(QIconViewItem* ,const QPoint& )));
}


/*!
    \fn mount_ui::load_data()
 */
void mount_ui::load_data()
{
	ex_list_part->clear();
	list_part->clear();
	p_list_part.clear();
	
	p_list_part = myManager->getPartition();
	if(p_list_part.isEmpty())
		return;
		
	load_view_data();
	load_tree_data();
}


/*!
    \fn mount_ui::getData(myDataPartition data, bool write)
 */
void mount_ui::getData(myDataPartition data)
{
	QApplication::setOverrideCursor(QCursor::waitCursor);
	
	myManager->mountPartition(data, b_write_fstab);
	
	QApplication::restoreOverrideCursor();
}


/*!
    \fn mount_ui::getDataUmount(bool , bool )
 */
void mount_ui::getDataUmount(bool bforce)
{
	QString s;
	if(b_view_ex){
		QListViewItem *item = ex_list_part->currentItem();
  
	  if(item==NULL)
		 return;
	  s = item->text(0);
	if(s.isEmpty())
		return;
	
	if(s.find("[")!=-1)
		return;
	
	}
	else
	{
		QIconViewItem *item = list_part->currentItem(); 
		if(item==NULL)
			return;
		s =item->text();
		get_dev_name(s);
	}
	QApplication::setOverrideCursor(QCursor::waitCursor);	  
	myManager->UmountPartition(s,bforce,b_write_fstab);
	QApplication::restoreOverrideCursor();
	load_data();
}


/*!
    \fn mount_ui::slot_mount()
 */
void mount_ui::slot_mount()
{
	QString tmp;
	
	if(b_view_ex)
	{
		QListViewItem *item = ex_list_part->currentItem();
		
		if(item == NULL)
			return;
		
		tmp = item->text(0);
		
		if(tmp.find("[")!=-1)
			return;
		
		myDataPartition data = get_data_part(tmp);
		
		myEditMountImpl *dlg = new myEditMountImpl(data, this);
		
		connect(dlg, SIGNAL(sendData( myDataPartition )),this, SLOT(getData( myDataPartition )));
		
		dlg->exec();
		
				
		delete dlg;
	}
	else
	{
	  QIconViewItem *item = list_part->currentItem(); 
	  if(item==NULL)
	   return;
	  tmp =item->text();
	  get_dev_name(tmp);
		QApplication::setOverrideCursor(QCursor::waitCursor);	  
	  tmp = myManager->fast_mount(tmp);
	  if(!tmp.isEmpty())
		  QMessageBox::about( this,tr("Mount"), tr("Mounted to : %1").arg(tmp));
		QApplication::restoreOverrideCursor();
	}
	
	load_data();
}


/*!
    \fn mount_ui::slot_rescan()
 */
void mount_ui::slot_rescan()
{
	load_data();
}


/*!
    \fn mount_ui::slot_umount()
 */
void mount_ui::slot_umount()
{
	QString tmp;
	
	if(b_view_ex){
		QListViewItem *i = ex_list_part->currentItem();
		if(i==NULL)
			return;
		
		if(i->text(7)=="*")
		{
			QMessageBox::critical(this,tr("Error unmount"), tr("You don't unmount root partition.'"));
			return;
		}
		/*
	  if(i->text(3)=="swap")
		return;*/
	}
	else
	{
		QIconViewItem *item = list_part->currentItem(); 
		if(item==NULL)
			return;
		tmp =item->text();
		get_dev_name(tmp);
	}
  
		
	mySelectModeImpl *selectDlg = new mySelectModeImpl(this);
  
	connect(selectDlg, SIGNAL(sendParameterUmount(bool )), this, SLOT(getDataUmount(bool )));
  
	selectDlg->exec();
  
	delete selectDlg;
}


/*!
    \fn mount_ui::slotViewMenu(QListViewItem* item,const QPoint& pt, int i)
 */
void mount_ui::slotViewMenu(QListViewItem* , const QPoint& pt)
{
	menu->popup(pt);
}


/*!
    \fn mount_ui::fileExit()
 */
void mount_ui::fileExit()
{
	close();
}


/*!
    \fn mount_ui::helpAbout()
 */
void mount_ui::helpAbout()
{
	QMessageBox::about(this, tr("About SCT mount"),tr("GUI for <mount> command.\n"
			"Module for SCT.\ndevelopment: sksoft@i.ua"));
}


/*!
    \fn mount_ui::helpIndex()
 */
void mount_ui::helpIndex()
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
        
  s+="/doc/"+get_locale()+"/sct_mount.html";
  
  proc.addArgument(s);
  
  if(!proc.start())
    QMessageBox::information(this,tr("Info"),tr("Can't find help system."));
}


/*!
    \fn mount_ui::load_menu()
 */
void mount_ui::load_menu()
{
  QPopupMenu *menuFile = new QPopupMenu(this);
  QPopupMenu *menuView = new QPopupMenu(this);
  QPopupMenu *menuHelp = new QPopupMenu(this);
  
  menuBar()->insertItem(tr("Data"), menuFile);
  menuBar()->insertItem(tr("View"), menuView);
  menuBar()->insertItem(tr("Help"), menuHelp);
 	
	write_fstab = new QAction(tr("Mode write data to fstab file"),tr("Write to fstab file"),0, this,tr("Write to fstab file"));
  write_fstab->setToggleAction(TRUE);
  write_fstab->addTo(menuFile);
  
  menuFile->insertSeparator(1);
  menuFile->insertItem(tr("Exit"), this, SLOT(fileExit()));
 
  expert_w = new QAction(tr("Expert mode"),tr("Expert"),0, this,tr("Expert mode"));
  expert_w->setToggleAction(TRUE);
  expert_w->addTo(menuView);
  
  menuHelp->insertItem(tr("Index..."), this, SLOT(helpIndex()));
  menuHelp->insertSeparator(1);
  menuHelp->insertItem(tr("About"),this, SLOT(helpAbout()));
}


/*!
    \fn mount_ui::slot_view(bool )
 */
void mount_ui::slot_view(bool b)
{
	if(b)
	{
		b_view_ex=true;
		ex_list_part->show();
		list_part->hide();
	}
	else
	{
		b_view_ex=false;
		ex_list_part->hide();
		list_part->show();
	}
	show_context_help();
}


/*!
    \fn mount_ui::slotOnItem(QIconViewItem* )
 */
void mount_ui::slotOnItem(QIconViewItem* item)
{
	if(!item)
		return;
	list_part->setCursor(QCursor(PointingHandCursor));
}


/*!
    \fn mount_ui::slotOnViewPort()
 */
void mount_ui::slotOnViewPort()
{
   list_part->setCursor(QCursor(ArrowCursor));
}


/*!
    \fn mount_ui::paintEvent ( QPaintEvent * )
 */
void mount_ui::paintEvent( QPaintEvent * )
{

}


/*!
    \fn mount_ui::toggle_write(bool )
 */
void mount_ui::toggle_write(bool b)
{
	b_write_fstab = b;
}


/*!
    \fn mount_ui::load_tree_data()
 */
void mount_ui::load_tree_data()
{
	myDataPartition* dat = (myDataPartition*)p_list_part.last();
	
	QString s_dev = QString("[%1 %2]").arg(dat->Device).arg(dat->SizeDevice);
	QString s_tmp, s_mnt, s_root;
	QListViewItem *item = new QListViewItem(ex_list_part,s_dev);
	//item->setExpandable(TRUE);
			
	while(dat!=NULL)
	{
		ex_list_part->setOpen(item, TRUE);
		s_tmp = QString("[%1 %2]").arg(dat->Device).arg(dat->SizeDevice);
		
		if(s_dev == s_tmp)
		{
			if(dat->b_mount)
				s_mnt = "*";
			else
				s_mnt = "";
			
			if(dat->b_root)
				s_root = "*";
			else
				s_root = "";
			
			
			new QListViewItem(item, dat->part,dat->Size,dat->fileSystem,dat->MntPnt,dat->Parameter,dat->nameDisk,s_mnt, s_root);
		}
		else
		{
			s_dev = QString("[%1 %2]").arg(dat->Device).arg(dat->SizeDevice);
			item = new QListViewItem(ex_list_part,s_dev);
			//item->setExpandable(TRUE);
			continue;
		}
		dat = (myDataPartition*)p_list_part.prev();
	}
}


/*!
    \fn mount_ui::load_view_data()
 */
void mount_ui::load_view_data()
{
	QString path = qApp->applicationDirPath();
	setAplDir(path);
			
	myDataPartition* data = (myDataPartition*)p_list_part.first();
	QIconViewItem *item = NULL;
		
		  
	while(data!=NULL)
	{
		if(data->b_root)
		{
			data = (myDataPartition*)p_list_part.next();
		  	
			continue;
		}
		
		if(data->fileSystem.find("swap") != -1)
		{
			data = (myDataPartition*)p_list_part.next();
		  	
			continue;
		}
		
		QString s;
		
		if(!data->MntPnt.isEmpty())
			s = tr("%1:(%2)\nSize:%3\nType:%4\nPath:%5").arg(data->nameDisk).arg(data->part)
					.arg(data->Size).arg(data->fileSystem).arg(data->MntPnt);
		else
			s = tr("%1:(%2)\nSize:%3\nType:%4\nPath:<not mounted>").arg(data->nameDisk).arg(data->part)
					.arg(data->Size).arg(data->fileSystem);
		
		if(data->fileSystem == "USB Flash disk")
		{
			if(data->b_mount)
				item = new QIconViewItem(list_part, s, QPixmap(path+"usb_mount.png"));
			else
				item = new QIconViewItem(list_part, s, QPixmap(path+"usb_unmount.png"));
			
			item->setDragEnabled(FALSE);
			item->setDropEnabled(FALSE);
			
			data = (myDataPartition*)p_list_part.next();
			continue;			
		}
				    				
		if(data->fileSystem.find("Linux")!=-1)
		{
			if(data->b_mount)
				item = new QIconViewItem(list_part, s, QPixmap(path+"lin_mount.png"));
			else
				item = new QIconViewItem(list_part, s, QPixmap(path+"lin_unmount.png"));
		}
		else{
			if((data->fileSystem.find("FAT32")!=-1) || (data->fileSystem.find("NTFS")!=-1))
			{
				if(data->b_mount)
					item = new QIconViewItem(list_part, s, QPixmap(path+"win_mount.png"));
				else
					item = new QIconViewItem(list_part, s, QPixmap(path+"win_unmount.png"));
			}
			else
			{  
				if(data->b_mount)
					item = new QIconViewItem(list_part, s, QPixmap(path+"mount_drive.png"));
				else
					item = new QIconViewItem(list_part, s, QPixmap(path+"unmount_drive.png"));
			}
		}
		
		item->setDragEnabled(FALSE);
		item->setDropEnabled(FALSE);
		
		data = (myDataPartition*)p_list_part.next();
	}
}


/*!
    \fn mount_ui::get_data_part(const QString& )
 */
myDataPartition mount_ui::get_data_part(const QString& s)
{
	myDataPartition* data = (myDataPartition*)p_list_part.first();
	
	while(data != NULL)
	{
		if(data->part == s)
			return *data;
		
		data = (myDataPartition*)p_list_part.next();
	}
        	
    return *data;
}

/*!
    \fn mount_ui::show_context_help()
 */
void mount_ui::show_context_help()
{
	QString page1 = tr("<h3>Ordinary mode.</h3><b>Attention.</b>"
			 "That sections were automatically connected with the parameters preset "
			 "by you at the load of the system it is necessary to set mode of record "
			 "in the file of fstab <b>Data/Write to fstab</b>. Before setting this mode "
			 "information about editing of sections in the file of /etc/fstab is not written "
			 "down. (It is recommended to create the back-up copy of file of /etc/fstab).<br><br>"
			 "To mount a section it is necessary to choose him and push the button to <b>Mount</b>, "
			 "or avail context dependent menus and the program automatically will define parameters "
			 "and will connect the chosen section and will show information where the set section was "
			 "connected. Like carried out unmount of section instruction <b>Unmount</b> execution."
			 " If you need greater control above editing of section commuted in the mode of expert "
			 "<b>View/Expert</b>. The button is intended to <b>Scan</b> for the exposure of new"
			 " sections which were connected to the computer already after the start of the program."
			 " (Example: USB flash disk and others like that).");
	
	QString page2 = tr("<h3>Mode of expert.</h3>"
			"Attention. That sections were automatically connected with the parameters preset "
			"by you at the load of the system it is necessary to set mode of record in the file of fstab "
			"<b>Data/Write to fstab</b>. Before setting this mode information about editing of sections in the file of "
      "/etc/fstab is not written down. (It is recommended to create the back-up copy of file of /etc/fstab).<br><br>"
			"In the mode of expert you will be able in hand to set the parameters of editing. This mode befits more "
			"skilled users which know naming of hard disks in <b>Linux</b> systems and they know the "
			"parameters of connecting of sections. For those users which the parameters of editing are "
			"unknown accessible system of prompts it is enough to point the cursor of mouse in the window "
			"of editing on a parameter which interests you. Mounting, unmount, scanning is conducted like "
			"till it is done in the ordinary mode.");
	
	if(b_view_ex) //if expert mode
		help_lab->setText(page2);
	else //if normal mode
		help_lab->setText(page1);
	
}


/*!
    \fn mount_ui::slotViewMenu(QListViewItem* ,const QPoint& pt)
 */
void mount_ui::slotViewMenu(QIconViewItem* ,const QPoint& pt)
{
	menu->popup(pt);
}


/*!
    \fn mount_ui::get_locale()
 */
QString mount_ui::get_locale()
{
  QDir d = QDir::home();
  
  QString s = d.absPath();
	
  QSettings settings;
	 
  settings.removeSearchPath( QSettings::Unix, s+"/.qt");
  settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
   
  QString slang = settings.readEntry("/SCT/Language_UI","en"); 
  
  return slang;
}
