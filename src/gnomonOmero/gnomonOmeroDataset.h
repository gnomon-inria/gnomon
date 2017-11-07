
#pragma once

#include <QtCore>
#include <gnomonOmeroObject>
#include "gnomonOmeroImage.h"

#include <omero/model/Dataset.h>

class gnomonOmeroDataset : public gnomonOmeroObject
{

public:
    gnomonOmeroDataset(void);
    gnomonOmeroDataset(omero::model::DatasetPtr);
   ~gnomonOmeroDataset(void);

public:
   bool isProject(void) ;
   bool isDataset(void) ;
   bool isImage(void)   ;
   gnomonOmeroType::type type(void);

public:
    QList<gnomonOmeroImagePtr> images(void);

private:
    class gnomonOmeroDatasetPrivate *e;

};

inline bool gnomonOmeroDataset::isProject(void) {return false;}
inline bool gnomonOmeroDataset::isDataset(void) {return true;}
inline bool gnomonOmeroDataset::isImage(void)   {return false;}

typedef QSharedPointer<gnomonOmeroDataset> gnomonOmeroDatasetPtr;
