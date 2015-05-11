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
#include "addsrcimpl.h"

#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qtextedit.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qapplication.h>

addSrcImpl::addSrcImpl(QWidget *parent, const char *name)
    :AddSrcdlg(parent, name)
    ,b_new(false)
    ,bCheked(false)
{
	backButton()->hide();
	helpButton()->hide();
	total = 0;
	load_hlp();
	
	connect(cancelButton(), SIGNAL(clicked()),this, SLOT(slot_cancel()));
}

void addSrcImpl::slotBrowse()
{
	QString DirPath;
	QFileDialog* fd = new QFileDialog( this, "file dialog", TRUE );
	fd->setMode( QFileDialog::DirectoryOnly);
	fd->setDir("/");
  
	fd->setFilter( "Packages (*.rpm)" );
  
	fd->setViewMode( QFileDialog::List );
	fd->setCaption(tr("Select Directory"));
	fd->setPreviewMode(QFileDialog::NoPreview); 
  
	if ( fd->exec() == QDialog::Accepted )
		DirPath = fd->dirPath();
	delete fd;
  
	pathEdit->setText(DirPath);
}



/*!
    \fn addSrcImpl::next()
 */
void addSrcImpl::next()
{
	//nextButton()->hide();
	
	QString name, path;
  
	name = nameEdit->text();
	path = pathEdit->text();
	bool b = checkCD->isChecked();
  
	if(name.isEmpty())
	{
		QMessageBox::information(this,tr("Info"),tr("Please enter name Media"));
    
		nameEdit->setFocus();
		return;
	}
	if(path.isEmpty())
	{
		QMessageBox::information(this,tr("Info"),tr("Please enter path to Media"));
    
		pathEdit->setFocus();
		return;
	}
	
	int code=0;
	
	if((b_new) || (pathEdit->isModified()))
		code = 0;
	else	if(nameEdit->isModified())
		code = 1;
	else if(b != bCheked)
		code = 2;
	else
	{
		accept();
		return;
	}
	  
	showPage(finishPage);
	backButton()->hide();
	qApp->processEvents();
	outAdd->setText(QString("<b>%1</b>").arg(tr("Preparing added ... ")));	
	qApp->processEvents();
	
	emit send_data(name, path, b,code);
}


/*!
    \fn addSrcImpl::finish(bool er)
 */
void addSrcImpl::finish(bool er)
{
   if(!er)
   {
	outAdd->append(QString("<b>%1</b>").arg(tr("Abort")));	
   }
   else
   {
     outAdd->append(QString("<b>%1</b>").arg(tr("Completed")));	
     QMessageBox::information(this, tr("Added"), tr("Job is success completed."));
   }
  //accept();
    setFinishEnabled(finishPage,TRUE);

   cancelButton()->hide();
}


/*!
    \fn addSrcImpl::set_data(QString name, QString path, bool cd)
 */
void addSrcImpl::set_data(QString name, QString path, bool cd, int mode)
{
	total = 0;
	b_new=false;
	bCheked = false;
	
	setFinishEnabled(finishPage,FALSE);
	cancelButton()->show();
	progressAdd->reset();
	label->setText("");
	outAdd->setText("");
	
	switch(mode){
		case 1: b_new = true;
		case 2: showPage(firstPage);
				nameEdit->setText(name);
				pathEdit->setText(path);
				checkCD->setChecked(cd);
				bCheked = cd;
				nameEdit->clearModified();
				pathEdit->clearModified();
				break;
		case 3: showPage(finishPage); break;
	}
	
	cancelButton()->setDefault(TRUE);
	
	nameEdit->setFocus();
}


/*!
    \fn addSrcImpl::set_cur(int )
 */
void addSrcImpl::set_cur(int i)
{
	progressAdd->setProgress(i, total);
	qApp->processEvents();
	label->setText(tr("Added %1 among %2").arg(i).arg(total));
	qApp->processEvents();
}


/*!
    \fn addSrcImpl::set_total(int )
 */
void addSrcImpl::set_total(int n)
{
	//outAdd->clear();
	//progressAdd->reset();
	//progressAdd->setTotalSteps(n);
	
	total += n;
}


/*!
    \fn addSrcImpl::slot_progress(QString)
 */
void addSrcImpl::slot_progress(QString msg, int code)
{
	QString s;
	if(code == 1)
	{
	    s = QString("<b>%1</b> <font color=\"blue\"><i>%2</i></font> <b>%3</b>")
			.arg(tr("Added"))
			.arg(msg)
			.arg(tr("package"));
	}
	else if(code == 2)
	{
		s = QString("<font color=\"red\"><b>%1</b> <i>%2</i> <b>%3</b> </font>")
				.arg(tr("Error added"))
				.arg(msg)
				.arg(tr("package : Error read header"));
	}
	else
	{
		s = QString("<font color=\"red\"><b>%1</b> <i>%2</i> <b>%3</b> </font>")
				.arg(tr("Error added"))
				.arg(msg)
				.arg(tr("package : Can't read package"));
	}
	outAdd->append(s);
	qApp->processEvents();
}


/*!
    \fn addSrcImpl::slot_cancel()
 */
void addSrcImpl::slot_cancel()
{
	emit abort_add();
	
	reject();
}


/*!
    \fn addSrcImpl::show()
 */
void addSrcImpl::show()
{
	QWizard::show();
}


/*!
    \fn addSrcImpl::load_hlp()
 */
void addSrcImpl::load_hlp()
{
	QString helpMsg;
	
        helpMsg = tr("<h3>Add and edit source wizard.</h3>"
            "Enter the name of source (choose on own discretion), the option of <b>CD/DVD</b> specifies"
            " on te that this source is  CD/DVD by a disk. In the field to the source specify a path where packages are. "
            "Push button <b>next</b>, to begin the process of addition.");
	
	helpTxt->setText(helpMsg);
}
