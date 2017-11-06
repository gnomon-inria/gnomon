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

namespace gnomonOmeroType{
  enum type {none, project, dataset, image};
};

class gnomonOmeroObject
{
public:
    gnomonOmeroObject(void);
   ~gnomonOmeroObject(void);

public:
    int id(void);
    QStringList annotation(void);

public:
    virtual gnomonOmeroType::type type(void) = 0;

    virtual bool isProject(void) = 0;
    virtual bool isDataset(void) = 0;
    virtual bool isImage(void)   = 0;

private:
    class gnomonOmeroObjectPrivate *d;
};


//
// gnomonOmeroObject.h ends here
