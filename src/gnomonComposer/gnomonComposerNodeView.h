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

// ///////////////////////////////////////////////////////////////////
// gnomonTissueGrowthModelNode
// ///////////////////////////////////////////////////////////////////

class GNOMONCOMPOSER_EXPORT gnomonComposerNodeView : public dtkComposerNodeLeaf
{
public:
     gnomonComposerNodeView(void);
    ~gnomonComposerNodeView(void);

public:
    void run(void);

private:
    class gnomonComposerNodeViewPrivate *d;
};

//
// gnomonComposerNodeView.h ends here
