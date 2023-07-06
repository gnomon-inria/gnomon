#include "gnomonWorkspaceMeshFilter.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFilter.h>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshFilterCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceMeshFilter
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceMeshFilter::gnomonWorkspaceMeshFilter(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("meshFilter");

    d->workspace = "Mesh Processing";
    d->command = new gnomonMeshFilterCommand;
    d->keys = gnomonCore::meshFilter::pluginFactory().keys();
    d->algorithmsData = gnomonCore::meshFilter::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();
}

gnomonWorkspaceMeshFilter::~gnomonWorkspaceMeshFilter(void)
{
    auto *command = (gnomonMeshFilterCommand *)d->command;
    delete command;
}
