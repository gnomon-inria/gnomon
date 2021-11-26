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

#include "gnomonWorkspaceRegistration.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceRegistration
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("imageRegistration");
    emit algorithmsLoaded();

    d->workspace = "Time Registration";
    d->command = new gnomonImageRegistrationCommand;
    d->keys = gnomonCore::imageRegistration::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();
    emit parametersChanged();

    //create the views
    this->sources()->addView();
    this->sources()->addView();
    this->targets()->addView();

    d->updateViewFormTypes();
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    gnomonImageRegistrationCommand *command = (gnomonImageRegistrationCommand *)d->command;
    if (command) {
        delete command;
    }
}


//
// gnomonWorkspaceRegistration.cpp ends here
