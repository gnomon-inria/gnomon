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

#include "gnomonActorMeshCellComplex.h"
#include "gnomonActorMeshCellGraph.h"
#include "gnomonActorMeshCellImage.h"
#include "gnomonActorVolume.h"

#include <QtCore>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewTreePrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorViewTreePrivate
{
public:
    QHash<QTreeWidgetItem *, gnomonActorMeshCellComplex *> cellcomplex_items;
    QHash<QTreeWidgetItem *, gnomonActorMeshCellGraph *> cellgraph_items;
    QHash<QTreeWidgetItem *, gnomonActorMeshCellImage *> cellimage_items;
    QHash<QTreeWidgetItem *, gnomonActorVolume *> volume_items;

public:
    std::size_t next_cellcomplex_id;
    std::size_t next_cellgraph_id;
    std::size_t next_cellimage_id;
    std::size_t next_volume_id;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewTree::gnomonInspectorViewTree(QWidget *parent) : QTreeWidget(parent), d(new gnomonInspectorViewTreePrivate)
{
    d->next_cellcomplex_id = 0;
    d->next_cellgraph_id = 0;
    d->next_cellimage_id = 0;
    d->next_volume_id = 0;

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

QTreeWidgetItem *gnomonInspectorViewTree::insert(gnomonActorMeshCellComplex *actor)
{
    if(!actor) {
        qDebug() << Q_FUNC_INFO << "actor is NULL";
        return nullptr;
    }

    if(d->cellcomplex_items.values().contains(actor))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "MeshCellComplex " + QString::number(d->next_cellcomplex_id) << "MeshCellComplex");

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

    d->cellcomplex_items.insert(item, actor);
    ++d->next_cellcomplex_id;

    item->setCheckState(2, actor->isVisible() ? Qt::Checked : Qt::Unchecked);

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

QTreeWidgetItem *gnomonInspectorViewTree::insert(gnomonActorMeshCellImage *actor)
{
    if(!actor) {
        qDebug() << Q_FUNC_INFO << "actor is NULL";
        return nullptr;
    }

    if(d->cellimage_items.values().contains(actor))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "CellImage " + QString::number(d->next_cellimage_id) << "CellImage");

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

    d->cellimage_items.insert(item, actor);
    ++d->next_cellimage_id;

    item->setCheckState(2, actor->isVisible() ? Qt::Checked : Qt::Unchecked);

    this->addTopLevelItem(item);

    return item;
}

QTreeWidgetItem *gnomonInspectorViewTree::insert(gnomonActorVolume *actor)
{
    if(!actor) {
        qDebug() << Q_FUNC_INFO << "actor is NULL";
        return nullptr;
    }

    if(d->volume_items.values().contains(actor))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Volume " + QString::number(d->next_volume_id) << "Volume");

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

    d->volume_items.insert(item, actor);
    ++d->next_volume_id;

    item->setCheckState(2, actor->isVisible() ? Qt::Checked : Qt::Unchecked);

    this->addTopLevelItem(item);

    return item;
}

void gnomonInspectorViewTree::onItemClicked(QTreeWidgetItem *item, int column)
{
    if(column == 2) {
        gnomonActor *actor = nullptr;

        if(d->volume_items.keys().contains(item))
            actor = d->volume_items.value(item);

        if(d->cellcomplex_items.keys().contains(item))
            actor = d->cellcomplex_items.value(item);

        if(d->cellgraph_items.keys().contains(item))
            actor = d->cellgraph_items.value(item);

        if(d->cellimage_items.keys().contains(item))
            actor = d->cellimage_items.value(item);

        if(!actor)
            return;

        if (item->checkState(2) == Qt::Checked)
            actor->show();
        else
            actor->hide();

        emit checked(actor, (item->checkState(2) == Qt::Checked));
    }
}

void gnomonInspectorViewTree::onItemSelected(void)
{
    if(d->cellcomplex_items.keys().contains(this->currentItem()))
        emit selected(d->cellcomplex_items.value(this->currentItem()));

    if(d->cellgraph_items.keys().contains(this->currentItem()))
        emit selected(d->cellgraph_items.value(this->currentItem()));

    if(d->cellimage_items.keys().contains(this->currentItem()))
        emit selected(d->cellimage_items.value(this->currentItem()));

    if(d->volume_items.keys().contains(this->currentItem()))
        emit selected(d->volume_items.value(this->currentItem()));
}

//
// gnomonInspectorViewTree.cpp ends here
