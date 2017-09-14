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

#pragma once

#include <tissueComposerExtensionExport.h>

#include <dtkComposer/dtkComposerExtension.h>

#include <QtCore>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class TISSUECOMPOSEREXTENSION_EXPORT tissueComposerExtensionPlugin : public dtkComposerExtensionPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkComposerExtensionPlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.tissueComposerExtensionPlugin" FILE "tissueComposerExtensionPlugin.json")

public:
     tissueComposerExtensionPlugin(void) {}
    ~tissueComposerExtensionPlugin(void) {}

public:
    void   initialize(void);
    void uninitialize(void);
};

//
// tissueComposerExtensionPlugin.h ends here
