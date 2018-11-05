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

#include "gnomonMainWindow.h"

class gnomonMainWindowPrivate
{
public:
};

gnomonMainWindow::gnomonMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonMainWindowPrivate;
}

gnomonMainWindow::~gnomonMainWindow(void)
{
    delete d;
}

//
// gnomonMainWindow.cpp ends here
