#pragma once

#include <gnomonOmeroObject>
#include <omero/model/Image.h>

#include <QtCore>
#include <dtkCore>
#include <dtkImaging>

class gnomonOmeroImage : public gnomonOmeroObject
{

public:
    gnomonOmeroImage(void);
    gnomonOmeroImage(omero::model::ImagePtr);
   ~gnomonOmeroImage(void);

public:
  bool isProject(void) ;
  bool isDataset(void) ;
  bool isImage(void)   ;

public:
  QString description(void);
  QString pixel_type(void);
  int number_of_channels(void);
  int number_of_timepoints(void);
  double dim_x(void);
  double dim_y(void);
  double dim_z(void);
  double pixel_size_x(void);
  double pixel_size_y(void);
  double pixel_size_z(void);


public:
  gnomonOmeroType::type type(void);
  int long id(void);
  QStringList details(void);
  QString name(void);

public:
  dtkImage *data(void);

private:
  class gnomonOmeroImagePrivate *e;

};

inline bool gnomonOmeroImage::isProject(void) {return false;}
inline bool gnomonOmeroImage::isDataset(void) {return false;}
inline bool gnomonOmeroImage::isImage(void)   {return true;}

typedef QSharedPointer<gnomonOmeroImage> gnomonOmeroImagePtr;
