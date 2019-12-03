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

class gnomonViewForm;

class gnomonInteractorStylePrivate
{
    public:
        QString mode;

    public:
        gnomonViewForm *view;

    public:
        QMap<QShortcut *, QString> keymap;

    public slots:
        void setMode(QString);

    public:
        gnomonInteractorStylePrivate(void);
};

//
// gnomonInteractorStyle_p.h ends here
