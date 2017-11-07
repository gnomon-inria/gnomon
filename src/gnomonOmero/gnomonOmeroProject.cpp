
#include "gnomonOmeroProject.h"
#include "gnomonOmeroObject_p.h"

class gnomonOmeroProjectPrivate
{
public:
  omero::model::ProjectPtr ref2omero;

};

gnomonOmeroProject::gnomonOmeroProject(void) : gnomonOmeroObject()
{
  e       = new gnomonOmeroProjectPrivate;
  d->type = gnomonOmeroType::project;
  e->ref2omero = Q_NULLPTR;
}

gnomonOmeroProject::gnomonOmeroProject(omero::model::ProjectPtr proj) : gnomonOmeroObject()
{
  e       = new gnomonOmeroProjectPrivate;
  d->type = gnomonOmeroType::project;
  e->ref2omero = proj;
}

gnomonOmeroProject::~gnomonOmeroProject(void)
{
  delete d;
  // if(e->ref2omero)
  //   delete e->ref2omero;
  delete e;

}

gnomonOmeroType::type gnomonOmeroProject::type(void)
{
  return gnomonOmeroType::project;
}

QList<gnomonOmeroDataset*> gnomonOmeroProject::datasets(void)
{

}

QList<gnomonOmeroImage*> gnomonOmeroProject::images(void)
{

}
