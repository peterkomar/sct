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
#ifndef PROPERTIESITEMIMPL_H
#define PROPERTIESITEMIMPL_H

#include "propertiesmain.h"
#include "editliloconf.h"

class propertiesItemImpl: public PropertiesMain {
Q_OBJECT
public:
	propertiesItemImpl(myMenuLiloData data, bool bNew = true, QWidget *parent = 0, const char *name = 0);
signals:
    void sendData(myMenuLiloData ,QString );
private:
	void loadData(myMenuLiloData& data, bool bNew);
	QString oldLabel;
public slots:
    virtual void slotFinish();
};

#endif
