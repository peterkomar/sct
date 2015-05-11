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
#ifndef MYPARTITIONS_H
#define MYPARTITIONS_H

#include <qobject.h>
#include <qstringlist.h>

#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>

#include "datapart.h"
#include "str_tools.h"

/**
Get partitions information 

@author Peter Komar
*/
class myPartitions : public QObject
{
Q_OBJECT
public:
    myPartitions(QObject *parent = 0, const char *name = 0);

    ~myPartitions();
    QStringList disklist();
    QPtrList<myDataPartition> get_data();

private:
    void get_part();
    QString scan_part(QString );
    QString convertSize(int size);
    void addListData(myDataPartition& );
    
    QPtrList<myDataPartition> ListPart;
};

#endif
