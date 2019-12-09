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
        QMap<int, QString> keymap;

    public:
        gnomonInteractorStylePrivate(void);

    public:
        int upperCase(int keycode);
};

//
// gnomonInteractorStyle_p.h ends here
