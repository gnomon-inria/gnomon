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

#include "tissueOmeroObject.h"

class tissueOmeroObjectPrivate
{
public:
    int id;
    tissueOmeroType::type type;
};

tissueOmeroObject::tissueOmeroObject(void)
{
    d       = new tissueOmeroObjectPrivate;
    d->id   = -1;
    d->type = tissueOmeroType::none;
}

tissueOmeroObject::~tissueOmeroObject(void)
{
  delete d;
}

void tissueOmeroObject::setId(int id)
{
  d->id = id;
}

int tissueOmeroObject::id(void)
{
  return d->id;
}

tissueOmeroType::type tissueOmeroObject::type(void)
{
    if ( d->id == -1)
        return tissueOmeroType::none;
    return d->type;
}

void tissueOmeroObject::setType(tissueOmeroType::type type)
{
    d->type = type;
}

bool tissueOmeroObject::isProject(void)
{
    if (d->id == -1)
        return false;

    return d->type == tissueOmeroType::project;
}

bool tissueOmeroObject::isDataset(void)
{
    if (d->id == -1)
        return false;

    return d->type == tissueOmeroType::dataset;
}

bool tissueOmeroObject::isImage(void)
{
    if (d->id == -1)
        return false;

    return d->type == tissueOmeroType::image;
}

//
// tissueOmeroObject.cpp ends here
