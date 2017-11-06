
#include "gnomonOmeroDataset.h"
#include "gnomonOmeroObject_p.h"

gnomonOmeroDataset::gnomonOmeroDataset(void) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::dataset;
}


gnomonOmeroType::type gnomonOmeroDataset::type(void)
{
  return gnomonOmeroType::dataset;
}

QList<gnomonOmeroImage*> gnomonOmeroDataset::images(void)
{

}
