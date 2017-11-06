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

class gnomonAbstractTissueGrowthModel;

// ///////////////////////////////////////////////////////////////////
// gnomonTissueGrowthModelNode
// ///////////////////////////////////////////////////////////////////

class GNOMONCOMPOSER_EXPORT gnomonTissueGrowthModelNode : public dtkComposerNodeObject<gnomonAbstractTissueGrowthModel>
{
public:
     gnomonTissueGrowthModelNode(void);
    ~gnomonTissueGrowthModelNode(void);

public:
    void run(void);

private:
    class gnomonTissueGrowthModelNodePrivate *d;
};

//
// gnomonTissueGrowthModelNode.h ends here
