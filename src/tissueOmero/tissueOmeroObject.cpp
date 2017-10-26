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
  d = new tissueOmeroObjectPrivate;
  d->id = -1;
  d->type = tissueOmeroType::none;

}

tissueOmeroObject::~tissueOmeroObject(void)
{
  delete d;
}

tissueOmeroObject::setId(int id)
{
  d->id = id;
}

int tissueOmeroObject::id(void)
{
  return d->id;
}

tissueOmeroType::type tissueOmeroObject::type(void)
{
  return d->type;
}

void tissueOmeroObject::setType(tissueOmeroType::type type)
{
  d->type = type;
}


//
// tissueOmeroObject.cpp ends here
