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

namespace gnomonOmeroType{
  enum type {none, project, dataset, image};
};

class gnomonOmeroObject
{
public:
    gnomonOmeroObject(void);
   ~gnomonOmeroObject(void);

public:
    void setId(int);
    int id(void);

public:
    gnomonOmeroType::type type(void);
    void setType(gnomonOmeroType::type type);

    bool isProject(void);
    bool isDataset(void);
    bool isImage(void);

private:
    class gnomonOmeroObjectPrivate *d;
};


//
// gnomonOmeroObject.h ends here
