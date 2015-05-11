#include <qapplication.h>
#include <qstring.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qeventloop.h>

//class QEventLoop;
#include "lilomain.h"


extern "C" void run_module( const QPixmap& pix)
{
        liloMain *cp = new liloMain(NULL/*recever*/);

        cp->show();
	cp->setIcon(pix);
	
	QApplication::eventLoop()->enterLoop();
	
	delete cp;
	
}

extern "C" QString group()
{
    return QString(QObject::tr("Boot config"));
}

extern "C" QString name()
{
    return QString(QObject::tr("lilo"));
}

extern "C" QString description()
{
    return QString(QObject::tr("Configure lilo loader "));
}

extern "C" QString pixmap()
{
    QString s = "sct_lilo.png";

    return s;
}

//end
