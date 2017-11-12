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

class gnomonAbstractCellImageFromImage;

class GNOMONCOMPOSER_EXPORT gnomonCellImageFromImageNode : public dtkComposerNodeObject<gnomonAbstractCellImageFromImage>
{
public:
     gnomonCellImageFromImageNode(void);
    ~gnomonCellImageFromImageNode(void);

public:
    void run(void);

private:
    class gnomonCellImageFromImageNodePrivate *d;
};


//
// gnomonCellImageFromImageNode.h ends here