
#include "gnomonOmeroDataset.h"
#include "gnomonOmeroObject_p.h"


class gnomonOmeroDatasetPrivate
{
public:
    omero::model::DatasetPtr ref2omero;

};

gnomonOmeroDataset::gnomonOmeroDataset(void) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::dataset;
  e = new gnomonOmeroDatasetPrivate();
  e->ref2omero = Q_NULLPTR;
}

gnomonOmeroDataset::gnomonOmeroDataset(omero::model::DatasetPtr ds) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::dataset;
  e = new gnomonOmeroDatasetPrivate();
  e->ref2omero = ds ;
}

gnomonOmeroDataset::~gnomonOmeroDataset(void)
{
  delete e;
}

gnomonOmeroType::type gnomonOmeroDataset::type(void)
{
  return gnomonOmeroType::dataset;
}

QList<gnomonOmeroImagePtr> gnomonOmeroDataset::images(void)
{

}
