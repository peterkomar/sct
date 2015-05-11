TEMPLATE	= app
LANGUAGE	= C++

TARGET = ../bin/sct_center

CONFIG	+= qt warn_on release

FORMS += configurator.ui \
         About.ui 

HEADERS += sct_center.h \
           configurator_ui.h \
           myqloader.h \
           str_tools.h \
           sct_views.h \
           about_ui.h 
SOURCES += sct_center.cpp \
           main.cpp \
           configurator_ui.cpp \
           myqloader.cpp \
           sct_views.cpp \
           about_ui.cpp \
           str_tools.cpp 

unix{
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
