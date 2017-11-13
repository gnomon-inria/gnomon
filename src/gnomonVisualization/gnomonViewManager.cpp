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

#include "gnomonViewManager.h"

#include "gnomonActor.h"
#include "gnomonActorImage.h"
#include "gnomonActorVolume.h"
#include "gnomonActorMesh.h"
#include "gnomonActorMeshCellComplex.h"
#include "gnomonActorMeshCellGraph.h"
#include "gnomonActorMeshCellImage.h"
#include "gnomonActorScalarBar.h"


#include "gnomonInspector.h"
#include "gnomonInspectorCellGraph.h"
#include "gnomonInspectorImage.h"
#include "gnomonInspectorMain.h"
#include "gnomonInspectorViewTree.h"
#include "gnomonInspectorViewWidget.h"
#include "gnomonInspectorVolume.h"
#include "gnomonInspectorSlicePlanes.h"

#include "gnomonStringEditor.h"
#include "gnomonClutEditor.h"
#include "gnomonDoubleRangeEditor.h"

#include <gnomonCellComplex.h>
#include <gnomonCellGraph.h>

#include <vtkImageData.h>
#include <vtkPolyData.h>

#include <cmath>

class gnomonViewManagerPrivate
{

public:
    gnomonInspectorViewTree   *inspector_tree;
    gnomonInspectorViewWidget *inspector_widget;
    gnomonInspectorMain       *inspector_main;

public:
    QHash<gnomonCellComplex *, gnomonActor *> cellcomplexes;
    QHash<gnomonCellGraph *, gnomonActor *>   cellgraphs;
    QHash<gnomonCellImage *, gnomonActor *>   cellimages;
    QHash<vtkImageData *, gnomonActor *>      images;
    QHash<vtkPolyData *, gnomonActor *>       meshes;
    QHash<vtkImageData *, gnomonActor *>      volumes;
    QHash<vtkImageData *, gnomonActor *>      scalar_bars;
    QHash<gnomonActorMeshCellGraph *, QList< gnomonInspectorCellGraph * > > cellgraphs_inspectors;
    QHash<gnomonActorVolume *, QList< gnomonInspectorImage * > > volumes_inspectors;
    QHash<gnomonInspectorCellGraph *, gnomonActorMeshCellGraph *> cellgraphs_inspector_actors;
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

gnomonActor *gnomonViewManager::insert(vtkImageData *image)
{
    // ///////////////////////////////////////////////////////////////////
    // Actors are created here
    // ///////////////////////////////////////////////////////////////////
    gnomonActorVolume *actor_volume = gnomonActorVolume::New();
    actor_volume->setVolume(image);
    d->volumes.insert(image, actor_volume);
    QTreeWidgetItem *actor_volume_item = d->inspector_tree->insert(actor_volume);
    emit inserted(actor_volume);

    gnomonActorImage *actor_image = gnomonActorImage::New();
    actor_image->setImage(image);
    //Replaces other insert ... TODO
    d->images.insert(image, actor_image);
    QTreeWidgetItem *actor_image_item = d->inspector_tree->insert(actor_image);
    emit inserted(actor_image);

    gnomonActorScalarBar *actor_scalar_bar = gnomonActorScalarBar::New();
    actor_scalar_bar->setData(image);
    //Replaces other insert ... TODO
    d->scalar_bars.insert(image, actor_scalar_bar);
    QTreeWidgetItem *actor_sclar_bar_item = d->inspector_tree->insert(actor_scalar_bar);
    emit inserted(actor_scalar_bar);

    // ///////////////////////////////////////////////////////////////////
    // Inspectors are created and connected to actors here
    // ///////////////////////////////////////////////////////////////////
    gnomonInspectorVolume *volume_inspector = new gnomonInspectorVolume();

    volume_inspector->editor()->setRange(actor_volume->rangeMin(), actor_volume->rangeMax());
    volume_inspector->editor()->setHistogram(actor_volume->histogram());
    volume_inspector->editor()->setOpacityTransferFunction(static_cast<vtkPiecewiseFunction *>(actor_volume->opacityTransferFunction()));
    volume_inspector->editor()->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(actor_volume->colorTransferFunction()));
    connect(volume_inspector->editor(), &gnomonClutEditor::updated, [=] () {
            actor_volume->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(volume_inspector->editor()->colorTransferFunction()));
            actor_volume->setOpacityTransferFunction(static_cast<vtkPiecewiseFunction *>(volume_inspector->editor()->opacityTransferFunction()));
        });
    connect(volume_inspector->editor(), &gnomonClutEditor::updated, [=] () {
            actor_image->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(volume_inspector->editor()->colorTransferFunction()));
        });
     connect(volume_inspector->editor(), &gnomonClutEditor::updated, [=] () {
            actor_scalar_bar->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(volume_inspector->editor()->colorTransferFunction()));
        });

    QList< gnomonInspectorVolume * > volumes_inspectors;
    volumes_inspectors.append(volume_inspector);
    d->inspector_tree->addChild(actor_volume_item, volume_inspector);

    // ///////////////////////////////////////////////////////////////////
    gnomonInspectorSlicePlanes *slice_planes_inspector = new gnomonInspectorSlicePlanes();
    connect(slice_planes_inspector, &gnomonInspectorSlicePlanes::xPlanePosChanged,
            [=] () { actor_image->setXPlanePos(slice_planes_inspector->xPlanePos());
            });
    connect(slice_planes_inspector, &gnomonInspectorSlicePlanes::yPlanePosChanged,
            [=] () { actor_image->setYPlanePos(slice_planes_inspector->yPlanePos());
            });
    connect(slice_planes_inspector, &gnomonInspectorSlicePlanes::zPlanePosChanged,
            [=] () { actor_image->setZPlanePos(slice_planes_inspector->zPlanePos());
            });
    connect(slice_planes_inspector, &gnomonInspectorSlicePlanes::xPlaneOpacityChanged,
            [=] () { actor_image->setXPlaneOpacity(slice_planes_inspector->xPlaneOpacity());
            });
    connect(slice_planes_inspector, &gnomonInspectorSlicePlanes::yPlaneOpacityChanged,
            [=] () { actor_image->setYPlaneOpacity(slice_planes_inspector->yPlaneOpacity());
            });
    connect(slice_planes_inspector, &gnomonInspectorSlicePlanes::zPlaneOpacityChanged,
            [=] () { actor_image->setZPlaneOpacity(slice_planes_inspector->zPlaneOpacity());
            });

    QList< gnomonInspectorImage * > images_inspectors;
    images_inspectors.append(slice_planes_inspector);
    d->inspector_tree->addChild(actor_image_item, slice_planes_inspector);

    //TODO what to return ????
    return actor_volume;
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

    gnomonInspectorCellGraph *cellgraph_inspector = new gnomonInspectorCellGraph();

    QStringList vertexProperties = cellgraph->vertexPropertyNames();
    vertexProperties.insert(0,"");
    cellgraph_inspector->vertexPropertyEditor()->setList(vertexProperties);
    connect(cellgraph_inspector, &gnomonInspectorCellGraph::vertexPropertyUpdated, [=] () { actor->setVertexProperty(cellgraph_inspector->vertexProperty()); });

    connect(cellgraph_inspector, &gnomonInspectorCellGraph::vertexSizeUpdated, [=] () { actor->setVertexSize(cellgraph_inspector->vertexSize()); });
    connect(cellgraph_inspector, &gnomonInspectorCellGraph::edgeOpacityUpdated, [=] () { actor->setEdgeOpacity(cellgraph_inspector->edgeOpacity()); });
    connect(cellgraph_inspector, &gnomonInspectorCellGraph::edgeLinewidthUpdated, [=] () { actor->setEdgeLinewidth(cellgraph_inspector->edgeLinewidth()); });

    QMap<QString, void(gnomonInspectorCellGraph::*)(void)> sliceSignals;
    sliceSignals["x"] = &gnomonInspectorCellGraph::xSliceUpdated;
    sliceSignals["y"] = &gnomonInspectorCellGraph::ySliceUpdated;
    sliceSignals["z"] = &gnomonInspectorCellGraph::zSliceUpdated;

    for (const auto& dim : sliceSignals.keys()) {
        QString barycenterProp("barycenter_");
        barycenterProp.append(dim);
        QMap<long, QVariant> positions = cellgraph->vertexProperty(barycenterProp);
        QList<double> points;
        for (const auto& vertexId : cellgraph->vertexIds()) {
            points<<positions[vertexId].value<double>();
        }
        double pointMin = floor(*std::min_element(points.begin(),points.end()));
        double pointMax = ceil(*std::max_element(points.begin(),points.end()));
        cellgraph_inspector->sliceEditor(dim)->setRange(pointMin,pointMax);
        cellgraph_inspector->sliceEditor(dim)->setValueMin(pointMin);
        cellgraph_inspector->sliceEditor(dim)->setValueMax(pointMax);
        connect(cellgraph_inspector, sliceSignals[dim], [=] () { actor->setSlice(dim,cellgraph_inspector->slice(dim)); });
    }

    QList< gnomonInspectorCellGraph * > cellgraphs_inspectors;
    cellgraphs_inspectors.append(cellgraph_inspector);
    d->cellgraphs_inspectors.insert(actor, cellgraphs_inspectors);
    d->cellgraphs_inspector_actors.insert(cellgraph_inspector,actor);

    QTreeWidgetItem *tree_item = d->inspector_tree->insert(actor);
    d->inspector_tree->addChild(tree_item, cellgraph_inspector);

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

    qDeleteAll(d->images.values());
    d->images.clear();

    qDeleteAll(d->scalar_bars.values());
    d->scalar_bars.clear();

    qDeleteAll(d->cellcomplexes.values());
    d->cellcomplexes.clear();

    qDeleteAll(d->cellgraphs.values());
    d->cellgraphs.clear();

    qDeleteAll(d->cellimages.values());
    d->cellimages.clear();
}

void gnomonViewManager::update(void)
{
    for (auto actor : d->cellcomplexes) {
        actor->update();
    }

    for (auto actor : d->cellgraphs) {
        actor->update();
    }

    for (auto actor : d->cellimages) {
        actor->update();
    }

    for (auto actor : d->images) {
        actor->update();
    }

    for(auto actor : d->meshes) {
        actor->update();
    }

    for (auto actor : d->volumes) {
        actor->update();
    }

    for (auto actor : d->scalar_bars) {
        actor->update();
    }
}

// ///////////////////////////////////////////////////////////////////
// Protected slots
// ///////////////////////////////////////////////////////////////////

void gnomonViewManager::onVolumeSelected(gnomonActorVolume *volume)
{
    //to implement
}

void gnomonViewManager::onImageSelected(gnomonActorImage *image)
{
    //to implement
}

void gnomonViewManager::onScalarBarSelected(gnomonActorScalarBar *scalar_bar)
{
    //to implement
}

void gnomonViewManager::onMeshSelected(vtkPolyData *mesh)
{
    //to implement
}

void gnomonViewManager::onCellGraphSelected(gnomonActorMeshCellGraph *graph)
{
    //to implement
}

void gnomonViewManager::onInspectorImageSelected(gnomonInspectorImage *inspector)
{
    d->inspector_widget->setInspector(inspector, true);

    emit selected(d->inspector_widget);
    //to implement
}

void gnomonViewManager::onInspectorVolumeSelected(gnomonInspectorVolume *inspector)
{
    d->inspector_widget->setInspector(inspector, true);

    emit selected(d->inspector_widget);
    //to implement
}

void gnomonViewManager::onInspectorCellGraphSelected(gnomonInspectorCellGraph *inspector)
{
    d->inspector_widget->setInspector(inspector, true);

    gnomonActorMeshCellGraph *actor = d->cellgraphs_inspector_actors[inspector];

    emit selected(d->inspector_widget);
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

    connect(d->inspector_tree, SIGNAL(selected(gnomonInspectorCellGraph *)), this, SLOT(onInspectorCellGraphSelected(gnomonInspectorCellGraph *)));
    connect(d->inspector_tree, SIGNAL(selected(gnomonInspectorImage *)), this, SLOT(onInspectorImageSelected(gnomonInspectorImage *)));
    connect(d->inspector_tree, SIGNAL(selected(gnomonInspectorVolume *)), this, SLOT(onInspectorVolumeSelected(gnomonInspectorVolume *)));
    connect(d->inspector_tree, SIGNAL(selected(vtkPolyData *)), this, SLOT(onMeshSelected(vtkPolyData *)));
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
