#include "gnomonAbstractImageFilter.h"

#include "gnomonCore.h"

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonAbstractImageFilter, imageFilter, gnomonCore);
}

//
// gnomonAbstractImageFilter.cpp ends here
