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

#include "gnomonAbstractForm.h"

#include "gnomonCore.h"

void gnomonAbstractForm::render(void) {
    qWarning() << "This class doesnt implement a rendering concept";
}

void gnomonAbstractForm::attachRenderer(vtkRenderer *renderer) {
    this->renderer = renderer;
}

void gnomonAbstractForm::record(void)
{
    static int counter = 0;
    dtkObjectManager::instance()->add(dtkMetaType::variantFromValue(this),
                                      QString("%1_%2").arg(this->name()).arg(counter++));
}

namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonAbstractForm, abstractForm, gnomonCore);
}

//
// gnomonAbstractForm.cpp ends here
