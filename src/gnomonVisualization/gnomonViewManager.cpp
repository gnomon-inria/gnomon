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
#include "gnomonActorMeshCellImage.h"
#include "gnomonActorVolume.h"

#include "gnomonViewManager.h"
#include "gnomonInspector.h"
#include "gnomonInspectorViewTree.h"
#include "gnomonInspectorViewWidget.h"
#include "gnomonInspectorMain.h"
#include "gnomonInspectorVolume.h"
#include "gnomonClutEditor.h"
#include <gnomonCellComplex.h>
#include <gnomonCellGraph.h>

#include <vtkImageData.h>
#include <vtkPolyData.h>

class gnomonViewManagerPrivate
{
public:
    gnomonInspectorViewTree *inspector_tree;
    gnomonInspectorViewWidget *inspector_widget;
    gnomonInspectorMain *inspector_main;

public:
    QHash<vtkPolyData *, gnomonActor *> meshes;
    QHash<vtkImageData *, gnomonActor *> volumes;
    QHash<gnomonActorVolume *, QList< gnomonInspectorVolume * > > volumes_inspectors;
    QHash<gnomonCellComplex *, gnomonActor *> cellcomplexes;
    QHash<gnomonCellGraph *, gnomonActor *> cellgraphs;
    QHash<gnomonCellImage *, gnomonActor *> cellimages;
};

gnomonInspectorViewTree *gnomonViewManager::inspectorTree(void)
{
    return d->inspector_tree;
}

gnomonInspectorViewWidget *gnomonViewManager::inspectorWidget(void)
{
    return d->inspector_widget;
}

gnomonInspectorMain *gnomonViewManager::inspectorMain(void)
{
    return d->inspector_main;
}

gnomonActor *gnomonViewManager::actor(vtkPolyData *mesh)
{
    return d->meshes.value(mesh, NULL);
}

gnomonActor *gnomonViewManager::insert(vtkPolyData *mesh)
{
    gnomonActorMesh *actor = gnomonActorMesh::New();
    actor->setMesh(mesh);
    // ///////////////////////////////////////////////////////////////////
    // Inspectors are created here
    // ///////////////////////////////////////////////////////////////////

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
    // ///////////////////////////////////////////////////////////////////
    // Actor is created here
    // ///////////////////////////////////////////////////////////////////
    gnomonActorVolume *actor = gnomonActorVolume::New();
    actor->setVolume(volume);

    d->volumes.insert(volume, actor);
    // ///////////////////////////////////////////////////////////////////
    // Inspectors are created here
    // ///////////////////////////////////////////////////////////////////
    gnomonInspectorVolume *volume_inspector = new gnomonInspectorVolume();

    connect(volume_inspector->editor(), &gnomonClutEditor::updated, [=] () {
            actor->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(volume_inspector->editor()->colorTransferFunction()));
            actor->setOpacityTransferFunction(static_cast<vtkPiecewiseFunction *>(volume_inspector->editor()->opacityTransferFunction()));
        });

    QList< gnomonInspectorVolume * > volumes_inspectors;
    volumes_inspectors.append(volume_inspector);
    d->volumes_inspectors.insert(actor, volumes_inspectors);

    QTreeWidgetItem *tree_item = d->inspector_tree->insert(actor);
    qWarning() << tree_item;
    qWarning() << Q_FUNC_INFO;
    qWarning() << d->inspector_tree->addChild(tree_item, volume_inspector);

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
    gnomonActorMeshCellComplex *actor = gnomonActorMeshCellComplex::New();
    actor->setCellComplex(cellcomplex);

    d->cellcomplexes.insert(cellcomplex, actor);

    d->inspector_tree->insert(cellcomplex);

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

    d->inspector_tree->insert(cellgraph);

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


gnomonActor *gnomonViewManager::actor(gnomonCellImage *cellimage)
{
    return d->cellimages.value(cellimage, NULL);
}

gnomonActor *gnomonViewManager::insert(gnomonCellImage *cellimage)
{
    gnomonActorMeshCellImage *actor = gnomonActorMeshCellImage::New();
    actor->setCellImage(cellimage);

    d->cellimages.insert(cellimage, actor);

    emit inserted(cellimage);

    return actor;
}

void gnomonViewManager::remove(gnomonCellImage *cellimage)
{
    d->cellimages.remove(cellimage);

    emit removed(cellimage);
}

QList<gnomonCellImage *> gnomonViewManager::cellimages(void)
{
    return d->cellimages.keys();
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

    qDeleteAll(d->cellimages.values());
    d->cellimages.clear();
}

void gnomonViewManager::update(void)
{
    for(auto actor : d->meshes) {
        actor->update();
    }

    for (auto actor : d->volumes) {
        actor->update();
    }

    for (auto actor : d->cellcomplexes) {
        actor->update();
    }

    for (auto actor : d->cellgraphs) {
        actor->update();
    }

    for (auto actor : d->cellimages) {
        actor->update();
    }
}

// ///////////////////////////////////////////////////////////////////
// Protected slots
// ///////////////////////////////////////////////////////////////////
void gnomonViewManager::onVolumeSelected(gnomonActorVolume *volume)
{
    d->inspector_widget->setActor(volume, true);

    emit selected(d->inspector_widget);
    //to implement
}

void gnomonViewManager::onInspectorVolumeSelected(gnomonInspectorVolume *inspector)
{
    d->inspector_widget->setInspector(inspector, true);

    emit selected(d->inspector_widget);
    //to implement
}

void gnomonViewManager::onMeshSelected(vtkPolyData *mesh)
{
    //to implement
}

// ///////////////////////////////////////////////////////////////////
// Constructors and destructors
// ///////////////////////////////////////////////////////////////////

gnomonViewManager::gnomonViewManager(void) : QObject(), d(new gnomonViewManagerPrivate)
{
    d->inspector_main = new gnomonInspectorMain();
    d->inspector_tree = new gnomonInspectorViewTree(d->inspector_main);
    d->inspector_widget = new gnomonInspectorViewWidget(d->inspector_main);

    d->inspector_main->addWidget(d->inspector_tree);
    d->inspector_main->addWidget(d->inspector_widget);

    connect(d->inspector_tree, SIGNAL(selected(vtkPolyData *)), this, SLOT(onMeshSelected(vtkPolyData *)));
    connect(d->inspector_tree, SIGNAL(selected(gnomonInspectorVolume *)), this, SLOT(onInspectorVolumeSelected(gnomonInspectorVolume *)));
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
