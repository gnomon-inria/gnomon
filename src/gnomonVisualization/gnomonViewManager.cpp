// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonActor.h"
#include "gnomonActorMesh.h"
#include "gnomonActorMeshCellGraph.h"
#include "gnomonActorVolume.h"

#include "gnomonViewManager.h"

#include <gnomonCellGraph.h>

#include <vtkImageData.h>
#include <vtkPolyData.h>

class gnomonViewManagerPrivate
{
public:
    QHash<vtkPolyData *, gnomonActor *> meshes;
    QHash<vtkImageData *, gnomonActor *> volumes;
    QHash<gnomonCellGraph *, gnomonActor *> cellgraphs;
};

gnomonActor *gnomonViewManager::actor(vtkPolyData *mesh)
{
    return d->meshes.value(mesh, NULL);
}

gnomonActor *gnomonViewManager::actor(vtkImageData *volume)
{
    return d->volumes.value(volume, NULL);
}

gnomonActor *gnomonViewManager::actor(gnomonCellGraph *cellgraph)
{
    return d->cellgraphs.value(cellgraph, NULL);
}

gnomonActor *gnomonViewManager::insert(vtkPolyData *mesh)
{
    gnomonActorMesh *actor = gnomonActorMesh::New();
    actor->setMesh(mesh);

    d->meshes.insert(mesh, actor);

    emit inserted(mesh);

    return actor;
}

gnomonActor *gnomonViewManager::insert(vtkImageData *volume)
{
    gnomonActorVolume *actor = gnomonActorVolume::New();
    actor->setVolume(volume);

    d->volumes.insert(volume, actor);

    emit inserted(volume);

    return actor;
}

gnomonActor *gnomonViewManager::insert(gnomonCellGraph *cellgraph)
{
    gnomonActorMeshCellGraph *actor = gnomonActorMeshCellGraph::New();
    actor->setCellGraph(cellgraph);

    qDebug()<<"--> View Insert Cell Graph"; 

    d->cellgraphs.insert(cellgraph, actor);

    qDebug()<<"<-- View Insert Cell Graph"; 

    emit inserted(cellgraph);

    return actor;
}


void gnomonViewManager::remove(vtkPolyData *mesh)
{
    d->meshes.remove(mesh);

    emit removed(mesh);
}

void gnomonViewManager::remove(vtkImageData *volume)
{
    d->volumes.remove(volume);

    emit removed(volume);
}

void gnomonViewManager::remove(gnomonCellGraph *cellgraph)
{
    d->cellgraphs.remove(cellgraph);

    emit removed(cellgraph);
}

QList<vtkPolyData *> gnomonViewManager::meshes(void)
{
    return d->meshes.keys();
}

QList<vtkImageData *> gnomonViewManager::volumes(void)
{
    return d->volumes.keys();
}

QList<gnomonCellGraph *> gnomonViewManager::cellgraphs(void)
{
    return d->cellgraphs.keys();
}

void gnomonViewManager::clear(void)
{
    qDeleteAll(d->meshes.values());
    qDeleteAll(d->volumes.values());
    qDeleteAll(d->cellgraphs.values());

    d->meshes.clear();
    d->volumes.clear();
    d->cellgraphs.clear();
}

void gnomonViewManager::update(void)
{
    foreach(gnomonActor *actor, d->meshes)
        actor->update();

    foreach(gnomonActor *actor, d->volumes)
        actor->update();

    foreach(gnomonActor *actor, d->cellgraphs)
        actor->update();
}

gnomonViewManager::gnomonViewManager(void) : QObject(), d(new gnomonViewManagerPrivate)
{

}

gnomonViewManager::~gnomonViewManager(void)
{
    this->clear();

    delete d;

    d = NULL;
}

//
// gnomonViewManager.cpp ends here
