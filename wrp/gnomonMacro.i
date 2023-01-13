#pragma once

// /////////////////////////////////////////////////////////////////
// Macro redefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMON_DECLARE_PLUGIN_FACTORY(type, Export)
%define GNOMON_DECLARE_PLUGIN_FACTORY(type, Export)
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
//%template(type##PluginFactorySwigTemplate) gnomonPluginFactory<type>;
class Export type##PluginFactory : public gnomonPluginFactory<type> {
public:                                                             \
    static type##PluginFactory& instance() {
        static type##PluginFactory _instance;
        return _instance;
    };
 };
%enddef

#undef  GNOMON_DECLARE_CONCEPT(type, Export, Namespace)
%define GNOMON_DECLARE_CONCEPT(type, Export, Namespace)
%rename(Namespace##_pluginFactory) Namespace::pluginFactory;
namespace Namespace {
     Export type##PluginFactory& pluginFactory();
}
%enddef
