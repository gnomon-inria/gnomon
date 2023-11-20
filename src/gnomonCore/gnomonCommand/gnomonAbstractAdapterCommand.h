#pragma once

#include <gnomonCoreExport>

#include "gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonAbstractAdapterCommand : public gnomonAbstractAlgorithmCommand
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
