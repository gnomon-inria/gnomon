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
#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonAbstractConstructorCommand : public gnomonAbstractCommand
{
public:
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override {
        return {};
    }

    orderedMap inputTypes() override {
        return {};
    }
};


//
// gnomonAbstractConstructorCommand.h ends here
