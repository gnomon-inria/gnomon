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

#include "gnomonInspector.h"
// #include "gnomonInspectorCellComplex.h"
// #include "gnomonInspectorCellImage.h"
#include "gnomonInspectorCellGraph.h"
#include "gnomonInspectorViewTree.h"
#include "gnomonInspectorViewWidget.h"
#include "gnomonInspectorVolume.h"

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidgetPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorPrivate
{
public:
    gnomonInspectorViewTree *tree;
    gnomonInspectorViewWidget *widget;
};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspector::gnomonInspector(QWidget *parent) : QSplitter(Qt::Vertical, parent), d(new gnomonInspectorPrivate)
{
    this->setHandleWidth(2);

    d->tree = new gnomonInspectorViewTree(this);
    d->widget = new gnomonInspectorViewWidget(this);

    this->addWidget(d->tree);
    this->addWidget(d->widget);

    connect(d->tree, SIGNAL(selected(gnomonActorMeshCellComplex *)), this, SLOT(onActorMeshCellComplexSelected(gnomonActorMeshCellComplex *)));
    connect(d->tree, SIGNAL(selected(gnomonActorMeshCellGraph *)), this, SLOT(onActorMeshCellGraphSelected(gnomonActorMeshCellGraph *)));
    connect(d->tree, SIGNAL(selected(gnomonActorMeshCellImage *)), this, SLOT(onActorMeshCellImageSelected(gnomonActorMeshCellImage *)));
    connect(d->tree, SIGNAL(selected(gnomonActorVolume *)), this, SLOT(onActorVolumeSelected(gnomonActorVolume *)));
}

gnomonInspector::~gnomonInspector(void)
{

}

gnomonInspectorViewTree *gnomonInspector::tree(void)
{
    return d->tree;
}

void gnomonInspector::onActorMeshCellComplexSelected(gnomonActorMeshCellComplex *actor)
{
    qDebug() << Q_FUNC_INFO << " TO IMPLEMENT";
}

void gnomonInspector::onActorMeshCellGraphSelected(gnomonActorMeshCellGraph *actor)
{
    gnomonInspectorCellGraph *inspector = new gnomonInspectorCellGraph(this);
    inspector->setActor(actor);

    d->widget->setInspector(inspector);
}

void gnomonInspector::onActorMeshCellImageSelected(gnomonActorMeshCellImage *actor)
{
    qDebug() << Q_FUNC_INFO << " TO IMPLEMENT";
}

void gnomonInspector::onActorVolumeSelected(gnomonActorVolume *actor)
{
    gnomonInspectorVolume *inspector = new gnomonInspectorVolume(this);
    inspector->setActor(actor);

    d->widget->setInspector(inspector);
}

//
// gnomonInspector.cpp ends here
