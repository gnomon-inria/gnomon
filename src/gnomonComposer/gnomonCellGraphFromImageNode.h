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

class gnomonAbstractCellGraphFromImage;

class GNOMONCOMPOSER_EXPORT gnomonCellGraphFromImageNode : public dtkComposerNodeObject<gnomonAbstractCellGraphFromImage>
{
public:
     gnomonCellGraphFromImageNode(void);
    ~gnomonCellGraphFromImageNode(void);

public:
    void run(void);

private:
    class gnomonCellGraphFromImageNodePrivate *d;
};


//
// gnomonCellGraphFromImageNode.h ends here