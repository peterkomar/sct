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
#ifndef MYMOUNTMANAGER_H
#define MYMOUNTMANAGER_H

#include "datapart.h"

#include <qobject.h>
#include <qstringlist.h>
#include <qptrlist.h>
#include <qprocess.h>

/**
@author peter
*/
class myMountManager : public QObject
{
Q_OBJECT
public:
    myMountManager();

    ~myMountManager();
    QPtrList<myDataPartition> getPartition(bool update=true);
    int mountPartition(myDataPartition , bool b=false); //mounted part in expert mode
    int UmountPartition(const char* , bool , bool b=false);
    QString getErrorr();
    QString fast_mount(QString dev); //mounted part in normal mode, return path where to mounted
    QString readLocale();
private:
    QStringList getMountedPartitions();
    void addDataToFstab(myDataPartition );
    void removeDataFromFstab(QString );
    void loadData();
    void convertString(QString& str);
    QPtrList<myDataPartition> ListPart;
public slots:
    void slotDataErrorMount();
protected:
    QString errorString;
private:
    QString getRootFS();
    QString convertFS(QString );
    QProcess* pMount;
private:
    void get_umount_part();
};

#endif
