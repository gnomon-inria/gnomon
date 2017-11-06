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

#pragma once

#include <QtCore>
#include <gnomonCore>
#include <omero/client.h>
#include <omero/model/Project.h>
#include <omero/model/Dataset.h>
#include <omero/model/Image.h>
#include <omero/model/Pixels.h>
#include <omero/model/PixelsType.h>
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/api/IAdmin.h>
#include <omero/api/IMetadata.h>

#include "gnomonOmeroProject.h"
#include "gnomonOmeroDataset.h"
#include "gnomonOmeroImage.h"


class gnomonOmero
{
public:
    gnomonOmero(void);
   ~gnomonOmero(void);

public:
  void browseDB(void);

public:
  QList<gnomonOmeroProject*> projects(void);
  QList<gnomonOmeroDataset*> datasets(void);
  QList<gnomonOmeroImage*> images(void);

protected:
    class gnomonOmeroPrivate *d;
};


//
// gnomonOmero.h ends here
