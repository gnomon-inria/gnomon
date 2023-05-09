#pragma once

//#include <dtkCorePlugin>
#include "gnomonPluginFactory.h"
#include "gnomonPluginManager.h"
#include "gnomonPythonPluginLoader.h"

// ///////////////////////////////////////////////////////////////////
// GNOMON_DECLARE_PLUGIN_FACTORY
// ///////////////////////////////////////////////////////////////////

#define GNOMON_DECLARE_PLUGIN_FACTORY(type, Export)                              \
    class Export type##PluginFactory : public gnomonPluginFactory<type>          \
    {                                                                            \
    public:                                                                      \
        static type##PluginFactory& instance()                                   \
        {                                                                        \
            static type##PluginFactory _instance;                                \
            return _instance;                                                    \
        };                                                                       \
    };                                                                           \
    class Export type##PluginManager : public gnomonPluginManager<type##Plugin> \
    {                                                                            \
    public:                                                                      \
        static type##PluginManager& instance()                                   \
        {                                                                        \
            static type##PluginManager _instance;                                \
            return _instance;                                                    \
        };                                                                       \
    };

#define GNOMON_DECLARE_CONCEPT(type, Export, Namespace)            \
    namespace Namespace                                            \
    {                                                              \
        Export type##PluginFactory& pluginFactory();               \
        Export type##PluginManager& pluginManager();               \
        struct type##ManagerRegister {                             \
            type##ManagerRegister() {                              \
                pluginsManagers()[#Namespace] = &pluginManager();  \
            } };                                                   \
    }

#define GNOMON_DEFINE_CONCEPT(type, Namespace, LayerName)   \
    namespace Namespace                                     \
    {                                                       \
        type##PluginFactory& pluginFactory()                \
        {                                                   \
            return type##PluginFactory::instance();         \
        }                                                   \
        type##PluginManager& pluginManager()                \
        {                                                   \
            return type##PluginManager::instance();         \
        }                                                   \
        static type##ManagerRegister _type##register;       \
    }
