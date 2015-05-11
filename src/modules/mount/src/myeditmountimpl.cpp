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
#include "myeditmountimpl.h"
#include "str_tools.h"

#include <qcombobox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>

myEditMountImpl::myEditMountImpl(myDataPartition data, QWidget *parent, const char *name)
    :editMount(parent, name)
{
  loadData(data);
}

void myEditMountImpl::slotFinish()
{
	myDataPartition dataM;
  QString opt, tmp;
	dataM.part = namePart->text();
  dataM.FS_fstab = selectType->currentText();
  dataM.MntPnt = selectMP->currentText();
  
  if(dataM.FS_fstab.isEmpty())
  {
    selectType->setFocus();
    return;
  }
  
  if(dataM.MntPnt.isEmpty())
  {
    selectMP->setFocus();
    return;
  }
  
  if(checkSync->isChecked ())
    opt+=",sync";
  if(checkNoatime->isChecked())
    opt+=",noatime";
  if(checkRo->isChecked())
    opt+=",ro";
  if(checkNoSuid->isChecked())
    opt+=",nosuid";
  if(checkUmask22->isChecked())
    opt+=",umask=0002";
  if(checkUser->isChecked())
    opt+=",user";
  if(checkNoExec->isChecked())
    opt+=",noexec";
  if(checkNodev->isChecked())
    opt+=",nodev";
  tmp = editOther->text();
  if(!tmp.isEmpty()) 
    opt+=","+tmp;
  
  tmp = editCharSet->currentText();
  if(!tmp.isEmpty())
    opt+=",iocharset="+tmp;
  
  tmp = editCodePage->currentText();
  if(!tmp.isEmpty())
    opt+=",codepage="+tmp;
	trimStr(opt, ",");
  
  dataM.Parameter=opt;
  
  if(dataM.fileSystem=="ext3")
  {
    dataM.freq="1";
    dataM.passno="2";
  }
  else
  {
    dataM.freq="0";
    dataM.passno="0";
  }
  
  
  accept();
  
  emit sendData(dataM);   
}



/*!
    \fn myEditMountImpl::loadData(myDataPartition& data)
 */
void myEditMountImpl::loadData(myDataPartition data)
{
	namePart->setText(data.Device);
  selectType->setCurrentText(data.FS_fstab);
  //selectMP->insertItem(data.MntPnt);
  selectMP->setCurrentText(data.MntPnt);
  editCharSet->setCurrentText("");
  editCodePage->setCurrentText("");
	
	namePart->setText(data.part);
	nameType->setText(data.fileSystem);;
	nameSize->setText(data.Size);
	  
  int n,i,j;
  QString s = data.Parameter;
  QString tmp,t;
  QString cmdL;
  i=n=0;
  
  while(i!=-1)
  {
    i = s.find(",");
    
    if(i==-1)
    {
      tmp = s;
    }
    
    tmp = s.mid(0,i);
    //tmp.remove(i,1);
    s.remove(0,i+1);
    
    if(tmp.find("iocharset=")!=-1)
    {
      j = tmp.find("=");
      
      t = tmp.mid(j+1,tmp.length()-j);
      editCharSet->insertItem(t);
      editCharSet->setCurrentText(t);
    }
    else if(tmp.find("codepage=")!=-1)
    {
      j = tmp.find("=");
      
      t = tmp.mid(j+1,tmp.length()-j);
      editCodePage->insertItem(t);
      editCodePage->setCurrentText(t);
    }
    else if(tmp=="sync")
      checkSync->setChecked(true);
    else if(tmp=="noatime")
      checkNoatime->setChecked(true);
    else if(tmp=="ro")
      checkRo->setChecked(true);
    else if(tmp=="nosuid")
      checkNoSuid->setChecked(true);
    else if(tmp=="umask=0022")
      checkUmask22->setChecked(true);
    else if(tmp=="user")
      checkUser->setChecked(true);
    else if(tmp=="noexec")
      checkNoExec->setChecked(true);
    else if(tmp=="nodev")
      checkNodev->setChecked(true);
    else{
      cmdL+=tmp+",";
    }//,iocharset=koi8-u,codepage=866,noauto,ro,exec,users
  }
	
	trimStr(cmdL, ",");
  editOther->setText(cmdL);
   
}
