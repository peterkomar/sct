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
#include "selectsourceimpl.h"
#include "addsrcimpl.h"

#include <qdir.h>
#include <qfile.h>
#include <qapplication.h>
#include <qdialog.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qtextedit.h>

selectsourceImpl::selectsourceImpl(QWidget *parent, const char *name, bool modal, WFlags f)
    :EditSource(parent, name, modal, f)
   ,itemI(NULL) 
{
	manager = new rpm_manager;
	manager->get_u_packages();
	
	dlg = new addSrcImpl(this,false);
	
	loadData();
	
	connect(dlg,SIGNAL(send_data( QString, QString, bool, int )),this,SLOT(slotGetMedia(QString, QString , bool ,int )));
	connect(dlg, SIGNAL(abort_add()), manager, SLOT(abort_added()));
	connect(manager, SIGNAL(send_cur_add( int )), dlg, SLOT(set_cur( int )));
	connect(manager,SIGNAL(send_cur_add_str( QString ,int )), dlg, SLOT(slot_progress( QString  ,int )));
	connect(manager, SIGNAL(send_total_add( int )), dlg, SLOT(set_total( int )));
	
	connect(this, SIGNAL(finish_add( bool )), dlg, SLOT(finish( bool )));
	
	load_hlp();
	
	//showMaximized();
}

selectsourceImpl::~ selectsourceImpl()
{
	disconnect(dlg,SIGNAL(send_data( QString, QString, bool, int )),this,SLOT(slotGetMedia(QString, QString , bool ,int )));
	disconnect(dlg, SIGNAL(abort_add()), manager, SLOT(abort_added()));
	disconnect(manager, SIGNAL(send_cur_add( int )), dlg, SLOT(set_cur( int )));
	disconnect(manager,SIGNAL(send_cur_add_str( QString , int)), dlg, SLOT(slot_progress( QString , int )));
	disconnect(manager, SIGNAL(send_total_add( int )), dlg, SLOT(set_total( int )));
	
	disconnect(this, SIGNAL(finish_add( bool )), dlg, SLOT(finish( bool )));
	
	delete dlg;
	delete manager;
}



void selectsourceImpl::slotAdd()
{
	dlg->set_data("", "", false, 1);
  dlg->exec();
}


void selectsourceImpl::slotRemove()
{
  QListViewItem* item = listSrc->selectedItem();
  
  if(item == NULL)
    return;
	
	QString caption = tr("Remove source - SCT rpm");
	QString text = tr("You already want remove <b>%1</b> source.").arg(item->text(0));
	
	QMessageBox *msgDlg = new QMessageBox(caption,text,QMessageBox::Warning,
	QMessageBox::Yes | QMessageBox::Default,
	QMessageBox::No | QMessageBox::Escape,0,this);
	
	msgDlg->setTextFormat(Qt::RichText);
	//msgDlg->resize(msgDlg->sizeHint());
	
	msgDlg->setButtonText( QMessageBox::Yes, tr("Yes") );
	msgDlg->setButtonText( QMessageBox::No, tr("No") );
		
	int i = msgDlg->exec();
	
	delete msgDlg;
	
	if(i == QMessageBox::No)
		return;
  
	manager->rem_rpm_media(item->text(0));
  
  listSrc->removeItem(item);
}


void selectsourceImpl::slotEdit()
{
  QListViewItem* item = listSrc->selectedItem();
  
  if(item == NULL)
    return;
  
  QString s = item->text(2);
  bool b = false;
  
  if(s == tr("YES"))
    b = true;
	
	oldName = item->text(0);
  
	dlg->set_data(item->text(0), item->text(1), b, 2);
  
  itemI = item;
  
	dlg->exec();
}


void selectsourceImpl::slotUpdateBtn()
{
	QListViewItem* item = listSrc->selectedItem();
  
  if(item == NULL)
    return;
  
  bool b = false, b1 = false;
  if(item->text(2) == tr("YES"))
    b = true;
    
  if(item->text(3) == tr("ON"))
    b1 = true;
  	
	QDir di;	
	if(!di.exists(item->text(1)))
	{
		QMessageBox::information(this,tr("No source"),tr("Not found source. Please insert source."));
		return;
	}
  
	dlg->set_data(item->text(0), item->text(1), b, 3);
	
	dlg->show();
	
  manager->rem_rpm_media(item->text(0));
	
	manager->set_path_src(item->text(1));
	
	manager->prepar_add_rpm_src(item->text(1));//calculate count rpm files
	manager->reset_scaler(); //reset scaler files
	
	int res = manager->add_rpm_media(item->text(0), item->text(1), b, b1);
	
	switch(res)
	{
		case -2: emit finish_add(false);	QMessageBox::information(this, tr("Abort"), tr("Can't read file.")); break;
		case -1: emit finish_add(false); QMessageBox::information(this, tr("Abort"), tr("User canceled.")); break;
		default: emit finish_add(true);
	}
}

/*!
    \fn selectsourceImpl::loadData()
 */
void selectsourceImpl::loadData()
{
	listSrc->clear();
	QPtrList<infoRpm> list = manager->getListURpm(true);
	load_data(list);
	list  = manager->get_tmp_list();
	if(!list.isEmpty())
		load_data(list);
}


/*!
    \fn selectsourceImpl::slotGetMedia(QString , QString )
 */
void selectsourceImpl::slotGetMedia(QString nameMedia, QString pathMedia, bool b, int modif)
{
  int res=0;
  QDir di;
  
  switch(modif)
	{
		case 1:	manager->edit_rpm_media(nameMedia,b,true,oldName); emit finish_add(true); break;
		case 2: manager->edit_rpm_media(nameMedia,b,true,"none"); emit finish_add(true); break;
		case 0: if(!di.exists(pathMedia))
		         QMessageBox::critical(this, tr("Not found"),tr("This path is not exists. Please set correct path to source."));
		        else
						{
							manager->rem_rpm_media(nameMedia);
							manager->set_path_src(pathMedia);
														
							manager->prepar_add_rpm_src(pathMedia);//calculate count rpm files
							manager->reset_scaler(); //reset scaler files
							res = manager->add_rpm_media(nameMedia, pathMedia, b, true);
							if(res == -1)
							{
								emit finish_add(false);
								QMessageBox::information(this, tr("Abort"), tr("User canceled."));
							}
							else
								emit finish_add(true);
						}
		break;
	}
	
	if(res != -1)
		loadData();
	else
		manager->rem_rpm_media(nameMedia);
}

void selectsourceImpl::slotYes()
{
	manager->write_media_data();
	accept();
}



/*!
    \fn selectsourceImpl::load_data(QPtrList<infoRpm> list)
 */
void selectsourceImpl::load_data(QPtrList<infoRpm> list)
{
	infoRpm *data = NULL;
	infoRpm *d = NULL;
	
	QPtrListIterator<infoRpm> it( list );
	QPtrListIterator<infoRpm> it2( list );
	
	while ( (data = it.current()) != 0 ) {
		++it;
		it2 = it;
		while ( (d = it2.current()) != 0 ) {
			++it2;
			if(d != 0)
			{
				if(data->nameSource == d->nameSource)
					list.remove(d);
				
			}
		}
	}
	
	data = (infoRpm*)list.first();
	
	QString cd, on;
	  
		
	while(data != NULL)
	{
		if(data->cd)
			cd = tr("YES");
		else
			cd = tr("NO");
                
                if(data->b_on)
                  on = tr("ON");
                else
                  on = tr("OFF");
                
		new QListViewItem(listSrc, data->nameSource,data->pathSource,cd,on);
			
		data = (infoRpm*)list.next();	
	}
    
	return;   
}


/*!
    \fn selectsourceImpl::load_hlp()
 */
void selectsourceImpl::load_hlp()
{
	QString helpMsg;
	
        helpMsg = tr("<h3>Source manager.</h3>"
        "Here you can execute various operations with sources. A source is this place (folder, cd, dvd) "
        "where rpm packages are. Accessible next operations of addition, delete, editing, renewal.<br>"
        "<b>Add</b> - is addition of new source.<br>"
        "<b>Delete</b> - is a delete of existent source.<br>"
        "<b>Editing</b> - is a change of the name of existent source, or to the path where it is.<br>"
        "<b>Update</b> - if you to the source (for example, to the directory ) added new packages or "
            "deleted about it it is needed "
        "to report manager of sources, choosing a necessary source and pushing the button to renew. "
        "To save information about added, remote or press the renewed sources to <b>save</b> the button, "
        "if no - <b>discard</b>.");
	
	helpTxt->setText(helpMsg);
}


/*!
    \fn selectsourceImpl::change_status()
 */
void selectsourceImpl::change_status()
{
  QListViewItem* item = listSrc->selectedItem();
  
  if(item == NULL)
    return;
  
  bool b = false, b1 = false;
  if(item->text(2) == tr("YES"))
    b = true;
    
  if(item->text(3) == tr("ON"))
  {
    b1 = false;
    item->setText(3, tr("OFF"));
  }
  else
  {
    b1 = true;
    item->setText(3, tr("ON"));
  }
  
  manager->edit_rpm_media(item->text(0),b,b1,"none"); 
}
