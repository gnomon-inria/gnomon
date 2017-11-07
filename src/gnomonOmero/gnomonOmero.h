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
#include "gnomonOmeroImage.h"
#include "gnomonOmeroDataset.h"
#include "gnomonOmeroProject.h"


class gnomonOmero
{
public:
    gnomonOmero(void);
   ~gnomonOmero(void);

public:
  void browseDB(void);
  int long userId(void);

public:
  QList<gnomonOmeroProjectPtr> projects(void);
  QList<gnomonOmeroDatasetPtr> datasets(void);
  QList<gnomonOmeroImagePtr> images(void);

protected:
    class gnomonOmeroPrivate *d;
};


//
// gnomonOmero.h ends here
