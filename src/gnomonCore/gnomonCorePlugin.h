#pragma once

//#include <dtkCorePlugin>
#include "gnomonPluginFactory.h"


// ///////////////////////////////////////////////////////////////////
// GNOMON_DECLARE_PLUGIN_FACTORY
// ///////////////////////////////////////////////////////////////////

#define GNOMON_DECLARE_PLUGIN_FACTORY(type, Export)                     \
    class Export type##PluginFactory : public gnomonPluginFactory<type> \
    {                                                                   \
    public:                                                             \
        static type##PluginFactory& instance()                          \
        {                                                               \
            static type##PluginFactory _instance;                       \
            return _instance;                                           \
        };                                                              \
    };

#define GNOMON_DECLARE_CONCEPT(type, Export, Namespace)     \
    namespace Namespace                                     \
    {                                                       \
        Export type##PluginFactory& pluginFactory();        \
    }

#define GNOMON_DEFINE_CONCEPT(type, Namespace, LayerName)   \
    namespace Namespace                                     \
    {                                                       \
        type##PluginFactory& pluginFactory()                \
        {                                                   \
            return type##PluginFactory::instance();         \
        }                                                   \
    }
