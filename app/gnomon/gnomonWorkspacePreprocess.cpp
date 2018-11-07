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

#include "gnomonWorkspacePreprocess.h"

class gnomonWorkspacePreprocessPrivate
{
public:

};

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspacePreprocessPrivate;
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

//
// gnomonWorkspacePreprocess.cpp ends here
