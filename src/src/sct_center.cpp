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


#include "sct_center.h"
#include "configurator_ui.h"
#include "about_ui.h"
#include "configure.xpm"

#include <qmenubar.h>
#include <qprocess.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qdir.h>
#include <qsettings.h>
#include <qdockwindow.h>
#include <qlabel.h>

myconfcenter::myconfcenter()
    : QMainWindow( 0, "sctcontrolcenter", WDestructiveClose )
{
	setIcon(configure_xpm);
        	
	loadMenu();
		
	mainW = new myconfviews(this);
  	setCentralWidget(mainW);
	
	init_connections();
	
	readSetings();
        
        QDockWindow *dock = new QDockWindow(QDockWindow::InDock, this);
        //dock->setResizeEnabled(TRUE);
        dock->setHorizontallyStretchable(TRUE);
        dock->setMovingEnabled (FALSE);
                
        //QHBox *bg = new QHBox(dock);
        
        QPixmap pix, pix_backgr,pix_grad;
        QString path; // = qApp->applicationDirPath();
        setAplDir(path);
        path += "/pixmaps/";
  
  
        pix.load(path+"top.png");
        pix_backgr.load(path+"baground_top.png");
        pix_grad.load(path+"gradient.png");
        
        QLabel *lab = new QLabel(dock);
        lab->setPixmap(pix);
        //lab->setScaledContents(TRUE);
        dock->setWidget(lab);
        lab->setFrameShape(QFrame::TabWidgetPanel);
        lab->setBackgroundPixmap(pix_backgr);
        statusBar()->setBackgroundPixmap(pix_grad);
        setPaletteBackgroundColor(QColor("#EDEDEC"));
        menuBar()->setPaletteBackgroundColor(QColor("#EDEDEC"));
                
        uid_t uid = geteuid();
        if(uid != 0)
          QMessageBox::information(this, tr("Information"),
                                   tr("You execute control center not like root.\nSomes programs and modules maybe not correct run."));
        
        
        statusBar()->message( tr("Ready"), 2000 );
				
	//resize(800, 581 );
}


myconfcenter::~myconfcenter()
{
	writeSetings();
	
	//free memory
	delete detalv;
	delete mainW;	
}


void myconfcenter::about()
{
	About_Ui* dlg = new About_Ui(this);
  
  dlg->exec();
  
  delete dlg;
}


void myconfcenter::aboutQt()
{
    QMessageBox::aboutQt( this, tr("SCT use QT") );
}


/*!
    \fn myconfcenter::loadMenu()
 */
void myconfcenter::loadMenu()
{
	QPopupMenu *menuFile = new QPopupMenu(this);
	QPopupMenu *menuView = new QPopupMenu(this);
	QPopupMenu *menuHelp = new QPopupMenu(this);
        
        menuFile->setPaletteBackgroundColor(QColor("#EDEDEC"));
        menuView->setPaletteBackgroundColor(QColor("#EDEDEC"));
        menuHelp->setPaletteBackgroundColor(QColor("#EDEDEC"));
	
	menuBar()->insertItem(tr("File"), menuFile);
	menuBar()->insertItem(tr("View"), menuView);
	menuBar()->insertItem(tr("Help"), menuHelp);
		
	menuFile->insertItem(tr("Preferencess"),this,SLOT(showPreferences()));
	menuFile->insertSeparator(1);
	menuFile->insertItem(tr("Exit"), this, SLOT(exit()));
	
	detalv = new QAction(tr("Detailed View"),tr("&Detailed View"),0, this,tr("Detailed View"));
	detalv->setToggleAction(TRUE);
	detalv->addTo(menuView);
	
	menuHelp->insertItem(tr("Help"), this, SLOT(index()));
	menuHelp->insertSeparator(1);
	menuHelp->insertItem(tr("About"),this, SLOT(about()));
}


/*!
    \fn myconfcenter::readSetings()
 */
void myconfcenter::readSetings()
{
	QSettings settings;
  
	QDir d = QDir::home();
  
	if(d.exists(".SCT"))
	{
		d.cd(".SCT");
                QFile f(d.absPath()+"/.sctrc.lock");
                
                if(!f.exists())
                {
                   myConfImpl::setEmptyConfig();
                }
	}
	else
	{
		myConfImpl::setEmptyConfig();
	}
  
  
	QString s = d.absPath();
  
	settings.removeSearchPath( QSettings::Unix, s+"/.qt");
	settings.insertSearchPath( QSettings::Unix, s );
  
	iviewr = settings.readNumEntry(APP_KEY+"ViewMode",1);
	  
	if(iviewr == 0)
	{
		updateGroupRightType(false);
		detalv->setOn(FALSE);
	}
	else
	{
		updateGroupRightType(true);
		detalv->setOn(TRUE);
	}
}


/*!
    \fn myconfcenter::writeSetings()
 */
void myconfcenter::writeSetings()
{
	QString pathApp; // = qApp->applicationDirPath();
	setAplDir(pathApp);
	
	QDir d;
	
	QSettings settings;
	d = QDir::home();
        QString s = d.absPath();
  
	settings.removeSearchPath( QSettings::Unix, s+"/.qt");
	settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
  
	settings.writeEntry(APP_KEY+"ViewMode",iviewr);
	
	settings.writeEntry(APP_KEY+"Install_Dir", pathApp);
}


/*!
    \fn myconfcenter::msgClr()
 */
void myconfcenter::msgClr()
{
  statusBar()->message(tr("Ready"));
}


/*!
    \fn myconfcenter::msg(QString )
 */
void myconfcenter::msg(const QString& name)
{
  statusBar()->message(tr("Please wait. %1 is loading ...").arg(name));
}

/*!
    \fn myconfcenter::msg(QString )
 */
void myconfcenter::msg_info(const QString& name)
{
  statusBar()->message(tr("\t%1 ").arg(name));
}

/*!
    \fn myconfcenter::updateGroupRightType(QAction* action)
 */
void myconfcenter::updateGroupRightType(bool b)
{
  iviewr = (int)b;
  mainW->setFullModeView(b);
}


/*!
    \fn myconfcenter::index()
 */
void myconfcenter::index()
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
        
  s+="/doc/"+get_locale()+"/index.html";
  
  proc.addArgument(s);
  
  if(!proc.start())
    QMessageBox::information(this,tr("Info"),tr("Can't find help system."));
  
}

/*!
    \fn myconfcenter::showPreferences()
 */
void myconfcenter::showPreferences()
{
	myConfImpl *conf = new myConfImpl(this);
     
	conf->exec();
     
	delete conf;
}


/*!
    \fn myconfcenter::exit()
 */
void myconfcenter::exit()
{
	close();
}


/*!
    \fn myconfcenter::init_connections()
 */
void myconfcenter::init_connections()
{
  connect(detalv, SIGNAL(toggled(bool )),this, SLOT(updateGroupRightType(bool )));
  connect(mainW, SIGNAL(statusMsg(const QString& )),this, SLOT(msg(const QString& )));
  connect(mainW, SIGNAL(status_msg_info(const QString& )),this, SLOT(msg_info(const QString& )));
  connect(mainW, SIGNAL(msgClr()), this, SLOT(msgClr()));
}


/*!
    \fn myconfcenter::get_locale()
 */
QString myconfcenter::get_locale()
{
  QDir d = QDir::home();
  
  QString s = d.absPath();
	
  QSettings settings;
	 
  settings.removeSearchPath( QSettings::Unix, s+"/.qt");
  settings.insertSearchPath( QSettings::Unix, s+"/.SCT" );
   
  QString slang = settings.readEntry("/SCT/Language_UI","en"); 
  
  return slang;
}
