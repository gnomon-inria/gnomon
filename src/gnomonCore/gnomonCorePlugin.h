#pragma once

//#include <dtkCorePlugin>
#include "gnomonPluginFactory.h"
#include "gnomonPluginManager.h"
#include "gnomonPythonPluginLoader.h"

// ///////////////////////////////////////////////////////////////////
// GNOMON_DECLARE_PLUGIN_FACTORY
// ///////////////////////////////////////////////////////////////////

#define GNOMON_DECLARE_PLUGIN_FACTORY(type, Export, Namespace, Layer)            \
    class type##PluginFactory;                                                   \
    class type##PluginManager;                                                   \
    namespace Layer { namespace Namespace {                                      \
        type##PluginFactory& pluginFactory();                                    \
        type##PluginManager& pluginManager();                                    \
    } }                                                                          \
    class Export type##PluginFactory : public gnomonPluginFactory<type>          \
    {                                                                            \
    public:                                                                      \
         type##PluginFactory& instance();                                        \
    private:                                                                     \
         type##PluginFactory() { };                                              \
         type##PluginFactory(type##PluginFactory const& other) = delete;         \
         type##PluginFactory(type##PluginFactory&& other) = delete;              \
         friend type##PluginFactory& Layer::Namespace::pluginFactory();     \
    };                                                                           \
    class Export type##PluginManager : public gnomonPluginManager<type##Plugin> \
    {                                                                            \
    public:                                                                      \
        type##PluginManager& instance();                                         \
    private:                                                                     \
         type##PluginManager() { };                                              \
         type##PluginManager(type##PluginManager const& other) = delete;         \
         type##PluginManager(type##PluginManager&& other) = delete;              \
         friend type##PluginManager& Layer::Namespace::pluginManager();     \
    };                                                                           \
    namespace Layer {                                                            \
        namespace Namespace                                                      \
        {                                                                        \
            Export type##PluginFactory& pluginFactory();                         \
            Export type##PluginManager& pluginManager();                         \
            struct type##ManagerRegister {                                       \
                type##ManagerRegister() {                                        \
                    pluginsManagers()[#Namespace] = &pluginManager();            \
                    pluginsFactories()[#Namespace] = &pluginFactory();           \
                } };                                                             \
        }                                                                        \
    }

#define GNOMON_DEFINE_CONCEPT(type, Namespace, LayerName)   \
    namespace Namespace                                     \
    {                                                       \
        type##PluginFactory& pluginFactory()                \
        {                                                   \
            static type##PluginFactory _instance;           \
            return _instance;                               \
        }                                                   \
        type##PluginManager& pluginManager()                \
        {                                                   \
            static type##PluginManager _instance;           \
            return _instance;                               \
        }                                                   \
        static type##ManagerRegister _type##register;       \
    }
