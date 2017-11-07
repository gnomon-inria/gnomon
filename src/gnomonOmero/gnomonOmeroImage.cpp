
#include "gnomonOmeroImage.h"
#include "gnomonOmeroObject_p.h"

class gnomonOmeroImagePrivate
{
public:


};

gnomonOmeroImage::gnomonOmeroImage(void) : gnomonOmeroObject()
{
    d->type = gnomonOmeroType::image;

}

gnomonOmeroImage::~gnomonOmeroImage(void)
{

}

gnomonOmeroType::type gnomonOmeroImage::type(void)
{
  return gnomonOmeroType::image;
}
