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
#include "gnomonActorVolume.h"
#include "gnomonViewManager.h"
#include "gnomonInspectorViewTree.h"
#include "gnomonInspectorViewWidget.h"

#include <vtkImageData.h>
#include <vtkPolyData.h>

class gnomonViewManagerPrivate
{
public:
    gnomonInspectorViewTree *inspector_tree;
    gnomonInspectorViewWidget *inspector_widget;

public:
    QHash<vtkPolyData *, gnomonActor *> meshes;
    QHash<vtkImageData *, gnomonActor *> volumes;
};

gnomonInspectorViewTree *gnomonViewManager::inspectorTree(void)
{
    return d->inspector_tree;
}

gnomonInspectorViewWidget *gnomonViewManager::inspectorWidget(void)
{
    return d->inspector_widget;
}

gnomonActor *gnomonViewManager::actor(vtkPolyData *mesh)
{
    return d->meshes.value(mesh, NULL);
}

gnomonActor *gnomonViewManager::actor(vtkImageData *volume)
{
    return d->volumes.value(volume, NULL);
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

    d->inspector_tree->insert(volume);

    emit inserted(volume);

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

QList<vtkPolyData *> gnomonViewManager::meshes(void)
{
    return d->meshes.keys();
}

QList<vtkImageData *> gnomonViewManager::volumes(void)
{
    return d->volumes.keys();
}

void gnomonViewManager::clear(void)
{
    qDeleteAll(d->meshes.values());
    qDeleteAll(d->volumes.values());

    d->meshes.clear();
    d->volumes.clear();
}

void gnomonViewManager::update(void)
{
    for(auto actor : d->meshes) {
        actor->update();
    }

    for (auto actor : d->volumes) {
        actor->update();
    }
}

// ///////////////////////////////////////////////////////////////////
// Protected slots
// ///////////////////////////////////////////////////////////////////
void gnomonViewManager::onVolumeSelected(vtkImageData *volume)
{
    d->inspector_widget->setActor(actor(volume), true);

    emit selected(d->inspector_widget);

    //     numVolumeActor *actor = dynamic_cast<numVolumeActor *>(d->view->manager()->actor(volume));

    //     if(actor) {
    //         d->clut->setVolume(actor->vol());
    //         d->clut->setVolumeProperty(actor->volumeProperty());
    //         d->clut->setMapper(actor->mapper());
    //         d->clut->setColorTransferFunction(actor->colorTransferFunction());
    //         d->clut->setOpacityTransferFunction(actor->opacityTransferFunction());
    //         d->clut->setColorAndOpacityTransferFunction(actor->colorTransferFunction(), actor->opacityTransferFunction());
    //     }
    // }
}

void gnomonViewManager::onMeshSelected(vtkPolyData *mesh)
{
    // if(!d->view)
    //     return;

    // this->setMesh(mesh);

    // if(numActor *actor = d->view->manager()->actor(mesh))
    //     d->widget->setActor(actor, actor->isVisible());
}

// ///////////////////////////////////////////////////////////////////
// Constructors and destructors
// ///////////////////////////////////////////////////////////////////

gnomonViewManager::gnomonViewManager(void) : QObject(), d(new gnomonViewManagerPrivate)
{
    d->inspector_tree = new gnomonInspectorViewTree();
    d->inspector_widget = new gnomonInspectorViewWidget();

    connect(d->inspector_tree, SIGNAL(selected(vtkPolyData *)), this, SLOT(onMeshSelected(vtkPolyData *)));
    connect(d->inspector_tree, SIGNAL(selected(vtkImageData *)), this, SLOT(onVolumeSelected(vtkImageData *)));
}

gnomonViewManager::~gnomonViewManager(void)
{
    this->clear();
    delete d->inspector_tree;
    delete d->inspector_widget;
    delete d;

    d = NULL;
}

//
// gnomonViewManager.cpp ends here
