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
#include "propertiesitemimpl.h"
#include "mypartitions.h"
#include "editliloconf.h"

#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qmessagebox.h>

propertiesItemImpl::propertiesItemImpl(myMenuLiloData data, bool bNew, QWidget *parent, const char *name)
    :PropertiesMain(parent, name)
{
	oldLabel = data.label;
	loadData(data, bNew);
}


/*!
    \fn propertiesItemImpl::loadData()
 */
void propertiesItemImpl::loadData(myMenuLiloData& data, bool bNew)
{
	 QStringList str = myPartitions::getPartitions();
  
	rootSelect->insertStringList(str);
  
  //load data image
  
	QDir d = QDir::root();
  
	if(!d.cd("/boot"))
	{
		QMessageBox::critical(0,"Error","Fatal Error can't find boot directory");
		return;
	}
  
  //d.setFilter( QDir::Files);
	d.setNameFilter("vmlinuz*");
  
	const QFileInfoList *list = d.entryInfoList();
	QFileInfoListIterator it( *list );
	QFileInfo *fi;

	while ( (fi = it.current()) != 0 ) {
		selectImage->insertItem(fi->absFilePath());
		++it;
	}
  
  //load initrd data
  
  //d.setFilter( QDir::Files);
	d.setNameFilter("initrd*");
  
	const QFileInfoList *list1 = d.entryInfoList();
	QFileInfoListIterator it1( *list1 );
  
	while ( (fi = it1.current()) != 0 ) {
		selectInitRd->insertItem(fi->absFilePath());
		++it1;
	}
        
	bool bWin = false;
    
	if(!data.type_os.compare("windows family"))
		bWin = true;
	
  
	if(bWin)
	{
		selectImage->setCurrentText("");
		selectInitRd->setCurrentText("");
		selectMode->setCurrentText("");
      
		selectImage->setEnabled(false);
		cmdLine->setEnabled(false);
		selectInitRd->setEnabled(false);
		selectMode->setEnabled(false);
    //cmdLine->hide();
	}
	else
	{
		selectImage->setCurrentText("");
		selectInitRd->setCurrentText("");
	}
    
	if(bNew)
	{    
		return;
	}
  
  
	LabelEdit->setText(data.label);
     
     //check other label
     	
	selectImage->setCurrentText(data.image);
	rootSelect->setCurrentText(data.Root);
	cmdLine->setText(data.append);
	selectInitRd->setCurrentText(data.initrd);
	
	if(data.mode=="838")
		selectMode->setCurrentItem(0);
	else if(data.mode=="837")
		selectMode->setCurrentItem(1);
	else if(data.mode=="795")
		selectMode->setCurrentItem(2);
	else if(data.mode=="794")
		selectMode->setCurrentItem(3);
	else if(data.mode=="775")
		selectMode->setCurrentItem(4);
	else if(data.mode=="792")
		selectMode->setCurrentItem(5);
	else if(data.mode=="791")
		selectMode->setCurrentItem(6);
	else if(data.mode=="773")
		selectMode->setCurrentItem(7);
	else if(data.mode=="789")
		selectMode->setCurrentItem(8);
	else if(data.mode=="788")
		selectMode->setCurrentItem(9);
	else if(data.mode=="771")
		selectMode->setCurrentItem(10);
	else if(data.mode=="786")
		selectMode->setCurrentItem(11);
	else if(data.mode=="785")
		selectMode->setCurrentItem(12);
	else if(data.mode=="769")
		selectMode->setCurrentItem(13);
	else if(data.mode=="0")
		selectMode->setCurrentItem(14);
	else if(data.mode=="6")
		selectMode->setCurrentItem(15);
	else
	  selectMode->setCurrentItem(14);
	
	if(data.main)
		checkMain->setChecked(TRUE);
}


/*!
    \fn propertiesItemImpl::slotFinish()
 */
void propertiesItemImpl::slotFinish()
{
	myMenuLiloData menu;
	QString s;
  
  	if(!selectImage->isEnabled())
	{
		s=rootSelect->currentText();
    
		if(s.isEmpty())
		{
			return;
		}
    
		menu.Root=s;
    
		s=LabelEdit->text();
    
		if(s.isEmpty())
		{
			LabelEdit->setFocus();
			return;
		}
    
		menu.label=s;
		menu.type_os="windows family";
	}
	else
	{
		menu.type_os="linux family";
		s=selectImage->currentText();
		if(s.isEmpty())
		{
			selectImage->setFocus();
			return;
		}
		menu.image=s;
    
		s=LabelEdit->text();
		if(s.isEmpty())
		{
			LabelEdit->setFocus();
			return;
		}
		menu.label=s;
    
		s=rootSelect->currentText();
		menu.Root=s;
    
		s=selectInitRd->currentText();
    
		if(s.isEmpty())
		{
			selectInitRd->setFocus();
			return;
		}
		menu.initrd=s;
    
		menu.append=cmdLine->text();
    
		switch(selectMode->currentItem()){
			case 0: menu.mode="838"; break;
			case 1:	menu.mode="837"; break;
			case 2: menu.mode="795"; break;
			case 3: menu.mode="794"; break;
			case 4: menu.mode="775"; break;
			case 5: menu.mode="792"; break;
			case 6: menu.mode="791"; break;
			case 7: menu.mode="773"; break;
			case 8: menu.mode="789"; break;
			case 9: menu.mode="788"; break;
			case 10: menu.mode="771"; break;
			case 11: menu.mode="786"; break;
			case 12: menu.mode="785"; break;
			case 13: menu.mode="769"; break;
			case 14: menu.mode=="0"; break; 
			case 15: menu.mode=="6"; break;
		}
	}
	
	menu.main = checkMain->isChecked();  
  
	emit sendData(menu, oldLabel);
	accept();
}
