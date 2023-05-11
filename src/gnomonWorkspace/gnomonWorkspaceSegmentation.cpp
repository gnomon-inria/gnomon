#include "gnomonWorkspaceSegmentation.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFromImage>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>
#include <gnomonConfig.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceSegmentation
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    WORKSPACEINIT(Solver, cellImageFromImage, gnomonCellImageFromImageCommand)
        /*
    loadPluginGroup("cellImageFromImage");
    gnomonCore::cellImageFromImage::pluginManager().setVerboseLoading(true);
    gnomonCore::cellImageFromImage::pluginManager().initialize(GNOMON_PLUGIN_PATH);
    d->command   = new gnomonCellImageFromImageCommand;
    d->workspace = "Segmentation";

    d->keys = gnomonCore::cellImageFromImage::pluginFactory().keys();
    d->algorithmsData = gnomonCore::cellImageFromImage::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();
        */
    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();

}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation()
{
    auto *command = (gnomonCellImageFromImageCommand *)d->command;
    delete command;
}


//
// gnomonWorkspaceSegmentation.cpp ends here
