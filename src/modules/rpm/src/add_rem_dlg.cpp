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
 
#include "add_rem_dlg.h"

#include <qpushbutton.h>
#include <qtextedit.h>
#include <qprogressbar.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qmessagebox.h>

Add_Rem_dlg::Add_Rem_dlg(QWidget *parent, const char *name)
    :rpm_dlg(parent, name)
    ,incr(0)
    ,allmatch(true)
    ,mode_install(true)
{
	set_gui_parameter();
		
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slot_cancel()));
	connect(rpmBtn, SIGNAL(clicked()), this, SLOT(begin_rpm()));
}


/*!
    \fn Add_Rem_dlg::set_gui_parameter()
 */
void Add_Rem_dlg::set_gui_parameter()
{
	
	total_progress->reset();
	countLabel->setText("");
	current_progress->reset();
	curLabel->setText("");
	
	rpmBtn->setDisabled(TRUE);
}



/*!
    \fn Add_Rem_dlg::set_mode(bool install=true)
 */
void Add_Rem_dlg::set_add_mode(bool install)
{
	selectOpt->hide();
	
	if(install)
	{
		selectOpt->setTitle(tr("Install options"));
		bAll->setText(tr("Install all (recommended)"));
		b_selected->setText(tr("Install only selected"));
		mode_install = true;
		rpmBtn->setText(tr("Install"));
	}
	else
	{
		selectOpt->setTitle(tr("Remove options"));
		bAll->setText(tr("Remove All (recommended)"));
		b_selected->setText(tr("Remove &only selected"));
		mode_install = false;
		rpmBtn->setText(tr("Remove"));
	}
        
        load_help();
}


/*!
    \fn Add_Rem_dlg::slot_get_first_report(const QString& report_msg, bool berr )
 */
void Add_Rem_dlg::slot_get_first_report(const QString& report_msg, bool berr )
{
	log_txt->setText(report_msg);
	
	if(berr)
		selectOpt->show();
	else
		selectOpt->hide();
			
	
	rpmBtn->setEnabled(TRUE);
}


/*!
    \fn Add_Rem_dlg::slot_current_item(const QString& )
 */
void Add_Rem_dlg::slot_current_all_items(int i, const QString& msg)
{	
        log_txt->scrollToBottom(); 
	log_txt->append(msg);
	
	int j = msg.find("-");
	
	if(j != -1)
	{
		QString s = msg.mid(0,j);
		
		s = s.stripWhiteSpace();
		
		curLabel->setText(s);
	}
	else
		curLabel->setText(msg);	
		
	total_progress->setProgress(i);
}


/*!
    \fn Add_Rem_dlg::slot_current_progress(int )
 */
void Add_Rem_dlg::slot_current_parcent_rpm(int n)
{
	current_progress->setProgress(n);
}


/*!
    \fn Add_Rem_dlg::slot_get_end_report(const QString& )
 */
void Add_Rem_dlg::slot_finish_rpm(const QString& msg, bool b_error)
{
	cancelBtn->setText(tr("Finish"));
	
        total_progress->hide();
        countLabel->hide();
        current_progress->hide();
        curLabel->hide();
        rpmBtn->hide();//if user abort process before installation
        selectOpt->hide();//if user abort process before installation
        
	
	QString s;
	s = "<br><br><br><table border=0 width=100%><tr><th colspan=2 valign=top bgcolor=\"lightGray\" width=5%>"
	    "<font color=\"blue\">%1</font></th></tr>";//heder
	QString txt = "<tr><td bgcolor=\"#EEF6FF\" width=5%><b>%1</b></td><td bgcolor=\"#EEF6FF\" width=95%><i>%2</i></td></tr>";
	
	s = s.arg(tr("Report"));
	
	if(mode_install)
	 s += txt.arg(tr("Process")).arg(tr("Installation"));
	else
	 s += txt.arg(tr("Process")).arg(tr("Deleting"));
	
	if(b_error)
	{
	 s += txt.arg(tr("Status")).arg(tr("error"));
	 s += txt.arg(tr("Causes")).arg(msg);
	}
	else
	 s += txt.arg(tr("Status")).arg(tr("succes"));
	
	s += "</table>";
		
	//log_txt->setText(s);
	log_txt->append(s);
	
	disconnect(cancelBtn, SIGNAL(clicked()), this, SLOT(slot_cancel()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slot_close()));
}


/*!
    \fn Add_Rem_dlg::slot_total_progress(int )
 */
void Add_Rem_dlg::slot_count_all_items(int n)
{
	total_progress->setTotalSteps(n);
}


/*!
    \fn Add_Rem_dlg::next()
 */
void Add_Rem_dlg::begin_rpm()
{
	total_progress->reset();
	incr++;
        
        
        selectOpt->hide();
        rpmBtn->hide();
	
	
	log_txt->append(QString("<hr><b>%1...</b>").arg(tr("Preparing")));
	
	emit start_rpm(allmatch, mode_install);
	
}

/*!
    \fn Add_Rem_dlg::slot_rem_mode(int )
 */
void Add_Rem_dlg::slot_rem_mode(int k)
{
    switch(k)
	{
		case 1 : allmatch=true; break;
		case 0 : allmatch=false; break;
	}
}


/*!
    \fn Add_Rem_dlg::load_help()
 */
void Add_Rem_dlg::load_help()
{
	QString msg;
      
        if(mode_install)
        {
          msg = tr("<h3>Install packages</h3>Program automatically checked dependences of the packages chosen by"
              " you and will add those which"
              " were not chosen by you, however, which are needed for satisfaction of dependences of the packages chosen "
              "by you. Packages are chosen by you will be in a section the <b>Selected</b> is added the program"
              " in a section <b>Additional</b>.<br>"
              "<b>Remark</b>. The program conducts a search only in the indicated sources (see help section manager sources). "
              "In case if the program will add packages, a section will appear the Additional packages, two variants will"
              " be offered to you:<ul>"
              "<li>Install all  packages - will be install indicated in sections chosen and additional.</li>"
              "<li>Install only selected packages - which are indicated in a section Selected will be install, in this case can"
              " be negativnand consequences (and can be not): some programs can be not loaded, or work incorrectly,"
              " in the case of delete the choice of this option can lead to collapse of the operating system.</li></ul>");;
        }
        else
        {
          msg = tr("<h3>Remove packages</h3>Program automatically checked dependences of the packages chosen by"
              " you and will add those which"
              " were not chosen by you, however, which are needed for satisfaction of dependences of the packages chosen "
              "by you. Packages are chosen by you will be in a section the <b>Selected</b> is added the program"
              " in a section <b>Additional</b>.<br>"
              "<b>Remark</b>. The program conducts a search only in the indicated sources (see help section manager sources). "
              "In case if the program will add packages, a section will appear the Additional packages, two variants will"
              " be offered to you:<ul>"
              "<li>Remove all  packages - will be remove indicated in sections chosen and additional.</li>"
              "<li>Remove only chosen packages - which are indicated in a section Selected will be remove, in this case can"
              " be negativnand consequences (and can be not): some programs can be not loaded, or work incorrectly,"
              " in the case of delete the choice of this option can lead to collapse of the operating system.</li></ul>");;
        }
	
	helpTxt->setText(msg);
}


/*!
    \fn Add_Rem_dlg::slot_cancel()
 */
void Add_Rem_dlg::slot_cancel()
{
  int icode;
  QString s1, s2;
  if(mode_install)
  {
    s1 = tr("Abort install");
    s2 = tr("You already want to abort install process?");
  }
  else
  {
    s1 = tr("Abort remove");
    s2 = tr("You already want to abort remove process?");
  }
  
  icode=QMessageBox::question(this,s1,s2,tr("&Yes"), tr("&No"));
     
    if(!icode) 
      emit terminate_rpm();
}

/*!
    \fn Add_Rem_dlg::closeEvent ( QCloseEvent * e )
 */
void Add_Rem_dlg::closeEvent ( QCloseEvent * e )
{
	e->ignore();
}


/*!
    \fn Add_Rem_dlg::slot_finish()
 */
void Add_Rem_dlg::slot_finish()
{
	emit finish_dlg(false);
}


/*!
    \fn Add_Rem_dlg::slot_progress_checked(int curr, int total)
 */
void Add_Rem_dlg::slot_progress_checked(int curr, int total)
{
	total_progress->setProgress(curr, total);
	qApp->processEvents();
}


/*!
    \fn Add_Rem_dlg::slot_message(const QString& )
 */
void Add_Rem_dlg::slot_message(const QString& report_msg)
{
	log_txt->setText(report_msg);
}


/*!
    \fn Add_Rem_dlg::slot_close()
 */
void Add_Rem_dlg::slot_close()
{
	close();
	if(incr >= 1)
		emit finish_dlg(false); //if install or rem process then update rpm tree
	else
		emit finish_dlg(true);  //else no
	
}
