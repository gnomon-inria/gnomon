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

#include <gnomonComposerExtensionExport.h>

#include <dtkComposer/dtkComposerExtension.h>

#include <QtCore>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class GNOMONCOMPOSEREXTENSION_EXPORT gnomonComposerExtensionPlugin : public dtkComposerExtensionPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkComposerExtensionPlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonComposerExtensionPlugin" FILE "gnomonComposerExtensionPlugin.json")

public:
     gnomonComposerExtensionPlugin(void) {}
    ~gnomonComposerExtensionPlugin(void) {}

public:
    void   initialize(void);
    void uninitialize(void);
};

//
// gnomonComposerExtensionPlugin.h ends here
