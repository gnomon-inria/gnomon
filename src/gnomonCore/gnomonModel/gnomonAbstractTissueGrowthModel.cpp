#include "gnomonCore.h"

#include "gnomonAbstractTissueGrowthModel.h"

//  ///////////////////////////////////////////////////////////////////
//  Register gnomonAbstractTissueGrowthModel to the layer
//  ///////////////////////////////////////////////////////////////////

namespace gnomonCore {
  GNOMON_DEFINE_CONCEPT(gnomonAbstractTissueGrowthModel, growthmodel, gnomonCore);
}

//
// gnomonAbstractTissueGrowthModel.cpp ends here
