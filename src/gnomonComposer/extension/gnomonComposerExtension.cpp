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

#include "gnomonComposerExtension.h"

#include <gnomonComposerNodeView.h>
#include <gnomonTissueGrowthModelNode.h>
#include <gnomonCellGraphFromImageNode.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonComposerExtension::gnomonComposerExtension(void) : dtkComposerExtension()
{

}

gnomonComposerExtension::~gnomonComposerExtension(void)
{

}

void gnomonComposerExtension::extend(dtkComposerNodeFactory *factory)
{
    if (!factory) {
        dtkError() << Q_FUNC_INFO << "No composer factory, can't extend it with dtkLinearAlgebraSparse nodes ";
        return;
    }

    factory->record(":dtkComposer/gnomonComposerNodeView.json", dtkComposerNodeCreator<gnomonComposerNodeView>);
    factory->record(":dtkComposer/gnomonTissueGrowthModelNode.json", dtkComposerNodeCreator<gnomonTissueGrowthModelNode>);
    factory->record(":dtkComposer/gnomonCellGraphFromImageNode.json", dtkComposerNodeCreator<gnomonCellGraphFromImageNode>);
}

//
// gnomonComposerExtension.cpp ends here
