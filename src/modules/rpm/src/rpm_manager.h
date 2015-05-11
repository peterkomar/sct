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
 
#ifndef RPM_MANAGER_H
#define RPM_MANAGER_H

#include <qobject.h>
#include <qapplication.h>
#include <qprocess.h>
#include <qdatetime.h>

//rpm headers
#include <rpm/rpmdb.h>
#include <rpm/rpmio.h>
#include <rpm/rpmlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpm/rpmts.h>
#include <rpm/rpmps.h>
#include <rpm/rpmmacro.h>

#include "headerRpm.h"
#include "str_tools.h"


/**
SCT rpm manger

	@author peter komar <markus_sksoft@mail.ru>
*/
class rpm_manager : public QObject
{
Q_OBJECT
public:
    rpm_manager(QObject *parent = 0, const char *name = 0);

    ~rpm_manager();
    void get_i_packages();
    void get_u_packages();
    QPtrList<infoRpm> getListRpm();
    QPtrList<infoRpm> getListURpm(bool full = false);
    void write_media_data();
    int add_rpm_media(const QString& name, const QString& path_rpm, bool brem, bool b_on);
    void rem_rpm_media(const QString& name);
    void edit_rpm_media(const QString& nameMedia, bool removable, bool b_on, const QString& oldName = "none");
    bool find_item(const QString& ,bool installed = true);
    void set_path_src(const QString& );
    void reset_scaler();
    int prepar_add_rpm_src(const QString& path)/*return count all files*/;
    QPtrList<infoRpm> get_tmp_list();
    int preparing_rpm(const QStringList& listRpm, bool binstall=true); //return 1 if allmatches
		/*QString convertSize(int );*/
private:
	//members
    QPtrList<infoRpm> listRpm;
    QPtrList<infoRpm> listUnRpm;
    QPtrList<infoRpm> tmp_list;
    QPtrList<add_rem_data> list_rpm; //list instaled or rem packages
    QString name_src;
    QString path_src;
    int i_scaler;
    QProcess *p_rpm;
    bool b_termit;
	
	//metods
    infoRpm get_info_rpm(Header h);
    void format_descr(QString& );
    
    bool get_symbol_compare(QString& name,QString& symbol, QString& version);//compare version package
    void get_type_item(const QString& ,char* type)/*return type depend*/;
    bool find_in_package_version(const QString& , infoRpm* data=NULL);
    bool check_preparing_remove(add_rem_data* , QString& );
    bool unleash_depends(add_rem_data* , QString& );
    bool find_in_inst_list(infoRpm* , bool in_tmp_list = true);//find in tmp_list or listUnRpm package
    void preparing_install_packages(const QStringList& );
    void preparing_remove_packages(const QStringList& );
    void rpm_i_v_h(bool b_all);
    void rpm_e(bool ball);
    bool processing_out_rpm(const QString& name_rpm, bool red_all=false);
    
    QString set_path_name_rpm(infoRpm* );
    
    //bool copyTmpDirFiles(QStringList listFiles);
    //void removeTmpDirFiles();
        
    //void display_list(QStringList& list);//debug metod show display list
    
    bool add_to_add_rpm_list(infoRpm );
    bool add_to_rem_rpm_list(infoRpm );
    bool add_to_tmp_rpm_list(infoRpm );
    bool add_to_rpm_list(infoRpm ,bool b_additional=false); //add to list packages like install or remove
    
    template <class type1, class type2>
        bool find_in_list(type1 *item, type2& list);
           
private slots:
    void start_process(bool allmatches, bool install_mode);
    void terminate_process();
    void abort_added();
signals:
    ///signals installation and removes packages  
    void first_report(const QString&  , bool ); //send report of depends packages if bool = true added additional packages
						    
    void count_all_items(int ); //send total packages like added to install or remove
    void current_all_items(int , const QString& ); //send current progress operation and name package like added or removes
    void current_parcent_rpm(int ); //send current progress added or removes operation
    void finish_rpm(const QString& , bool ); //send report installation after end. If bool = true ok else - fail operation
  
    ///signals add media  
    void send_cur_add(int );
    void send_cur_add_str(QString ,int code);
    void send_total_add(int );
    ///send parcer read data base rpm
    void send_parcent( int ,int );
    ///send parcent preparing remove
    void send_check_parcent(int, int);
    ///send message to rpm_dlg
    void send_message(const QString& );
};

#endif
