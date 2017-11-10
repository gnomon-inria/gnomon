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

#include "gnomonComposerExtensionPlugin.h"
#include "gnomonComposerExtension.h"

#include <gnomonCore.h>

#include <dtkComposer>
#include <dtkLog>

// ///////////////////////////////////////////////////////////////////
// Helper functions
// ///////////////////////////////////////////////////////////////////

dtkComposerExtension *gnomonComposerCreator(void)
{
    return new gnomonComposerExtension();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////


void gnomonComposerExtensionPlugin::initialize(void)
{
    dtkComposer::extension::pluginFactory().record("gnomon", gnomonComposerCreator);
    dtkComposerExtension *extension = dtkComposer::extension::pluginFactory().create("gnomon");
    bool verbose = dtkComposer::extension::pluginManager().verboseLoading();
    gnomonCore::setVerboseLoading(verbose);
    extension->extend(&(dtkComposer::node::factory()));
    if (dtkComposer::extension::isObjectManagerActive()) {
        gnomonCore::activateObjectManager();
    }
    gnomonCore::initialize();
}

void gnomonComposerExtensionPlugin::uninitialize(void)
{

}

// ///////////////////////////////////////////////////////////////////
// Plugin meta data
// ///////////////////////////////////////////////////////////////////

DTK_DEFINE_PLUGIN(dtkComposerExtension)

//
// gnomonComposerExtensionPlugin.cpp ends here
