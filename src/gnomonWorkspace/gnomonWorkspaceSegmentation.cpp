#include "gnomonWorkspaceSegmentation.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand.h>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>
#include <vtkRenderer.h>

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

    connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation()
{
    auto *command = (gnomonCellImageFromImageCommand *)d->command;
    delete command;
}


//
// gnomonWorkspaceSegmentation.cpp ends here
