#include "gnomonWorkspaceSegmentation.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFromImage>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceSegmentation
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("cellImageFromImage");
    emit algorithmsLoaded();

    d->workspace = "Segmentation";
    d->command   = new gnomonCellImageFromImageCommand;
    d->keys = gnomonCore::cellImageFromImage::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();

    connect(d->command, &gnomonAbstractCommand::finished, [this]() { 
        this->viewOutputs();
        this->finished();
    });

}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation()
{
    auto *command = (gnomonCellImageFromImageCommand *)d->command;
    delete command;
}


//
// gnomonWorkspaceSegmentation.cpp ends here
