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

int long gnomonOmeroImage::id(void)
{
  int long imgId = e->ref2omero->getId()->getValue();
  qWarning() << "ImageID: " << imgId ;
  return imgId;
}

QStringList gnomonOmeroImage::annotation(void)
{
  // omero::sys::LongList imglist;
  // imglist.push_back(imgId);
  // omero::model::ImagePtr img = d->containerService->getImages("Image",imglist,0)[0];
  //
  // omero::model::PixelsPtr pixels = img->getPrimaryPixels();
  // long int pixelID = pixels->getId()->getValue();
  // omero::model::PixelsPtr pixelsObject = d->pixelsService->retrievePixDescription(pixelID);
  //
  // long pixelId = pixelsObject->getId()->getValue();
  // d->rawPixelsStore->setPixelsId(pixelId, false);
  //
  // d->xdim = pixelsObject->getSizeX()->getValue();
  // d->ydim = pixelsObject->getSizeY()->getValue();
  // d->zdim = pixelsObject->getSizeZ()->getValue();
  //
  // d->cdim = pixelsObject->getSizeC()->getValue();
  // d->tdim = pixelsObject->getSizeT()->getValue();
  //
  // d->xphysize = pixelsObject->getPhysicalSizeX()->getValue();
  // d->yphysize = pixelsObject->getPhysicalSizeY()->getValue();
  // d->zphysize = pixelsObject->getPhysicalSizeZ()->getValue();
  //
  // d->pixtype = pixelsObject->getPixelsType()->getValue()->getValue();
  //
  // qWarning()<< " OMERO Image id " << imgId;
  // // qWarning()<< " Dimensions(x,y,z,c,t) = "<<xdim<<","<<ydim<<","<<zdim<<","<<cdim<<","<<tdim ;
  // // qWarning()<< " PhyicalSize(x,y,z) "<<xphysize<<","<<yphysize<<","<<zphysize;
  // qWarning()<< " PixelType: "<< d->pixtype;
  return QStringList();
}

QString gnomonOmeroImage::name(void)
{
    return QString();
}
