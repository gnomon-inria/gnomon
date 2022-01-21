#include "gnomonWorkspaceBinarization.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonBinaryImage/gnomonBinaryImageFromImageCommand.h>
#include <gnomonVisualization>

#include <vtkImageData.h>
#include <vtkRenderer.h>


gnomonWorkspaceBinarization::gnomonWorkspaceBinarization(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("binaryImageFromImage");
    emit algorithmsLoaded();

    d->workspace = "Binarization";
    d->command   = new gnomonBinaryImageFromImageCommand;
    d->keys = gnomonCore::binaryImageFromImage::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();

    //create the views
    this->sources()->addView();
    this->targets()->addView();

    emit parametersChanged();

    d->updateViewFormTypes();
    d->updatePool();
}

gnomonWorkspaceBinarization::~gnomonWorkspaceBinarization(void)
{
    gnomonBinaryImageFromImageCommand *command = (gnomonBinaryImageFromImageCommand *)d->command;
    if (command) {
        delete command;
    }
}
