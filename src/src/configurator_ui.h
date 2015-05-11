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
//
#ifndef MYCONFIMPL_H
#define MYCONFIMPL_H

#include "configurator.h"

#include <qstringlist.h>
#include <qptrlist.h>
#include <qsettings.h>

struct infoProg
{
  QString name; //name tool configure
  QString path; //path to tool
};

class myConfImpl: public configurator {
Q_OBJECT
public:
    myConfImpl(QWidget *parent = 0, const char *name = 0);
public slots:
    virtual void slotAply();
public slots:
    virtual void slotBSecurity();
public slots:
    virtual void slotBRoot();
public slots:
    virtual void slotBServices();
public slots:
    virtual void slotBSyslog();
public slots:
    virtual void slotBDate();
public slots:
    virtual void slotBUser();
public slots:
    virtual void slotBMail();
public slots:
    virtual void slotBFTP();
public slots:
    virtual void slotBNFS();
public slots:
    virtual void slotBSamba();
public slots:
    virtual void slotBApache();
public slots:
    virtual void slotBNetwork();
public slots:
    virtual void slotBPackage();
public slots:
    virtual void slotBPrinter();
public slots:
    virtual void slotBKeyB();
public slots:
    virtual void slotBSound();
public slots:
    virtual void slotBXSrv();
public slots:
    virtual void slotBHwrInfo();
public slots:
    virtual void slotBMount();
public slots:
    virtual void slotBrwsBoot();
    virtual void slotProxy();
    virtual void slotScaner();
    virtual void slotMouse();
    virtual void slotDNS();
    virtual void slotDHCP();
    virtual void slotAddSysMenu();
    virtual void slotAddDesktop();
    virtual void slotBLang();
    virtual void langChange(const QString& );
private:
    QString browseMetod();
    void aplyParameter();
    void readConfig();
	 bool bLang;
public:
  static void setEmptyConfig();
};

#endif
