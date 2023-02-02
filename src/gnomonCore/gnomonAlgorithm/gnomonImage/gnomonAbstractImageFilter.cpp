#include "gnomonAbstractImageFilter.h"

#include "gnomonCore.h"

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DEFINE_CONCEPT(gnomonAbstractImageFilter, imageFilter, gnomonCore);
}

//
// gnomonAbstractImageFilter.cpp ends here
