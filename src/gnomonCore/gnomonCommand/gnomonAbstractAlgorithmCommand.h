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

class gnomonAbstractAlgorithmCommand : public gnomonAbstractCommand
{

public:
    virtual dtkCoreParameters parameters(void) const {dtkCoreParameters empty; return empty;};;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
};


//
// gnomonAbstractAlgorithmCommand.h ends here
