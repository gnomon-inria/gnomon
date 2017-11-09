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
#include "gnomonActorMeshCellComplex.h"
#include "gnomonActorMeshCellGraph.h"
#include "gnomonActorVolume.h"

#include "gnomonViewManager.h"

#include <gnomonCellComplex.h>
#include <gnomonCellGraph.h>

#include <vtkImageData.h>
#include <vtkPolyData.h>

class gnomonViewManagerPrivate
{
public:
    QHash<vtkPolyData *, gnomonActor *> meshes;
    QHash<vtkImageData *, gnomonActor *> volumes;
    QHash<gnomonCellComplex *, gnomonActor *> cellcomplexes;
    QHash<gnomonCellGraph *, gnomonActor *> cellgraphs;
};


gnomonActor *gnomonViewManager::actor(vtkPolyData *mesh)
{
    return d->meshes.value(mesh, NULL);
}

gnomonActor *gnomonViewManager::insert(vtkPolyData *mesh)
{
    gnomonActorMesh *actor = gnomonActorMesh::New();
    actor->setMesh(mesh);

    d->meshes.insert(mesh, actor);

    emit inserted(mesh);

    return actor;
}

void gnomonViewManager::remove(vtkPolyData *mesh)
{
    d->meshes.remove(mesh);

    emit removed(mesh);
}

QList<vtkPolyData *> gnomonViewManager::meshes(void)
{
    return d->meshes.keys();
}


gnomonActor *gnomonViewManager::actor(vtkImageData *volume)
{
    return d->volumes.value(volume, NULL);
}

gnomonActor *gnomonViewManager::insert(vtkImageData *volume)
{
    gnomonActorVolume *actor = gnomonActorVolume::New();
    actor->setVolume(volume);

    d->volumes.insert(volume, actor);

    emit inserted(volume);

    return actor;
}

void gnomonViewManager::remove(vtkImageData *volume)
{
    d->volumes.remove(volume);

    emit removed(volume);
}

QList<vtkImageData *> gnomonViewManager::volumes(void)
{
    return d->volumes.keys();
}


gnomonActor *gnomonViewManager::actor(gnomonCellComplex *cellcomplex)
{
    return d->cellcomplexes.value(cellcomplex, NULL);
}

gnomonActor *gnomonViewManager::insert(gnomonCellComplex *cellcomplex)
{
    qDebug()<<"View Manager Cell Complex Create";
    gnomonActorMeshCellComplex *actor = gnomonActorMeshCellComplex::New();
    actor->setCellComplex(cellcomplex);

    d->cellcomplexes.insert(cellcomplex, actor);
    qDebug()<<"View Manager Cell Complex Emit";

    emit inserted(cellcomplex);

    return actor;
}

void gnomonViewManager::remove(gnomonCellComplex *cellcomplex)
{
    d->cellcomplexes.remove(cellcomplex);

    emit removed(cellcomplex);
}

QList<gnomonCellComplex *> gnomonViewManager::cellcomplexes(void)
{
    return d->cellcomplexes.keys();
}


gnomonActor *gnomonViewManager::actor(gnomonCellGraph *cellgraph)
{
    return d->cellgraphs.value(cellgraph, NULL);
}

gnomonActor *gnomonViewManager::insert(gnomonCellGraph *cellgraph)
{
    gnomonActorMeshCellGraph *actor = gnomonActorMeshCellGraph::New();
    actor->setCellGraph(cellgraph);

    d->cellgraphs.insert(cellgraph, actor);

    emit inserted(cellgraph);

    return actor;
}

void gnomonViewManager::remove(gnomonCellGraph *cellgraph)
{
    d->cellgraphs.remove(cellgraph);

    emit removed(cellgraph);
}

QList<gnomonCellGraph *> gnomonViewManager::cellgraphs(void)
{
    return d->cellgraphs.keys();
}


void gnomonViewManager::clear(void)
{
    qDeleteAll(d->meshes.values());
    d->meshes.clear();

    qDeleteAll(d->volumes.values());
    d->volumes.clear();

    qDeleteAll(d->cellcomplexes.values());
    d->cellcomplexes.clear();

    qDeleteAll(d->cellgraphs.values());
    d->cellgraphs.clear();
}

void gnomonViewManager::update(void)
{
    foreach(gnomonActor *actor, d->meshes)
        actor->update();

    foreach(gnomonActor *actor, d->volumes)
        actor->update();

    foreach(gnomonActor *actor, d->cellcomplexes)
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
