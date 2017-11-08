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

#include <dtkComposer>

#include <gnomonComposerExport.h>

class gnomonAbstractCellComplexFromImage;

class GNOMONCOMPOSER_EXPORT gnomonCellComplexFromImageNode : public dtkComposerNodeObject<gnomonAbstractCellComplexFromImage>
{
public:
     gnomonCellComplexFromImageNode(void);
    ~gnomonCellComplexFromImageNode(void);

public:
    void run(void);

private:
    class gnomonCellComplexFromImageNodePrivate *d;
};


//
// gnomonCellComplexFromImageNode.h ends here