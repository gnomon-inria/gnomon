
#pragma once

#include <QtCore>
#include <gnomonOmeroObject>

class gnomonOmeroImage : public gnomonOmeroObject
{
public:
    gnomonOmeroImage(void);
   ~gnomonOmeroImage(void);

public:
  bool isProject(void) ;
  bool isDataset(void) ;
  bool isImage(void)   ;
  gnomonOmeroType::type type(void);

private:
    class gnomonOmeroImagePrivate *d;

};


inline bool gnomonOmeroImage::isProject(void) {return false;}
inline bool gnomonOmeroImage::isDataset(void) {return false;}
inline bool gnomonOmeroImage::isImage(void)   {return true;}
