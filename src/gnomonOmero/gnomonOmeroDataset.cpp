
#include "gnomonOmeroDataset.h"

class gnomonOmeroDatasetPrivate
{
public:

};

gnomonOmeroDataset::gnomonOmeroDataset(void)
{
    d = new gnomonOmeroDatasetPrivate;

}

gnomonOmeroDataset::~gnomonOmeroDataset(void)
{
    delete d;
}

QList<gnomonOmeroImage*> gnomonOmeroDataset::images(void)
{

}

gnomonOmeroType::type gnomonOmeroDataset::type(void)
{
  return gnomonOmeroType::dataset;
}
