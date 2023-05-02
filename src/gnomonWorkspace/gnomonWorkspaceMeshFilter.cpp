#include "gnomonWorkspaceMeshFilter.h"
#include "gnomonAlgorithmWorkspace_p.h"

//#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractCellImageFromImage>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshFilterCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>
#include <gnomonConfig.h>


gnomonWorkspaceMeshFilter::gnomonWorkspaceMeshFilter(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    WORKSPACEINIT(Solver, meshFilter, gnomonMeshFilterCommand)

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
