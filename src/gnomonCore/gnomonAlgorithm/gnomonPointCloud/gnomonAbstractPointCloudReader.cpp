#include "gnomonAbstractPointCloudReader.h"

#include "gnomonCore.h"

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DEFINE_CONCEPT(gnomonAbstractPointCloudReader, pointCloudReader, gnomonCore);
}

//
// gnomonAbstractPointCloudReader.cpp ends here
