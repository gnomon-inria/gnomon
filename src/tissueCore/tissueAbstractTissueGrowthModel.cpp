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

#include "tissueCore.h"

#include "tissueAbstractTissueGrowthModel.h"

//  ///////////////////////////////////////////////////////////////////
//  Register tissueAbstractTissueGrowthModel to the layer
//  ///////////////////////////////////////////////////////////////////

namespace tissueCore {
  DTK_DEFINE_CONCEPT(tissueAbstractTissueGrowthModel, growthmodel, tissueCore);
}

//
// tissueAbstractTissueGrowthModel.cpp ends here
