//
// Created by Arthur Luciani on 09/12/2021.
//


#include "gnomonCore.h"
#include "gnomonAbstractBinaryImageData.h"
// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonAbstractBinaryImageData, binaryImageData, gnomonCore);
}