#include "gnomonOmeroImage.h"
#include "gnomonOmeroObject_p.h"

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
  double pixel_size_x, pixel_size_y, pixel_size_z, dim_x, dim_y, dim_z;
  QString description;
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

QStringList gnomonOmeroImage::details(void)
{

  QStringList list;

  e->description = QString::fromStdString(e->ref2omero->getDescription()->getValue());

  e->pixel_type = e->ref2omero->getPrimaryPixels()->getPixelsType();

  e->number_of_channels = e->ref2omero->getPrimaryPixels()->getSizeC()->getValue();
  qWarning() << "nb channels: " << e->number_of_channels ;
  e->number_of_timepoints = e->ref2omero->getPrimaryPixels()->getSizeT()->getValue();
  qWarning() << "nb timepoint: " << e->number_of_channels ;

  e->dim_x = e->ref2omero->getPrimaryPixels()->getSizeX()->getValue();
  qWarning() << "pixel dim x: " << e->dim_x ;
  e->dim_y = e->ref2omero->getPrimaryPixels()->getSizeY()->getValue();
  qWarning() << "pixel dim y: " << e->dim_y ;
  e->dim_z = e->ref2omero->getPrimaryPixels()->getSizeZ()->getValue();
  qWarning() << "pixel dim z: " << e->dim_z ;

  //e->pixel_size_x = e->ref2omero->getPrimaryPixels()->getPhysicalSizeX()->getValue();
  //e->pixel_size_y = e->ref2omero->getPrimaryPixels()->getPhysicalSizeY()->getValue();
  //e->pixel_size_z = e->ref2omero->getPrimaryPixels()->getPhysicalSizeZ()->getValue();

  // list.push_back(QString::fromStdString(e->pixtype));
  return QStringList();
}

QString gnomonOmeroImage::name(void)
{
  if( e->ref2omero )
      return QString::fromStdString(e->ref2omero->getName()->getValue());
  else
      return QString();
}
