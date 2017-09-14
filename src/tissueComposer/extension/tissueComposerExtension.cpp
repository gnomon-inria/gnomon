// Version: $Id: 1e79f4fa799a123ea455879ae03c89d8ac844a2f $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "tissueComposerExtension.h"

//#include <tissueComposerNodeGrowthModel.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

tissueComposerExtension::tissueComposerExtension(void) : dtkComposerExtension()
{

}

tissueComposerExtension::~tissueComposerExtension(void)
{

}

void tissueComposerExtension::extend(dtkComposerNodeFactory *factory)
{
    if (!factory) {
        dtkError() << Q_FUNC_INFO << "No composer factory, can't extend it with dtkLinearAlgebraSparse nodes ";
        return;
    }
    //factory->record(":dtkComposer/tissueComposerNodeGrowthModel.json", dtkComposerNodeCreator< dtkComposerNodeGrowthModel >);
}

//
// tissueComposerExtension.cpp ends here
