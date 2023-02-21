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
        QString mode = "3D";

    public:
        gnomonViewForm *view = nullptr;

    public:
        QMap<int, QString> keymap;

    public:
        QMetaObject::Connection connect3D;
        QMetaObject::Connection connect2D;

    public:
        gnomonInteractorStylePrivate(void);

    public:
        int upperCase(int keycode);
};

//
// gnomonInteractorStyle_p.h ends here
