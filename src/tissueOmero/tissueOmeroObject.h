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

namespace tissueOmeroType{
  enum type {none, project, dataset, image};
};

class tissueOmeroObject
{
public:
    tissueOmeroObject(void);
   ~tissueOmeroObject(void);

public:
    void setId(int);
    int id(void);

public:
    tissueOmeroType::type type(void);
    void setType(tissueOmeroType::type type);

    bool isProject(void);
    bool isDataset(void);
    bool isImage(void);

private:
    class tissueOmeroObjectPrivate *d;
};


//
// tissueOmeroObject.h ends here
