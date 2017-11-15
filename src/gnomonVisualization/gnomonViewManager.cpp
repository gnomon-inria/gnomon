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

#include "gnomonInspector.h"
#include "gnomonInspectorViewTree.h"

#include "gnomonActor.h"

#include "gnomonActorVolume.h"
#include "gnomonActorMeshCellComplex.h"
#include "gnomonActorMeshCellGraph.h"
#include "gnomonActorMeshCellImage.h"

#include <gnomonCellComplex.h>
#include <gnomonCellGraph.h>

#include <vtkImageData.h>

#include <dtkImaging>

class gnomonViewManagerPrivate
{

public:
    gnomonInspector       *inspector;

public:
    QHash<dtkImage *, gnomonActor *>      images;
    QHash<gnomonCellComplex *, gnomonActor *> cellcomplexes;
    QHash<gnomonCellGraph *, gnomonActor *>   cellgraphs;
    QHash<gnomonCellImage *, gnomonActor *>   cellimages;
};

gnomonActor *gnomonViewManager::actor(dtkImage *data)
{
    return d->images.value(data, NULL);
}

gnomonActor *gnomonViewManager::actor(gnomonCellComplex *data)
{
    return d->cellcomplexes.value(data, NULL);
}

gnomonActor *gnomonViewManager::actor(gnomonCellGraph *data)
{
    return d->cellgraphs.value(data, NULL);
}

gnomonActor *gnomonViewManager::actor(gnomonCellImage *data)
{
    return d->cellimages.value(data, NULL);
}

gnomonInspector *gnomonViewManager::inspector(void)
{
    return d->inspector;
}

gnomonActor *gnomonViewManager::insert(dtkImage *data)
{
    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    if (!converter) {
        qWarning() << "can't create vtk converter!";
        return nullptr;
    }
    converter->setInput(data);
    converter->convert();
    vtkImageData *vtk_image = static_cast<vtkImageData*>(converter->output());
    if (!vtk_image) {
        qWarning() << "Can't convert image to vtkImageData !";
        return nullptr;
    }

    gnomonActorVolume *actor = gnomonActorVolume::New();
    actor->setVolume(vtk_image);
    d->inspector->tree()->insert(actor);
    d->images.insert(data, actor);
    emit inserted(data);
    emit inserted(actor);

    return actor;
}

gnomonActor *gnomonViewManager::insert(gnomonCellComplex *data)
{
    gnomonActorMeshCellComplex *actor = gnomonActorMeshCellComplex::New();
    actor->setCellComplex(data);
    d->inspector->tree()->insert(actor);
    d->cellcomplexes.insert(data, actor);
    emit inserted(data);
    emit inserted(actor);

    return actor;
}

gnomonActor *gnomonViewManager::insert(gnomonCellGraph *data)
{
    gnomonActorMeshCellGraph *actor = gnomonActorMeshCellGraph::New();
    actor->setCellGraph(data);
    d->inspector->tree()->insert(actor);
    d->cellgraphs.insert(data, actor);
    emit inserted(data);
    emit inserted(actor);

    return actor;
}

gnomonActor *gnomonViewManager::insert(gnomonCellImage *data)
{
    gnomonActorMeshCellImage *actor = gnomonActorMeshCellImage::New();
    actor->setCellImage(data);
    d->inspector->tree()->insert(actor);
    d->cellimages.insert(data, actor);
    emit inserted(data);
    emit inserted(actor);

    return actor;
}

void gnomonViewManager::remove(dtkImage *data)
{
    d->images.remove(data);

    emit removed(data);
}

void gnomonViewManager::remove(gnomonCellComplex *data)
{
    d->cellcomplexes.remove(data);

    emit removed(data);
}

void gnomonViewManager::remove(gnomonCellGraph *data)
{
    d->cellgraphs.remove(data);

    emit removed(data);
}

void gnomonViewManager::remove(gnomonCellImage *data)
{
    d->cellimages.remove(data);

    emit removed(data);
}

QList<dtkImage *> gnomonViewManager::images(void)
{
    return d->images.keys();
}

QList<gnomonCellComplex *> gnomonViewManager::cellcomplexes(void)
{
    return d->cellcomplexes.keys();
}

QList<gnomonCellGraph *> gnomonViewManager::cellgraphs(void)
{
    return d->cellgraphs.keys();
}

QList<gnomonCellImage *> gnomonViewManager::cellimages(void)
{
    return d->cellimages.keys();
}

void gnomonViewManager::clear(void)
{
    qDeleteAll(d->images.values());
    d->images.clear();

    qDeleteAll(d->cellcomplexes.values());
    d->cellcomplexes.clear();

    qDeleteAll(d->cellgraphs.values());
    d->cellgraphs.clear();

    qDeleteAll(d->cellimages.values());
    d->cellimages.clear();
}

void gnomonViewManager::update(void)
{
    for (auto actor : d->images) {
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

void gnomonViewManager::onActorVolumeSelected(gnomonActorVolume *actor)
{
    //to implement
}

void gnomonViewManager::onActorCellComplexSelected(gnomonActorMeshCellComplex *actor)
{
    //to implement
}

void gnomonViewManager::onActorCellGraphSelected(gnomonActorMeshCellGraph *actor)
{
    //to implement
}

void gnomonViewManager::onActorCellImageSelected(gnomonActorMeshCellImage *actor)
{
    //to implement
}

// ///////////////////////////////////////////////////////////////////
// Constructors and destructors
// ///////////////////////////////////////////////////////////////////

gnomonViewManager::gnomonViewManager(void) : QObject(), d(new gnomonViewManagerPrivate)
{
    d->inspector = new gnomonInspector();
}

gnomonViewManager::~gnomonViewManager(void)
{
    this->clear();

    delete d->inspector;
    delete d;

    d = NULL;
}

//
// gnomonViewManager.cpp ends here
