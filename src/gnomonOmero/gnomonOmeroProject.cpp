
#include "gnomonOmeroProject.h"
#include "gnomonOmeroObject_p.h"

gnomonOmeroProject::gnomonOmeroProject(void) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::project;
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
