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

#pragma once

#include <gnomonCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class gnomonAbstractCommand;

class gnomonAbstractConstructorCommand : public gnomonAbstractCommand
{

public:
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const {QMap<QString, gnomonCoreParameter *> empty; return empty;};;

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
};


//
// gnomonAbstractConstructorCommand.h ends here
