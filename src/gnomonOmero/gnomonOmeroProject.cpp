
#include "gnomonOmeroProject.h"
#include "gnomonOmeroObject_p.h"

class gnomonOmeroProjectPrivate
{
public:
  omero::model::ProjectPtr ref2omero;
  omero::model::DatasetPtr ref2omeroDataset;

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
  qWarning() << "aaaa" << e->ref2omero;

}

gnomonOmeroProject::~gnomonOmeroProject(void)
{
  // if(e->ref2omero)
  //   delete e->ref2omero;
  delete e;

}

gnomonOmeroType::type gnomonOmeroProject::type(void)
{
  return gnomonOmeroType::project;
}

QList<gnomonOmeroDatasetPtr> gnomonOmeroProject::datasets(void)
{
  qWarning() << "kkkkk" << e->ref2omero;
  QList<gnomonOmeroDatasetPtr> listDataset;
  omero::model::ProjectLinkedDatasetSeq datasets = e->ref2omero->linkedDatasetList();

  for(int i=0; i< datasets.size(); i++)
  {
    omero::model::DatasetPtr ds = datasets[i];
    gnomonOmeroDatasetPtr item = gnomonOmeroDatasetPtr(new gnomonOmeroDataset(ds));
    // std::string datasetName = ds->getName()->getValue();
    // qWarning() << "OMERO: Dataset: " << datasetName.data();
    listDataset << item;

  }

  return listDataset;
}

QList<gnomonOmeroImagePtr> gnomonOmeroProject::images(void)
{
  // QList<gnomonOmeroImagePtr> listImage;
  //
  // omero::model::DatasetLinkedImageSeq images = e->ref2omeroDataset->linkedImageList();
  //
  // for(int i=0; i< images.size(); i++)
  // {
  //   omero::model::ImagePtr img = images[i];
  //   gnomonOmeroImagePtr item = gnomonOmeroImagePtr(new gnomonOmeroImage(img));
  //   // std::string imgName = img->getName()->getValue();
  //   // int long imgid = img->getId()->getValue();
  //   // qWarning() << "OMERO: Image: ID: " << imgid ;
  //   listImage << item;
  // }
  //
  // return listImage;
}
