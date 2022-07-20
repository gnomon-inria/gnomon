#include "gnomonWorkspacePointCloudFromImage.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudFromImage.h>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudFromImageCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePointCloudFromImage
// ///////////////////////////////////////////////////////////////////

gnomonWorkspacePointCloudFromImage::gnomonWorkspacePointCloudFromImage(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("pointCloudFromImage");
    emit algorithmsLoaded();

    d->workspace = "Point Detection";
    d->command   = new gnomonPointCloudFromImageCommand;
    d->keys = gnomonCore::pointCloudFromImage::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();


    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();
}

gnomonWorkspacePointCloudFromImage::~gnomonWorkspacePointCloudFromImage(void)
{
    gnomonPointCloudFromImageCommand *command = (gnomonPointCloudFromImageCommand *)d->command;
    if (command) {
        delete command;
    }
}


//
// gnomonWorkspacePointCloudFromImage.cpp ends here
