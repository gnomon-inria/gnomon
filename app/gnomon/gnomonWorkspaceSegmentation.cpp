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

#include "gnomonWorkspaceSegmentation.h"

class gnomonWorkspaceSegmentationPrivate
{
public:

};

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspaceSegmentationPrivate;
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation(void)
{
    delete d;
}

//
// gnomonWorkspaceSegmentation.cpp ends here
