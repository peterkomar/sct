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
 
#ifndef MOUNT_UI_H
#define MOUNT_UI_H

#include <qmainwindow.h>
#include <qpixmap.h>

#include "rpm_views.h"

class QAction;
class QPoupMenu;
class QToolBar;
class QToolButton;

class rpm_ui: public QMainWindow {
Q_OBJECT
public:
	rpm_ui(bool i = false, QWidget *parent = 0, const char *name = 0);
  static QString get_locale();
private:
    void greate_gui();
    void init_connections();
    void load_menu();
    void set_mode();//set mode installed or nor not installed
    bool loadIcons();
    void set_par(QToolButton* );
    //actions
    QAction *installed_act;
    QAction *uninstalled_act;
    QAction *reload_act;
    QAction *find_act;
    QPopupMenu *menu_rpm;
    QToolBar *irpmTools;
    QToolBar *urpmTools;
    //tool buttons
    QToolButton *inst_act;
    QToolButton *rem_act;
    QToolButton *find_btn;
    QToolButton *edit_src_act;
    //icons
    QPixmap pixIns;
    QPixmap pixUnist;
    QPixmap pixFind;
    QPixmap pixEditSrc;
    //variables
    RPM_Views *rpm_main;
    bool b_installed;
    uid_t uid;
public slots:
    virtual void fileExit();
    virtual void helpAbout();
    virtual void helpIndex();
private slots:
    void slot_mode(QAction* );
    void slotMessageStatus(const QString& message);
};

#endif
