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

#ifndef LISTVIEWS_H
#define LISTVIEWS_H

#include <qsplitter.h>
#include <qpixmap.h>

#include "str_tools.h"
#include "qinfopanel.h"
#include "sct_left_panel.h"

#include "finddlgimpl.h"

class QListView;
class QLabel;
class QTextEdit;
class QListViewItem;
class QProgressDialog;
class Add_Rem_dlg;

// -----------------------------------------------------------------



// -----------------------------------------------------------------

class RPM_Views : public QSplitter
{
    Q_OBJECT

public:
	RPM_Views( QWidget *parent = 0, const char *name = 0 );
	~RPM_Views()
    {}
	void build_tree(bool bInst = true);

protected:
	QListView *rpm_list, *group_list;
	SCT_Left_Panel *l_panel;
	QSplitter *vsplitter, *sd;
	QInfoPanel *b_panel;
private:
       //members
        rpm_manager *manager;
        finddlgImpl *find_dlg;
        QProgressDialog *scr;
	
        Add_Rem_dlg *rpm_dlg;
					
        QPixmap pix_p;
        QPixmap pix_p_apl;
        QPixmap pix_p_fav;
        QPixmap pix_p_games;
        QPixmap pix_p_multimedia;
        QPixmap pix_p_network;
        QPixmap pix_p_settings;
        QPixmap pix_p_toys;
        QPixmap pix_p_util;
        QPixmap pix_rpm;
        QPixmap pix_p_development;
        QPixmap pix_p_editors;
        QPixmap pix_p_edutainment;
        QPixmap pix_p_games_arcade;
        QPixmap pix_p_games_board;
        QPixmap pix_p_games_card;
        QPixmap pix_p_games_strategy;
        QPixmap pix_p_misc;
        QPixmap pix_p_security;
        QPixmap pix_p_shells;
        QPixmap pix_p_system;
        QPixmap pix_p_graphics;
		
        QStringList listRPM;
        bool b_mode_inst_pkgd;
        
        //metods
        bool load_icons();
        QPixmap set_group_icon(QString& name);
        QListViewItem* findItem(QString name, QListViewItem* item, int c, QListView *parent);
        void init_conections();
        bool find_in_list(const QString& s);
        infoRpm* get_data_rpm(const QString& );
    
    	//QPixmap create_spash_dialog(QColor& color_text);
        void calculate_status_panel();
        void init_local_connections();
        void rem_local_connections();

private slots:
    void slot_sel_group(QListViewItem * item );
    void slot_clicked( QListViewItem * item, const QPoint & pnt, int c );
    void slot_get_find(find_struct );
    void slot_select(QListViewItem* );
    void slotPressSpace(QListViewItem* );
    void slot_updateTree();
    void slot_finish_rpm_dialog(bool );
    void slot_find(bool );
    void slot_edit_src();
    void slot_install();
    void slot_notinstall();
    void get_parcent(int ,int );
signals:
    void sendMessageStatus(const QString& message);
};

#endif
