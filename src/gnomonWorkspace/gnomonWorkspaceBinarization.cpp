#include "gnomonWorkspaceBinarization.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageFromImage>
#include <gnomonCore/gnomonCommand/gnomonBinaryImage/gnomonBinaryImageFromImageCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

gnomonWorkspaceBinarization::gnomonWorkspaceBinarization(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("binaryImageFromImage");

    d->workspace = "Binarization";
    d->command   = new gnomonBinaryImageFromImageCommand;
    d->keys = gnomonCore::binaryImageFromImage::pluginFactory().keys();
    d->algorithmsData = gnomonCore::binaryImageFromImage::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();
}

gnomonWorkspaceBinarization::~gnomonWorkspaceBinarization(void)
{
    gnomonBinaryImageFromImageCommand *command = (gnomonBinaryImageFromImageCommand *)d->command;
    if (command) {
        delete command;
    }
}
