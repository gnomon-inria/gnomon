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
#include "gnomonAbstractModel.h"

// namespace gnomonCore {
//     DTK_DEFINE_CONCEPT(gnomonAbstractModel, form, gnomonCore);
// }

void gnomonAbstractModel::setForm(gnomonAbstractDynamicForm *dynmic_form)
{
    this->dynamic_form = dynamic_form;
}

//
// gnomonAbstractModel.cpp ends here
