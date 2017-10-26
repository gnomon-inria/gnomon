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
    tissueOmeroType::type type(void);
    void setType(tissueOmeroType::type type);

private:
    class tissueOmeroObjectPrivate *d;
};


//
// tissueOmeroObject.h ends here
