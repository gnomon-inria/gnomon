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
        Export type##PluginFactory& pluginFactory();                             \
        Export type##PluginManager& pluginManager();                             \
        Export bool type##Registration(); \
        extern const bool is_registered; \
    } }                                                                          \
    class Export type##PluginFactory : public gnomonPluginFactory<type>          \
    {                                                                            \
    private:                                                                     \
         type##PluginFactory() {                                                 \
             qDebug() << "factory from c++ " << #type << this;               \
             pluginsFactories()[#Namespace] = this;                              \
             Layer::Namespace::pluginManager();                                  \
         };                                                                      \
         type##PluginFactory(type##PluginFactory const& other) = delete;         \
         type##PluginFactory(type##PluginFactory&& other) = delete;              \
         friend type##PluginFactory& Layer::Namespace::pluginFactory();          \
    };                                                                           \
    class Export type##PluginManager : public gnomonPluginManager<type##Plugin>  \
    {                                                                            \
    private:                                                                     \
         type##PluginManager() {                                                 \
             pluginsManagers()[#Namespace] = this;                               \
         };                                                                      \
         type##PluginManager(type##PluginManager const& other) = delete;         \
         type##PluginManager(type##PluginManager&& other) = delete;              \
         friend type##PluginManager& Layer::Namespace::pluginManager();          \
    };

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
        bool type##Registration() {                         \
            pluginManager();                                \
            pluginFactory();                                \
            return true;                                    \
        }                                                   \
        const bool is_registered = type##Registration();    \
    }
