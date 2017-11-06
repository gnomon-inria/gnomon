// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonOmeroObject.h"

class gnomonOmeroObjectPrivate
{
public:
    int id;
    gnomonOmeroType::type type;
};

gnomonOmeroObject::gnomonOmeroObject(void)
{
    d       = new gnomonOmeroObjectPrivate;
    d->id   = -1;
    d->type = gnomonOmeroType::none;
}

gnomonOmeroObject::~gnomonOmeroObject(void)
{
  delete d;
}

int gnomonOmeroObject::id(void)
{
  return d->id;
}

QStringList gnomonOmeroObject::annotation(void)
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

}

//
// gnomonOmeroObject.cpp ends here
