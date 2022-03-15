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

#include <gnomonCoreExport>

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
#include "gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonAbstractAdapterCommand : public gnomonAbstractCommand
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
