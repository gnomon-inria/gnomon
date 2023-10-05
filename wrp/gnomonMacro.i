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

%feature(nodirector) type##PluginFactory;
class Export type##PluginFactory : public gnomonPluginFactory<type>
{
    // public:
    //type##PluginFactory& instance();
 private:
    type##PluginFactory() { };
    type##PluginFactory(type##PluginFactory const& other) = delete;
    type##PluginFactory(type##PluginFactory&& other) = delete;
    friend type##PluginFactory& Layer::Namespace::pluginFactory();
};


%enddef
