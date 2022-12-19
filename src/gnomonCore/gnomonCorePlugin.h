#pragma once

#include <dtkCorePlugin>
#include "gnomonPluginFactory.h"


// ///////////////////////////////////////////////////////////////////
// GNOMON_DECLARE_PLUGIN_FACTORY
// ///////////////////////////////////////////////////////////////////

#define GNOMON_DECLARE_PLUGIN_FACTORY(type, Export)                         \
    class Export type##PluginFactory : public gnomonPluginFactory<type> \
    {                                                                    \
    };

