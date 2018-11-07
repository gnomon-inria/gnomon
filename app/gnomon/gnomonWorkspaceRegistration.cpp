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

#include "gnomonWorkspaceRegistration.h"

class gnomonWorkspaceRegistrationPrivate
{
public:

};

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspaceRegistrationPrivate;
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    delete d;
}

//
// gnomonWorkspaceRegistration.cpp ends here
