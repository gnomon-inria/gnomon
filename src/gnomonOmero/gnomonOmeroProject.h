
#pragma once

#include <QtCore>
#include <gnomonOmeroObject>
#include "gnomonOmeroDataset.h"
#include "gnomonOmeroImage.h"

#include <omero/model/Project.h>

class gnomonOmeroProject : public gnomonOmeroObject
{
public:
    gnomonOmeroProject(void);
    gnomonOmeroProject(omero::model::ProjectPtr);
   ~gnomonOmeroProject(void);


public:
  bool isProject(void) ;
  bool isDataset(void) ;
  bool isImage(void)   ;
  gnomonOmeroType::type type(void);

public:
  QList<gnomonOmeroDataset*> datasets(void);
  QList<gnomonOmeroImage*> images(void);

private:
    class gnomonOmeroProjectPrivate *e;
};

inline bool gnomonOmeroProject::isProject(void) {return true;}
inline bool gnomonOmeroProject::isDataset(void) {return false;}
inline bool gnomonOmeroProject::isImage(void)   {return false;}
