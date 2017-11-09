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

#include "gnomonOmeroObject.h"
#include "gnomonOmeroObject_p.h"

gnomonOmeroObject::gnomonOmeroObject(void)
{
    d       = new gnomonOmeroObjectPrivate;
    d->id   = -1;
    d->type = gnomonOmeroType::none;
}

gnomonOmeroObject::~gnomonOmeroObject(void)
{
  delete d;
}


//
// gnomonOmeroObject.cpp ends here
