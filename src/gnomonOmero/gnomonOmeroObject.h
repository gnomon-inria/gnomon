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
   virtual ~gnomonOmeroObject(void);

public:
    virtual int long id(void) = 0;
    virtual QStringList details(void)= 0;

public:
    virtual gnomonOmeroType::type type(void) = 0;
    virtual QString name(void) = 0;

public:
    virtual bool isProject(void) = 0;
    virtual bool isDataset(void) = 0;
    virtual bool isImage(void)   = 0;

protected:
    class gnomonOmeroObjectPrivate *d;
};


//
// gnomonOmeroObject.h ends here
