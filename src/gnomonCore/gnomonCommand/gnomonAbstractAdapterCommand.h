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

class gnomonAbstractAdapterCommand : public gnomonAbstractCommand
{
public:
    orderedMap outputTypes() override {
        orderedMap types;
        types.emplace_back(std::make_pair("output", "gnomonAbstractDynamicForm"));
        return types;
    }
};


//
// gnomonAbstractAdapterCommand.h ends here
