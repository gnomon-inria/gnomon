#pragma once

// /////////////////////////////////////////////////////////////////
// Macro redefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMON_DECLARE_PLUGIN_FACTORY(type, Export, Namespace, Layer)
%define GNOMON_DECLARE_PLUGIN_FACTORY(type, Export, Namespace, Layer)
%extend QVariant {
        void setValue(type *value) {
            $self->setValue(dtk::variantFromValue(value));
        }
        type* to##type() const {
            return $self->value<type *>();
        }
}

%include <gnomonCore/gnomonPluginFactory.h>

%template(type##PluginFactorySwigTemplate) gnomonPluginFactory<type>;

class type##PluginFactory;

namespace Layer {
    %rename(Namespace##_pluginFactory) Namespace::pluginFactory;
    namespace Namespace
    {
        Export type##PluginFactory& pluginFactory();
    }
}

%newobject type##PluginFactory::create;

%feature(nodirector) type##PluginFactory;
class Export type##PluginFactory : public gnomonPluginFactory<type>
{
    // public:
    //type##PluginFactory& instance();
 private:
    type##PluginFactory() {
        qDebug() << "WARNING create factory from swig" << #type << this;
        qDebug() << "This shouldn't happen! the constructor is private call the one from gnomonCorePlugin.h";
    };
    type##PluginFactory(type##PluginFactory const& other) = delete;
    type##PluginFactory(type##PluginFactory&& other) = delete;
    friend type##PluginFactory& Layer::Namespace::pluginFactory();
};


%enddef
