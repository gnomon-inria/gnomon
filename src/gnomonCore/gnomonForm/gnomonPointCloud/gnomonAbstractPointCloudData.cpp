#include "gnomonCore.h"
#include "gnomonAbstractPointCloudData.h"

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DEFINE_CONCEPT(gnomonAbstractPointCloudData, pointCloudData, gnomonCore);
}

//
// gnomonAbstractPointCloudData.cpp ends here
