#pragma once

#include <QtCore>

#include <gnomonOmeroObject>
#include "gnomonOmeroImage.h"
#include "gnomonOmeroDataset.h"

#include <omero/model/Project.h>
#include <omero/model/Image.h>

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
  QString name(void);

public:
  QList<gnomonOmeroDatasetPtr> datasets(void);

private:
    class gnomonOmeroProjectPrivate *e;
};

inline bool gnomonOmeroProject::isProject(void) {return true;}
inline bool gnomonOmeroProject::isDataset(void) {return false;}
inline bool gnomonOmeroProject::isImage(void)   {return false;}

typedef QSharedPointer<gnomonOmeroProject> gnomonOmeroProjectPtr;
