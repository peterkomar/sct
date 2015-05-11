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
#include "sct_left_panel.h"
#include "str_tools.h"

#include <qtextedit.h>

SCT_Left_Panel::SCT_Left_Panel(QWidget *parent, const char *name)
  :Left_Panel(parent, name)
{
	update_info(0, "0 Byte");
}


/*!
    \fn SCT_Left_Panel::update_info(int count, const QString size_packages)
 */
void SCT_Left_Panel::update_info(int count_n, const QString size_packages)
{
	QString s = QString::number(count_n);
	
	QString font = "<font color=\"#55aa00\"><b>%1</b> : </font>";
	QString font2 = "<font size=+1 color=\"blue\"><b>%1</b> : </font><br><br><br>";
            
	QString s_msg = font2.arg(tr("Packages statistic"));
		
	s_msg += "<table border=0 width=100%>";
	      
	s_msg += "<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font.arg(tr("Selected"))+"</td>";
	s_msg += "<td bgcolor=\"#EEF6FF\" width=95%>"+s+"</td></tr>";
	
	s_msg+="<tr><td bgcolor=\"#EEF6FF\" width=5%>"+font.arg(tr("Size"))+"</td>";
	s_msg+="<td bgcolor=\"#EEF6FF\" width=95%>"+size_packages+"</td></tr>";
	
	s_msg +="</table><br><br>";
		
	s_msg += font2.arg(tr("Disk's statistic"));
	s_msg += "<font size=-2>";
	s_msg += "<table border=0 width=100%>";
	s_msg += QString("<tr bgcolor=\"lightGray\"><th>%1</th><th>%2</th><th>%3</th><th>%4</th></tr>")
			.arg(tr("Device")).arg(tr("Size")).arg(tr("Available")).arg(tr("Mounted to"));
	
	s_msg += get_disk_statistic();
	s_msg += "</table>";
	
	info_stat->setText(s_msg);
}
