// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonStyle.h"

QString gnomonStyleSheet(void)
{
    QFile file(":gnomonStyle/gnomonStyle.qss");

    if(!file.open(QIODevice::ReadOnly))
        return QString();

    QString contents = file.readAll();

    file.close();

    qDebug() << Q_FUNC_INFO << contents;

    return contents;
}

//
// gnomonStyle.cpp ends here
