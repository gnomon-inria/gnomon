
#include "gnomonOmeroImage.h"

class gnomonOmeroImagePrivate
{
public:

};

gnomonOmeroImage::gnomonOmeroImage(void)
{
    d = new gnomonOmeroImagePrivate;

}

gnomonOmeroImage::~gnomonOmeroImage(void)
{
  delete d;
}

gnomonOmeroType::type gnomonOmeroImage::type(void)
{
  return gnomonOmeroType::image;
}
