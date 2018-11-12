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

#include "gnomonWorkspace.h"

class gnomonWorkspacePrivate
{
public:

};

gnomonWorkspace::gnomonWorkspace(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspacePrivate;
}

gnomonWorkspace::~gnomonWorkspace(void)
{
    delete d;
}

//
// gnomonWorkspace.cpp ends here
