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

#include <gnomonCore>
#include <omero/client.h>
#include <omero/model/Project.h>
#include <omero/model/Dataset.h>
#include <omero/model/Image.h>
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/api/IAdmin.h>
#include <omero/api/IMetadata.h>


class gnomonOmero
{
public:
    gnomonOmero(void);
   ~gnomonOmero(void);

public:
  void readData(void);
  void writeData(void);
  void browseDB(void);

private:
    class gnomonOmeroPrivate *d;
};


//
// gnomonOmero.h ends here
