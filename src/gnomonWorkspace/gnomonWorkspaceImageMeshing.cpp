#include "gnomonWorkspaceImageMeshing.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFromImage>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshFromImageCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>
#include <gnomonConfig.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceImageMeshing
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceImageMeshing::gnomonWorkspaceImageMeshing(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    WORKSPACEINIT(Solver, meshFromImage, gnomonMeshFromImageCommand)

    loadPluginGroup("meshFromImage");

    d->workspace = "Image Meshing";
    d->command = new gnomonMeshFromImageCommand;
    d->keys = gnomonCore::meshFromImage::pluginFactory().keys();
    d->algorithmsData = gnomonCore::meshFromImage::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();

}

gnomonWorkspaceImageMeshing::~gnomonWorkspaceImageMeshing()
{
    auto *command = (gnomonMeshFromImageCommand *)d->command;
    delete command;
}


//
// gnomonWorkspaceImageMeshing.cpp ends here
