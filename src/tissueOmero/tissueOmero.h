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

#include <tissueCore>

class tissueOmero
{
public:
    tissueOmero(void);
   ~tissueOmero(void);

public:
  void readData(void);
  void writeData(void);
  void browseDB(void);

private:
    class tissueOmeroPrivate *d;
};


//
// tissueOmero.h ends here
