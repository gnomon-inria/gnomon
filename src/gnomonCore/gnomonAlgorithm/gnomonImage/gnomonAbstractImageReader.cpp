#include "gnomonAbstractImageReader.h"

#include "gnomonCore.h"

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DEFINE_CONCEPT(gnomonAbstractImageReader, imageReader, gnomonCore);
}

//
// gnomonAbstractImageReader.cpp ends here
