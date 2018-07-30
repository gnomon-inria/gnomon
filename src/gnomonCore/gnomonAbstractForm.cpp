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

#include <dtkCore>

#include "gnomonCore.h"
#include "gnomonAbstractForm.h"


void gnomonAbstractForm::render(void) {
    qWarning() << "This class doesnt implement a rendering concept";
}

void gnomonAbstractForm::attachRenderer(vtkRenderer *renderer) {
    this->renderer = renderer;
}

// namespace gnomonCore {
//     DTK_DEFINE_CONCEPT(gnomonAbstractForm, form, gnomonCore);
// }

//
// gnomonAbstractForm.cpp ends here
