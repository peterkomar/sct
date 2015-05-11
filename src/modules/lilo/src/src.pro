TEMPLATE	= lib
LANGUAGE	= C++

TARGET = ../modules/sct_lilo

CONFIG	+= qt warn_on release

HEADERS	+= editliloconf.h \
	lilomain.h \
	mypartitions.h \
	propertiesitemimpl.h \
	typeitem.h 
	
SOURCES	+= editliloconf.cpp \
	glob.cpp \
	lilomain.cpp \
	mypartitions.cpp \
	propertiesitemimpl.cpp \
	typeitem.cpp \
	
FORMS	= lilo.ui \
	propertiesmain.ui \
	QTypeItem.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}




