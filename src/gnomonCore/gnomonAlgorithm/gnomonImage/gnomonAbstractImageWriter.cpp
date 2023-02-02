#include "gnomonAbstractImageWriter.h"

#include "gnomonCore.h"

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DEFINE_CONCEPT(gnomonAbstractImageWriter, imageWriter, gnomonCore);
}

//
// gnomonAbstractImageWriter.cpp ends here
