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

#include "gnomonToolBar.h"

class gnomonToolBarPrivate
{
public:
};

gnomonToolBar::gnomonToolBar(QWidget *parent) : QFrame(parent)
{
    d = new gnomonToolBarPrivate;

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

gnomonToolBar::~gnomonToolBar(void)
{
    delete d;
}

QSize gnomonToolBar::sizeHint(void) const
{
    return QSize(200, 32);
}

//
// gnomonToolBar.cpp ends here
