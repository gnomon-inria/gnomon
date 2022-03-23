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

#include "gnomonWorkspaceCellImageTracking.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageTrackingCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

gnomonWorkspaceCellImageTracking::gnomonWorkspaceCellImageTracking(QObject *parent): gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("cellImageTracking");
    emit algorithmsLoaded();

    d->workspace = "Cellular Tracking";
    d->command   = new gnomonCellImageTrackingCommand;
    d->keys = gnomonCore::cellImageTracking::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();

    connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));
}

gnomonWorkspaceCellImageTracking::~gnomonWorkspaceCellImageTracking(void)
{

}
