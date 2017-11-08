
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
  QList<gnomonOmeroImagePtr> listImage;

  omero::model::DatasetLinkedImageSeq images = e->ref2omero->linkedImageList();

  for(int i=0; i< images.size(); i++)
  {
    omero::model::ImagePtr img = images[i];
    gnomonOmeroImagePtr item = gnomonOmeroImagePtr(new gnomonOmeroImage(img));
    listImage << item;
  }

  return listImage;

}

int long gnomonOmeroDataset::id(void)
{
  int long datasetId = e->ref2omero->getId()->getValue();
  qWarning() << "datasetID: " << datasetId ;
  return datasetId;
}

QStringList gnomonOmeroDataset::annotation(void)
{

}
