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
  // if(e->ref2omero)
  //   delete e->ref2omero;
  delete e;

}

gnomonOmeroType::type gnomonOmeroProject::type(void)
{
  return gnomonOmeroType::project;
}

QString gnomonOmeroProject::name(void)
{
    if( e->ref2omero )
        return QString::fromStdString(e->ref2omero->getName()->getValue());
    else
        return QString();
}

QList<gnomonOmeroDatasetPtr> gnomonOmeroProject::datasets(void)
{
  QList<gnomonOmeroDatasetPtr> listDataset;
  omero::model::ProjectLinkedDatasetSeq datasets = e->ref2omero->linkedDatasetList();

  for(int i=0; i< datasets.size(); i++)
  {
    omero::model::DatasetPtr ds = datasets[i];
    gnomonOmeroDatasetPtr item = gnomonOmeroDatasetPtr(new gnomonOmeroDataset(ds));
    listDataset << item;

  }

  return listDataset;
}
