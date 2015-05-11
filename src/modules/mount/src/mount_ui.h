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
#include <qhbox.h>
#include <qlayout.h>

#include "mymountmanager.h"

class QListView;
class QListViewItem;
class QAction;
class QPoupMenu;
class QIconView;
class QLabel;
class QTextEdit;
class QPushButton;
class QVBox;
class QIconViewItem;

class mount_ui: public QMainWindow {
Q_OBJECT
public:
	mount_ui(QWidget *parent = 0, const char *name = 0);
private:
    void create_gui();
    void init_connections();
    void load_data();
    void load_menu();
private slots:
    void getData(myDataPartition data);
		void getDataUmount(bool );
    void slot_mount();
    void slot_rescan();
    void slot_umount();
public slots:
	  void slotViewMenu(QListViewItem* ,const QPoint& pt);
		virtual void fileExit();
    virtual void helpAbout();
    virtual void helpIndex();
private:
    myMountManager *myManager;
    QAction *mountAct;
    QAction *umountAct;
    QPopupMenu *menu;
    QListView *ex_list_part;
    QIconView *list_part;
    QTextEdit *help_lab;
    
    bool b_view_ex;
    bool b_write_fstab;
		QPtrList<myDataPartition> p_list_part;
    
    //buttons
    QPushButton *mountBtn;
    QPushButton *umountBtn;
    QPushButton *rescanBtn;
    
    //layouts
    QHBox *box;
    QVBox *vbox;
    QGridLayout *view;
protected:
    QAction *expert_w;
    QAction *write_fstab;
private slots:
    void slot_view(bool );
    void slotOnItem(QIconViewItem* );
    void slotOnViewPort();
    void toggle_write(bool );
		void slotViewMenu(QIconViewItem* ,const QPoint& pt);
protected:
    virtual void paintEvent ( QPaintEvent * );
private:
	void load_tree_data();
  void load_view_data();
  myDataPartition get_data_part(const QString& );
    void show_context_help();
    static QString get_locale();
};

#endif
