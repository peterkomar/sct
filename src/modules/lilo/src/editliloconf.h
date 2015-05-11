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
#ifndef EDITLILOCONF_H
#define EDITLILOCONF_H

#include <qobject.h>

#include <qstringlist.h>
#include <qptrlist.h>
#include <qprocess.h>

/**
Metods edit the file lilo.conf

@author Peter Komar
*/

struct myMenuLiloData{
 QString label; //label
 QString Root; //root
 QString initrd; //initrd
 QString image; //image
 QString append; //added parameter
 QString mode; //video mode
 bool main; //set default or no
 QString type_os; //windows, linux, etc
};

struct theme_data{
  QString name;
  QString path_pixmap;
  QString path_data_file;
};

class EditLiloConf : public QObject
{
Q_OBJECT
public:
    EditLiloConf(QObject *parent = 0, const char *name = 0);

    ~EditLiloConf();
    QPtrList<myMenuLiloData> getBootMenu();
	QPtrList<theme_data> getListThemes();
    void getMainParameter(QString* dev_boot, QString *i_pause);
    QString getCurrentTheme();

    bool addNewOS(myMenuLiloData );
	bool editExistOS(myMenuLiloData ,QString );
    bool removeOS(QString );
    void setParameters(QString dev_boot, QString i_pase, QString name_theme);
private:
    void setAplPath(QString& path);
    void add_to_list(myMenuLiloData );
    QString getPause();
    QString getTheme();
    void getMenuData();
    void parse_lilo_conf_file();
    void setTheme(QString& str, QString name_theme, bool b_disable_timer);
    QString getDefaultOS();
    void copyFile(const char* cName);
    void run_lilo_command();
    void set_path_my_lilo_conf();
    void add_to_theme_list(theme_data );
protected:
    QPtrList<myMenuLiloData> listOS;
    QPtrList<theme_data> listThemes;
    QProcess *lilo;
    QString path_my_lilo_conf;
    QString default_OS;
signals:
    void sendOut(QString ,int );
public:
    static void Trim(QString& str, const char*);
    static void TrimLeft(QString& str, QString );
    static void TrimRight(QString& str, QString );	
};

#endif
