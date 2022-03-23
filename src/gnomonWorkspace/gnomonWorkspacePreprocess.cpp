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

#include "gnomonWorkspacePreprocess.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFilter.h>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFilterCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePreprocess
// ///////////////////////////////////////////////////////////////////

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("imageFilter");
    emit algorithmsLoaded();

    d->workspace = "Preprocess";
    d->command   = new gnomonImageFilterCommand;
    d->keys = gnomonCore::imageFilter::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();
    

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();

    connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    gnomonImageFilterCommand *command = (gnomonImageFilterCommand *)d->command;
    if (command) {
        delete command;
    }
}


//
// gnomonWorkspacePreprocess.cpp ends here
