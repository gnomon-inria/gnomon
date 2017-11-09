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

#include "gnomonView"
#include "gnomonActor"
#include "gnomonViewManager"

#include <QtCore>
#include <QtWidgets>

#include <vtkPolyData.h>
#include <vtkImageData.h>

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewTreePrivate
// ///////////////////////////////////////////////////////////////////
class gnomonInspectorViewTreePrivate {

public:
    gnomonView *view;

    QHash<QTreeWidgetItem *, vtkPolyData *> mesh_items;
    QHash<QTreeWidgetItem *, vtkImageData *> volume_items;
    QHash<QTreeWidgetItem *, gnomonCellComplex *> complex_items;
    QHash<QTreeWidgetItem *, gnomonCellGraph *> graph_items;

    std::size_t next_mesh_id;
    std::size_t next_volume_id;
    std::size_t next_complex_id;
    std::size_t next_graph_id;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewTree::gnomonInspectorViewTree(QWidget *parent) : QTreeWidget(parent), d(new gnomonInspectorViewTreePrivate)
{
    d->view = nullptr;
    d->next_volume_id = 0;
    d->next_mesh_id = 0;
    d->next_graph_id = 0;
    d->next_complex_id = 0;

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

gnomonInspectorViewTree::~gnomonInspectorViewTree() {
    // ///////////////////////////////////////////////////////////////////
    // Cleans the tree by deleting the tree items, not the data
    // ///////////////////////////////////////////////////////////////////
    for(auto item : d->mesh_items.keys()) {
        delete item;
    }
    for(auto item : d->volume_items.keys()) {
        delete item;
    }
    for(auto item : d->complex_items.keys()) {
        delete item;
    }
    for(auto item : d->graph_items.keys()) {
        delete item;
    }
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

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Mesh " + d->next_mesh_id << "Mesh");
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

void gnomonInspectorViewTree::insert(vtkImageData *volume)
{
    if(!volume) {
        qDebug() << Q_FUNC_INFO << "volume is NULL";
        return;
    }

    if(d->volume_items.values().contains(volume))
        return;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Volume " + QString::number(d->next_volume_id) << "Volume");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

    d->volume_items.insert(item, volume);
    ++d->next_volume_id;

    if(gnomonActor *actor = d->view->manager()->actor(volume)) {
        item->setCheckState(2, actor->isVisible() ? Qt::Checked : Qt::Unchecked);
    } else {
        item->setCheckState(2, Qt::Unchecked);
    }
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

void gnomonInspectorViewTree::insert(gnomonCellGraph *graph)
{
    if(!graph) {
        qDebug() << Q_FUNC_INFO << "graph is NULL";
        return;
    }

    if(d->graph_items.values().contains(graph))
        return;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "Graph " + QString::number(d->next_graph_id) << "Graph");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

    d->graph_items.insert(item, graph);
    ++d->next_graph_id;

    if(gnomonActor *actor = d->view->manager()->actor(graph)) {
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

        if(d->mesh_items.keys().contains(item)) {
            actor = d->view->manager()->actor(d->mesh_items.value(item));
        }

        if(d->volume_items.keys().contains(item)) {
            actor = d->view->manager()->actor(d->volume_items.value(item));
        }

        if(d->complex_items.keys().contains(item)) {
            actor = d->view->manager()->actor(d->complex_items.value(item));
        }

        if(d->graph_items.keys().contains(item)) {
            actor = d->view->manager()->actor(d->graph_items.value(item));
        }

        if(!actor)
            return;

        if (item->checkState(2) == Qt::Checked)
            actor->show();
        else {
            actor->hide();
        }

        emit checked(actor, (item->checkState(2) == Qt::Checked));

        d->view->update();
    }
}

void gnomonInspectorViewTree::onItemSelected(void)
{
    if(d->mesh_items.keys().contains(this->currentItem())) {
        emit selected(d->mesh_items.value(this->currentItem()));
    }
    if(d->volume_items.keys().contains(this->currentItem())) {
        emit selected(d->volume_items.value(this->currentItem()));
    }
    if(d->complex_items.keys().contains(this->currentItem())) {
        emit selected(d->complex_items.value(this->currentItem()));
    }
    if(d->graph_items.keys().contains(this->currentItem())) {
        emit selected(d->graph_items.value(this->currentItem()));
    }
}

//
// gnomonInspectorViewTree.cpp ends here
