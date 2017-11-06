
#include "gnomonOmeroProject.h"

class gnomonOmeroProjectPrivate
{
public:

};

gnomonOmeroProject::gnomonOmeroProject(void)
{
    d = new gnomonOmeroProjectPrivate;

}

gnomonOmeroProject::~gnomonOmeroProject(void)
{
    delete d;
}

QList<gnomonOmeroDataset*> gnomonOmeroProject::datasets(void)
{

}

QList<gnomonOmeroImage*> gnomonOmeroProject::images(void)
{

}

inline gnomonOmeroType::type gnomonOmeroProject::type(void)
{
  return gnomonOmeroType::project;
}
