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

#include <tissueComposerExport.h>

// ///////////////////////////////////////////////////////////////////
// tissueTissueGrowthModelNode
// ///////////////////////////////////////////////////////////////////

class TISSUECOMPOSER_EXPORT tissueComposerNodeView : public dtkComposerNodeLeaf
{
public:
     tissueComposerNodeView(void);
    ~tissueComposerNodeView(void);

public:
    void run(void);

private:
    class tissueComposerNodeViewPrivate *d;
};

//
// tissueComposerNodeView.h ends here
