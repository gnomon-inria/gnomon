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

#include "gnomonInspector.h"

gnomonInspector::gnomonInspector(QWidget *parent) : QFrame(parent)
{

}

gnomonInspector::~gnomonInspector(void)
{
    this->SetReferenceCount(0);
}

bool gnomonInspector::isVisible(void)
{
    return (bool)(this->GetVisibility());
}

void gnomonInspector::hide(void)
{
    this->VisibilityOff();
}

void gnomonInspector::show(void)
{
    this->VisibilityOn();
}

//
// gnomonInspector.cpp ends here
