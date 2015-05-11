TEMPLATE	= lib
LANGUAGE	= C++

TARGET = ../modules/sct_mount

CONFIG	+= qt warn_on release

FORMS += mymntparameter.ui \
         selectmode.ui 
 
HEADERS += datapart.h \
           myeditmountimpl.h \
           mymountmanager.h \
           mypartitions.h \
           myselectmodeimpl.h \
           mount_ui.h \
           str_tools.h 
SOURCES += glob.cpp \
           myeditmountimpl.cpp \
           mymountmanager.cpp \
           mypartitions.cpp \
           myselectmodeimpl.cpp \
           mount_ui.cpp

unix {
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
}
