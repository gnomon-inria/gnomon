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
