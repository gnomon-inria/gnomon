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

class gnomonOmeroObjectPrivate
{
public:
    int id;
    gnomonOmeroType::type type;
};

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

int gnomonOmeroObject::id(void)
{
  return d->id;
}

//
// gnomonOmeroObject.cpp ends here
