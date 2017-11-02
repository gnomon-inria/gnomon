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

#include "gnomonTissueGrowthModelNode.h"

#include <gnomonAbstractTissueGrowthModel.h>
#include <gnomonCellComplex.h>
#include <gnomonCore.h>

#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// gnomonTissueGrowthModelNodePrivate
// /////////////////////////////////////////////////////////////////

class gnomonTissueGrowthModelNodePrivate
{
public:
    dtkComposerTransmitterReceiver<gnomonCellComplex *> complex_in;

    dtkComposerTransmitterEmitter<gnomonCellComplex *>  complex_out;
};

// /////////////////////////////////////////////////////////////////
// gnomonTissueGrowthModelNode
// /////////////////////////////////////////////////////////////////

gnomonTissueGrowthModelNode::gnomonTissueGrowthModelNode(void) : dtkComposerNodeObject<gnomonAbstractTissueGrowthModel>(), d(new gnomonTissueGrowthModelNodePrivate())
{
    this->setFactory(gnomonCore::growthmodel::pluginFactory());

    this->appendReceiver(&d->complex_in);

    this->appendEmitter (&d->complex_out);
}

gnomonTissueGrowthModelNode::~gnomonTissueGrowthModelNode(void)
{
    delete d;
}

void gnomonTissueGrowthModelNode::run(void)
{
    if (d->complex_in.isEmpty()) {
        dtkError() << Q_FUNC_INFO << "The input complex is not set. Aborting.";
        return;

    } else {

        gnomonAbstractTissueGrowthModel *filter = this->object();
        if (!filter) {
            dtkError() << Q_FUNC_INFO << "No Watershed filter found. Aborting.";
            return;
        }
        filter->setCellComplex(d->complex_in.data());

        filter->run();

        d->complex_out.setData(filter->grownCellComplex());
    }
}

//
// gnomonTissueGrowthModelNode.cpp ends here
