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
}

gnomonToolBar::~gnomonToolBar(void)
{
    delete d;
}

//
// gnomonToolBar.cpp ends here
