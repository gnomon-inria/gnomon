
#include "gnomonOmeroImage.h"
#include "gnomonOmeroObject_p.h"

gnomonOmeroImage::gnomonOmeroImage(void) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::image;
}

gnomonOmeroType::type gnomonOmeroImage::type(void)
{
  return gnomonOmeroType::image;
}

