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

#include "tissueTissueGrowthModelNode.h"

#include <tissueAbstractTissueGrowthModel.h>
#include <tissueCellComplex.h>
#include <tissueCore.h>

#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// tissueTissueGrowthModelNodePrivate
// /////////////////////////////////////////////////////////////////

class tissueTissueGrowthModelNodePrivate
{
public:
    dtkComposerTransmitterReceiver<tissueCellComplex *> complex_in;

    dtkComposerTransmitterEmitter<tissueCellComplex *>  complex_out;
};

// /////////////////////////////////////////////////////////////////
// tissueTissueGrowthModelNode
// /////////////////////////////////////////////////////////////////

tissueTissueGrowthModelNode::tissueTissueGrowthModelNode(void) : dtkComposerNodeObject<tissueAbstractTissueGrowthModel>(), d(new tissueTissueGrowthModelNodePrivate())
{
    this->setFactory(tissueCore::growthModel::pluginFactory());

    this->appendReceiver(&d->complex_in);

    this->appendEmitter (&d->complex_out);
}

tissueTissueGrowthModelNode::~tissueTissueGrowthModelNode(void)
{
    delete d;
}

void tissueTissueGrowthModelNode::run(void)
{
    if (d->complex_in.isEmpty()) {
        dtkError() << Q_FUNC_INFO << "The input complex is not set. Aborting.";
        return;

    } else {

        tissueAbstractTissueGrowthModel *filter = this->object();
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
// tissueTissueGrowthModelNode.cpp ends here
