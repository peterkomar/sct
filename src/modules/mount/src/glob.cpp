#include <qapplication.h>
#include <qstring.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qeventloop.h>

//class QEventLoop;
#include "mount_ui.h"


extern "C" void run_module( const QPixmap& pix)
{
        mount_ui *cp = new mount_ui(NULL/*recever*/);

        cp->show();
	cp->setIcon(pix);
	
	QApplication::eventLoop()->enterLoop();
	
	delete cp;
	
}

extern "C" QString group()
{
    return QString(QObject::tr("Hard Disk"));
}

extern "C" QString name()
{
    return QString(QObject::tr("mount"));
}

extern "C" QString description()
{
    return QString(QObject::tr("Mount partitions"));
}

extern "C" QString pixmap()
{
    QString s = "sct_mount.png";

    return s;
}

//end
