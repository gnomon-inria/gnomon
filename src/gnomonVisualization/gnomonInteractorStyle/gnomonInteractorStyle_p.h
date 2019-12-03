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

#pragma once

#include <QtCore>

class gnomonInteractorStylePrivate
{
    public:
        QString mode;

    public:
        QMap<int, QString> keymap;

    public slots:
        void setMode(QString);

    public:
        gnomonInteractorStylePrivate(void);
};

//
// gnomonInteractorStyle_p.h ends here
