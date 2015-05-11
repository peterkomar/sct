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

#ifndef ADD_REM_DLG_H
#define ADD_REM_DLG_H

#include "rpmdlg.h"

class Add_Rem_dlg: public rpm_dlg {
Q_OBJECT
public:
    Add_Rem_dlg(QWidget *parent = 0, const char *name = 0);
    void set_add_mode(bool install=true);
private:
    void set_gui_parameter();
    void load_help();
		
    //members
    int incr;
    bool allmatch;
    bool mode_install;
public slots:
    void slot_get_first_report(const QString& report_msg, bool berr );
    void slot_current_all_items(int , const QString& );
    void slot_current_parcent_rpm(int );
    void slot_finish_rpm(const QString& , bool );
    void slot_count_all_items(int );
    
    virtual void slot_rem_mode(int );
    void slot_cancel();
    void slot_finish();

protected:
    virtual void closeEvent ( QCloseEvent * e );
private slots:
    void begin_rpm();
    void slot_progress_checked(int curr, int total);
    void slot_message(const QString& );
    void slot_close();
signals:
    void start_rpm(bool allmatches, bool install_mode);
    void terminate_rpm();
    void finish_dlg(bool ); //if user click cancel button then bool = true
};

#endif
