#pragma once

#include <gnomonOmeroObject>
#include <omero/model/Image.h>

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
  gnomonOmeroType::type type(void);
  QString name(void);

private:
  class gnomonOmeroImagePrivate *e;

};

inline bool gnomonOmeroImage::isProject(void) {return false;}
inline bool gnomonOmeroImage::isDataset(void) {return false;}
inline bool gnomonOmeroImage::isImage(void)   {return true;}

typedef QSharedPointer<gnomonOmeroImage> gnomonOmeroImagePtr;
