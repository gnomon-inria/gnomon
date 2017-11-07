#include "gnomonOmeroImage.h"
#include "gnomonOmeroObject_p.h"

class gnomonOmeroImagePrivate
{
public:
  omero::model::ImagePtr ref2omero;

};

gnomonOmeroImage::gnomonOmeroImage(void) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::image;

}

gnomonOmeroImage::gnomonOmeroImage(omero::model::ImagePtr img) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::image;
  e = new gnomonOmeroImagePrivate();
  e->ref2omero = img ;
}

gnomonOmeroImage::~gnomonOmeroImage(void)
{
  delete e;

}

gnomonOmeroType::type gnomonOmeroImage::type(void)
{
  return gnomonOmeroType::image;
}

QString gnomonOmeroImage::name(void)
{
    return QString();
}
