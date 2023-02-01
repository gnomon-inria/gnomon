#include "gnomonWorkspacePointDetection.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudFromImage.h>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudFromImageCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePointCloudFromImage
// ///////////////////////////////////////////////////////////////////

gnomonWorkspacePointDetection::gnomonWorkspacePointDetection(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("pointCloudFromImage");

    d->workspace = "Point Detection";
    d->command   = new gnomonPointCloudFromImageCommand;
    d->keys = gnomonCore::pointCloudFromImage::pluginFactory().keys();
    d->algorithmsData = gnomonCore::pointCloudFromImage::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();


    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();
}

gnomonWorkspacePointDetection::~gnomonWorkspacePointDetection(void)
{
    gnomonPointCloudFromImageCommand *command = (gnomonPointCloudFromImageCommand *)d->command;
    if (command) {
        delete command;
    }
}


//
// gnomonWorkspacePointDetection.cpp ends here
