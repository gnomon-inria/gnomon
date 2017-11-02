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

#include "tissueActor.h"
#include "tissueActorMesh.h"
#include "tissueActorVolume.h"
#include "tissueViewManager.h"

#include <vtkImageData.h>
#include <vtkPolyData.h>

class tissueViewManagerPrivate
{
public:
    QHash<vtkPolyData *, tissueActor *> meshes;
    QHash<vtkImageData *, tissueActor *> volumes;
};

tissueActor *tissueViewManager::actor(vtkPolyData *mesh)
{
    return d->meshes.value(mesh, NULL);
}

tissueActor *tissueViewManager::actor(vtkImageData *volume)
{
    return d->volumes.value(volume, NULL);
}

tissueActor *tissueViewManager::insert(vtkPolyData *mesh)
{
    tissueActorMesh *actor = tissueActorMesh::New();
    actor->setMesh(mesh);

    d->meshes.insert(mesh, actor);

    emit inserted(mesh);

    return actor;
}

tissueActor *tissueViewManager::insert(vtkImageData *volume)
{
    tissueActorVolume *actor = tissueActorVolume::New();
    actor->setVolume(volume);

    d->volumes.insert(volume, actor);

    emit inserted(volume);

    return actor;
}

void tissueViewManager::remove(vtkPolyData *mesh)
{
    d->meshes.remove(mesh);

    emit removed(mesh);
}

void tissueViewManager::remove(vtkImageData *volume)
{
    d->volumes.remove(volume);

    emit removed(volume);
}

QList<vtkPolyData *> tissueViewManager::meshes(void)
{
    return d->meshes.keys();
}

QList<vtkImageData *> tissueViewManager::volumes(void)
{
    return d->volumes.keys();
}

void tissueViewManager::clear(void)
{
    qDeleteAll(d->meshes.values());
    qDeleteAll(d->volumes.values());

    d->meshes.clear();
    d->volumes.clear();
}

void tissueViewManager::update(void)
{
    foreach(tissueActor *actor, d->meshes)
        actor->update();

    foreach(tissueActor *actor, d->volumes)
        actor->update();
}

tissueViewManager::tissueViewManager(void) : QObject(), d(new tissueViewManagerPrivate)
{

}

tissueViewManager::~tissueViewManager(void)
{
    this->clear();

    delete d;

    d = NULL;
}

//
// tissueViewManager.cpp ends here
