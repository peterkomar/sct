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
 
#ifndef QINFOPANEL_H
#define QINFOPANEL_H

#include "bottom.h"
#include "rpm_manager.h"

class QInfoPanel: public info_panel {
Q_OBJECT
public:
    QInfoPanel(QWidget *parent = 0, const char *name = 0);
    void show_info_package(infoRpm rpm_data);
private:
    void format_description(QString& s);
		QString format_description();
    QString format_log();
    QString format_dep();
    QString format_content();
		
		infoRpm data;
private slots:
    void slotChanged( QWidget * );
};

#endif
