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


#include "tissueActor.h"

tissueActor::~tissueActor(void)
{
    this->SetReferenceCount(0);
}

bool tissueActor::isVisible(void)
{
    return (bool)(this->GetVisibility());
}

void tissueActor::hide(void)
{
    this->VisibilityOff();
}

void tissueActor::show(void)
{
    this->VisibilityOn();
}

//
// tissueActor.cpp ends here
