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

void gnomonOmeroObject::setId(int id)
{
  d->id = id;
}

int gnomonOmeroObject::id(void)
{
  return d->id;
}

gnomonOmeroType::type gnomonOmeroObject::type(void)
{
    if ( d->id == -1)
        return gnomonOmeroType::none;
    return d->type;
}

void gnomonOmeroObject::setType(gnomonOmeroType::type type)
{
    d->type = type;
}

bool gnomonOmeroObject::isProject(void)
{
    if (d->id == -1)
        return false;

    return d->type == gnomonOmeroType::project;
}

bool gnomonOmeroObject::isDataset(void)
{
    if (d->id == -1)
        return false;

    return d->type == gnomonOmeroType::dataset;
}

bool gnomonOmeroObject::isImage(void)
{
    if (d->id == -1)
        return false;

    return d->type == gnomonOmeroType::image;
}

//
// gnomonOmeroObject.cpp ends here
