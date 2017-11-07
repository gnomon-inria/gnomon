
#pragma once

#include <QtCore>
#include <gnomonOmeroObject>
#include "gnomonOmeroImage.h"

class gnomonOmeroDataset : public gnomonOmeroObject
{

public:
    gnomonOmeroDataset(void);
   ~gnomonOmeroDataset(void);

public:
   bool isProject(void) ;
   bool isDataset(void) ;
   bool isImage(void)   ;
   gnomonOmeroType::type type(void);

public:
    QList<gnomonOmeroImage*> images(void);

};

inline bool gnomonOmeroDataset::isProject(void) {return false;}
inline bool gnomonOmeroDataset::isDataset(void) {return true;}
inline bool gnomonOmeroDataset::isImage(void)   {return false;}
