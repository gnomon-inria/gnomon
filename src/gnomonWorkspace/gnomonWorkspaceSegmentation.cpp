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

    d->workspace = "Segmentation";
    d->command   = new gnomonCellImageFromImageCommand;
    d->algorithmsData = gnomonCore::cellImageFromImage::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

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
