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

// int long gnomonOmeroObject::id(void)
// {
//   return d->id;
// }


//
// gnomonOmeroObject.cpp ends here
