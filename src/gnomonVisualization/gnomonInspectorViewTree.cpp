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

#include "gnomonInspectorViewTree.h"

#include "gnomonActor.h"
#include "gnomonActorVolume.h"
#include "gnomonActorImage.h"
#include "gnomonActorScalarBar.h"
#include "gnomonActorMeshCellGraph.h"
#include "gnomonView.h"
#include "gnomonViewManager.h"

#include <QtCore>
#include <QtWidgets>

#include <vtkPolyData.h>
#include <vtkImageData.h>

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewTreePrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorViewTreePrivate
{
public:
    gnomonView *view;

public:
    QHash<QTreeWidgetItem *, gnomonActorVolume *> volume_items;
    QHash<QTreeWidgetItem *, gnomonActorImage *> image_items;
    QHash<QTreeWidgetItem *, gnomonActorMeshCellGraph *> cellgraph_items;
    QHash<QTreeWidgetItem *, gnomonActorScalarBar *> scalar_bar_items;

    QHash<QTreeWidgetItem *, gnomonInspectorVolume *> inspector_volume_items;
    QHash<QTreeWidgetItem *, gnomonInspectorCellGraph *> inspector_cellgraph_items;
    QHash<QTreeWidgetItem *, gnomonInspectorImage *> inspector_image_items;

    QHash<QTreeWidgetItem *, gnomonCellComplex *> complex_items;
    QHash<QTreeWidgetItem *, vtkPolyData *> mesh_items;

public:
    std::size_t next_image_id;
    std::size_t next_inspector_cellgraph_id;
    std::size_t next_inspector_image_id;
    std::size_t next_inspector_volume_id;
    std::size_t next_cellgraph_id;
    std::size_t next_complex_id;
    std::size_t next_mesh_id;
    std::size_t next_volume_id;
    std::size_t next_scalar_bar_id;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewTree::gnomonInspectorViewTree(QWidget *parent) : QTreeWidget(parent), d(new gnomonInspectorViewTreePrivate)
{
    d->view = nullptr;
    d->next_image_id = 0;
    d->next_inspector_cellgraph_id = 0;
    d->next_inspector_image_id = 0;
    d->next_inspector_volume_id = 0;
    d->next_cellgraph_id = 0;
    d->next_complex_id = 0;
    d->next_mesh_id = 0;
    d->next_volume_id = 0;
    d->next_scalar_bar_id = 0;

    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setCursor(Qt::ArrowCursor);
    this->setFrameShape(QFrame::NoFrame);
    this->setHeaderLabels(QStringList() << "Object" << "Type" << "");
    this->setColumnWidth(0, 180);
    this->setColumnWidth(1, 84);
    this->setColumnWidth(2, 16);

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelected()));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(onItemClicked(QTreeWidgetItem *, int)));
}

gnomonInspectorViewTree::~gnomonInspectorViewTree(void)
{
    delete d;
}

void gnomonInspectorViewTree::setView(gnomonView *view)
{
    d->view = view;
}

void gnomonInspectorViewTree::insert(vtkPolyData *mesh)
{
    if(!mesh) {
        qDebug() << Q_FUNC_INFO << "mesh is NULL";
        return;
    }

    if(d->mesh_items.values().contains(mesh)) {
        return;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Mesh " << QString::number(int(d->next_mesh_id)) << "Mesh");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

    d->mesh_items.insert(item, mesh);

    ++d->next_mesh_id;

    gnomonActor *actor = d->view->manager()->actor(mesh);

    if(actor != nullptr) {
        item->setCheckState(2, actor->isVisible() ? Qt::Checked : Qt::Unchecked);
    } else {
        item->setCheckState(2, Qt::Unchecked);
    }
}

QTreeWidgetItem *gnomonInspectorViewTree::insert(gnomonActorVolume *volume_actor)
{
    if(!volume_actor) {
        qDebug() << Q_FUNC_INFO << "volume_actor is NULL";
        return nullptr;
    }

    if(d->volume_items.values().contains(volume_actor))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Volume " + QString::number(d->next_volume_id) << "Volume");

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

    d->volume_items.insert(item, volume_actor);
    ++d->next_volume_id;

    item->setCheckState(2, volume_actor->isVisible() ? Qt::Checked : Qt::Unchecked);

    this->addTopLevelItem(item);

    return item;
}

QTreeWidgetItem *gnomonInspectorViewTree::addChild(QTreeWidgetItem *parent, gnomonInspectorVolume *inspector_volume)
{
    if(!parent) {
        return nullptr;
    }

    if(!inspector_volume) {
        return nullptr;
    }

    if(!d->volume_items.keys().contains(parent)) {
        return nullptr;
    }

    if(d->inspector_volume_items.values().contains(inspector_volume)) {
        return nullptr;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << "Inspector " + QString::number(d->next_inspector_volume_id) << "Inspector");

    parent->addChild(item);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    d->inspector_volume_items.insert(item, inspector_volume);

    ++d->next_inspector_volume_id;

    return item;
}

QTreeWidgetItem *gnomonInspectorViewTree::insert(gnomonActorImage *image_actor)
{
    if(!image_actor) {
        qDebug() << Q_FUNC_INFO << "image_actor is NULL";
        return nullptr;
    }

    if(d->image_items.values().contains(image_actor))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Image " + QString::number(d->next_image_id) << "Image");

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

    d->image_items.insert(item, image_actor);
    ++d->next_image_id;

    item->setCheckState(2, image_actor->isVisible() ? Qt::Checked : Qt::Unchecked);

    this->addTopLevelItem(item);

    return item;
}

QTreeWidgetItem *gnomonInspectorViewTree::addChild(QTreeWidgetItem *parent, gnomonInspectorImage *inspector_image)
{
    if(!parent)
        return nullptr;

    if(!inspector_image)
        return nullptr;

    if(!d->image_items.keys().contains(parent))
        return nullptr;

    if(d->inspector_image_items.values().contains(inspector_image))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << "Inspector " + QString::number(d->next_inspector_image_id) << "Inspector");

    parent->addChild(item);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    d->inspector_image_items.insert(item, inspector_image);

    ++d->next_inspector_image_id;

    return item;
}

QTreeWidgetItem *gnomonInspectorViewTree::insert(gnomonActorScalarBar *scalar_bar_actor)
{
    if(!scalar_bar_actor) {
        qDebug() << Q_FUNC_INFO << "scalar_bar_actor is NULL";
        return nullptr;
    }

    if(d->scalar_bar_items.values().contains(scalar_bar_actor))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Scalar bar " + QString::number(d->next_scalar_bar_id) << "Scalar bar");

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

    d->scalar_bar_items.insert(item, scalar_bar_actor);
    ++d->next_scalar_bar_id;

    item->setCheckState(2, scalar_bar_actor->isVisible() ? Qt::Checked : Qt::Unchecked);

    this->addTopLevelItem(item);

    return item;
}

QTreeWidgetItem *gnomonInspectorViewTree::insert(gnomonActorMeshCellGraph *cellgraph_actor)
{
    if(!cellgraph_actor) {
        qDebug() << Q_FUNC_INFO << "cellgraph_actor is NULL";
        return nullptr;
    }

    if(d->cellgraph_items.values().contains(cellgraph_actor))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "CellGraph " + QString::number(d->next_cellgraph_id) << "CellGraph");

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

    d->cellgraph_items.insert(item, cellgraph_actor);
    ++d->next_cellgraph_id;

    item->setCheckState(2, cellgraph_actor->isVisible() ? Qt::Checked : Qt::Unchecked);

    this->addTopLevelItem(item);

    return item;
}

QTreeWidgetItem *gnomonInspectorViewTree::addChild(QTreeWidgetItem *parent, gnomonInspectorCellGraph *inspector_cellgraph)
{
    if(!parent)
        return nullptr;

    if(!inspector_cellgraph)
        return nullptr;

    if(!d->cellgraph_items.keys().contains(parent))
        return nullptr;

    if(d->inspector_cellgraph_items.values().contains(inspector_cellgraph))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << "Inspector " + QString::number(d->next_inspector_cellgraph_id) << "Inspector");

    parent->addChild(item);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    d->inspector_cellgraph_items.insert(item, inspector_cellgraph);

    ++d->next_inspector_cellgraph_id;

    return item;
}

void gnomonInspectorViewTree::insert(gnomonCellComplex *complex)
{
    if(!complex) {
        qDebug() << Q_FUNC_INFO << "complex is NULL";
        return;
    }

    if(d->complex_items.values().contains(complex))
        return;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Complex " + QString::number(d->next_complex_id) << "Complex");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

    d->complex_items.insert(item, complex);
    ++d->next_complex_id;

    if(gnomonActor *actor = d->view->manager()->actor(complex)) {
        item->setCheckState(2, actor->isVisible() ? Qt::Checked : Qt::Unchecked);
    } else {
        item->setCheckState(2, Qt::Unchecked);
    }
}

void gnomonInspectorViewTree::onItemClicked(QTreeWidgetItem *item, int column)
{
    if(!d->view)
        return;

    if(column == 2) {
        gnomonActor *actor = nullptr;

        if(d->mesh_items.keys().contains(item))
            actor = d->view->manager()->actor(d->mesh_items.value(item));

        if(d->volume_items.keys().contains(item))
            actor = d->volume_items.value(item);

        if(d->image_items.keys().contains(item))
            actor = d->image_items.value(item);

        if(d->scalar_bar_items.keys().contains(item))
            actor = d->scalar_bar_items.value(item);

        if(d->complex_items.keys().contains(item))
            actor = d->view->manager()->actor(d->complex_items.value(item));

        if(d->cellgraph_items.keys().contains(item))
            actor = d->cellgraph_items.value(item);

        if(!actor)
            return;

        if (item->checkState(2) == Qt::Checked)
            actor->show();
        else
            actor->hide();

        emit checked(actor, (item->checkState(2) == Qt::Checked));

        d->view->update();
    }
}

void gnomonInspectorViewTree::onItemSelected(void)
{
    if(d->mesh_items.keys().contains(this->currentItem()))
        emit selected(d->mesh_items.value(this->currentItem()));

    if(d->volume_items.keys().contains(this->currentItem()))
        emit selected(d->volume_items.value(this->currentItem()));

    if(d->image_items.keys().contains(this->currentItem()))
        emit selected(d->volume_items.value(this->currentItem()));

    if(d->scalar_bar_items.keys().contains(this->currentItem()))
        emit selected(d->scalar_bar_items.value(this->currentItem()));

    if(d->complex_items.keys().contains(this->currentItem()))
        emit selected(d->complex_items.value(this->currentItem()));

    if(d->cellgraph_items.keys().contains(this->currentItem()))
        emit selected(d->cellgraph_items.value(this->currentItem()));

    if(d->inspector_image_items.keys().contains(this->currentItem()))
        emit selected(d->inspector_image_items.value(this->currentItem()));

    if(d->inspector_cellgraph_items.keys().contains(this->currentItem()))
        emit selected(d->inspector_cellgraph_items.value(this->currentItem()));

    if(d->inspector_volume_items.keys().contains(this->currentItem()))
        emit selected(d->inspector_volume_items.value(this->currentItem()));
}

//
// gnomonInspectorViewTree.cpp ends here
