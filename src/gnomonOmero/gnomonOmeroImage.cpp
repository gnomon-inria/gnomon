#include "gnomonOmeroImage.h"
#include "gnomonOmeroObject_p.h"

#include <omero/model/Length.h>
#include <omero/model/Pixels.h>
#include <omero/model/PixelsType.h>
#include "omero/model/TagAnnotationI.h"
#include <omero/model/ImageAnnotationLinkI.h>
#include <omero/all.h>
#include <omero/client.h>

class gnomonOmeroImagePrivate
{
public:
  omero::model::ImagePtr ref2omero;
  omero::model::PixelsTypePtr pixel_type;

public:
  int number_of_channels, number_of_timepoints;
  double pixel_size_x, pixel_size_y, pixel_size_z;
  double dim_x, dim_y, dim_z;
  QString description;
  omero::model::ExperimenterPtr owner;
};

gnomonOmeroImage::gnomonOmeroImage(void) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::image;
  e = new gnomonOmeroImagePrivate();
  e->description = "none" ;
  e->number_of_channels =0;
  e->number_of_timepoints =0;
  e->dim_x = 0;
  e->dim_y = 0;
  e->dim_z = 0 ;
  e->pixel_size_x = 1;
  e->pixel_size_y = 1;
  e->pixel_size_x = 1;

}

gnomonOmeroImage::gnomonOmeroImage(int long id) : gnomonOmeroObject()
{


}

gnomonOmeroImage::gnomonOmeroImage(omero::model::ImagePtr img) : gnomonOmeroObject()
{
  d->type = gnomonOmeroType::image;
  e = new gnomonOmeroImagePrivate();
  e->ref2omero = img ;

  if(e->ref2omero->getDescription()) {
    e->description = QString::fromStdString(e->ref2omero->getDescription()->getValue());
  }
  // omero::model::DetailsPtr details = e->ref2omero->getDetails();
  // e->owner = details->getOwner();

  if(e->ref2omero->getPrimaryPixels()) {

    e->pixel_type = e->ref2omero->getPrimaryPixels()->getPixelsType();

    e->number_of_channels = e->ref2omero->getPrimaryPixels()->getSizeC()->getValue();
    e->number_of_timepoints = e->ref2omero->getPrimaryPixels()->getSizeT()->getValue();

    e->dim_x = e->ref2omero->getPrimaryPixels()->getSizeX()->getValue();

    e->dim_y = e->ref2omero->getPrimaryPixels()->getSizeY()->getValue();
    e->dim_z = e->ref2omero->getPrimaryPixels()->getSizeZ()->getValue();

    if(e->ref2omero->getPrimaryPixels()->getPhysicalSizeX()) {

      e->pixel_size_x = e->ref2omero->getPrimaryPixels()->getPhysicalSizeX()->getValue();
      e->pixel_size_y = e->ref2omero->getPrimaryPixels()->getPhysicalSizeY()->getValue();
      e->pixel_size_z = e->ref2omero->getPrimaryPixels()->getPhysicalSizeZ()->getValue();
    }
  }
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

QStringList gnomonOmeroImage::details(void)
{
  QStringList list;
  QString item;

  item = "description       : " + e->description ;
  list << item;
  item = "nb channels : " + QString::number(e->number_of_channels);
  list << item;
  item = "nb timepoint: " + QString::number(e->number_of_timepoints);
  list << item;
  item = "pixel dim x : " + QString::number(e->dim_x) ;
  list << item;
  item = "pixel dim y : " + QString::number(e->dim_y) ;
  list << item;
  item = "pixel dim z : " + QString::number(e->dim_z) ;
  list << item;
  item = "pixel size x: " + QString::number(e->pixel_size_x) ;
  list << item;
  item = "pixel size y: " + QString::number(e->pixel_size_y) ;
  list << item;
  item = "pixel size z: " + QString::number(e->pixel_size_x) ;
  list << item;

  return list;
}

QString gnomonOmeroImage::name(void)
{
  if( e->ref2omero )
      return QString::fromStdString(e->ref2omero->getName()->getValue());
  else
      return QString();
}
