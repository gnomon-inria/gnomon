// Version: $Id: 421ba1c166024ad9bbc87c7580fce648f036e19f $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:


#include "gnomonActor.h"

gnomonActor::~gnomonActor(void)
{
    this->SetReferenceCount(0);
}

bool gnomonActor::isVisible(void)
{
    return (bool)(this->GetVisibility());
}

void gnomonActor::hide(void)
{
    this->VisibilityOff();
}

void gnomonActor::show(void)
{
    this->VisibilityOn();
}

//
// gnomonActor.cpp ends here
