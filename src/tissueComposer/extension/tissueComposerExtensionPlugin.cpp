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

#include "tissueComposerExtensionPlugin.h"
#include "tissueComposerExtension.h"

#include <tissueCore.h>

#include <dtkComposer>
#include <dtkLog>

// ///////////////////////////////////////////////////////////////////
// Helper functions
// ///////////////////////////////////////////////////////////////////

dtkComposerExtension *tissueComposerCreator(void)
{
    return new tissueComposerExtension();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////


void tissueComposerExtensionPlugin::initialize(void)
{
    dtkComposer::extension::pluginFactory().record("tissue", tissueComposerCreator);
    dtkComposerExtension *extension = dtkComposer::extension::pluginFactory().create("tissue");
    bool verbose = dtkComposer::extension::pluginManager().verboseLoading();
    tissueCore::setVerboseLoading(verbose);
    extension->extend(&(dtkComposer::node::factory()));
    tissueCore::initialize();
}

void tissueComposerExtensionPlugin::uninitialize(void)
{

}

// ///////////////////////////////////////////////////////////////////
// Plugin meta data
// ///////////////////////////////////////////////////////////////////

DTK_DEFINE_PLUGIN(dtkComposerExtension)

//
// tissueComposerExtensionPlugin.cpp ends here
