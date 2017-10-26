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

class tissueAbstractTissueGrowthModel;

// ///////////////////////////////////////////////////////////////////
// tissueTissueGrowthModelNode
// ///////////////////////////////////////////////////////////////////

class TISSUECOMPOSER_EXPORT tissueTissueGrowthModelNode : public dtkComposerNodeObject<tissueAbstractTissueGrowthModel>
{
public:
     tissueTissueGrowthModelNode(void);
    ~tissueTissueGrowthModelNode(void);

public:
    void run(void);

private:
    class tissueTissueGrowthModelNodePrivate *d;
};

//
// tissueTissueGrowthModelNode.h ends here
