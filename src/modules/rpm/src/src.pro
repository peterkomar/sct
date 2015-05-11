TEMPLATE	= app
LANGUAGE	= C++

TARGET = ../modules/rpm

CONFIG	+= qt warn_on release

FORMS += bottom.ui \
         finddlg.ui \
         selectsource.ui \
         addsrcdlg.ui \
         left_p.ui \
         rpmdlg.ui 

HEADERS += rpm_ui.h \
           str_tools.h \
           rpm_views.h \
           qinfopanel.h \
           rpm_manager.h \
           finddlgimpl.h \
           parcersource.h \
           selectsourceimpl.h \
           headerRpm.h \
           addsrcimpl.h \
           add_rem_dlg.h \
           sct_left_panel.h
	   
SOURCES += main.cpp \
           rpm_ui.cpp \
           rpm_views.cpp \
           qinfopanel.cpp \
           rpm_manager.cpp \
           finddlgimpl.cpp \
           parcersource.cpp \
           selectsourceimpl.cpp \
           str_tools.cpp \
           addsrcimpl.cpp \
           add_rem_dlg.cpp \
           sct_left_panel.cpp 
	   
LIBS += -lrpm 

unix {
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
}