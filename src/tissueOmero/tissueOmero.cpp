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

#include "tissueOmero.h"

class tissueOmeroPrivate
{
};

tissueOmero::tissueOmero(void)
{
    this->d = new tissueOmeroPrivate;
}

tissueOmero::~tissueOmero(void)
{
    delete d;
}

//
// tissueOmero.cpp ends here
