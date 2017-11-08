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

#include <QtCore>
#include <QtWidgets>

#include <vtkPolyData.h>
#include <vtkImageData.h>

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewTreePrivate
// ///////////////////////////////////////////////////////////////////
class gnomonInspectorViewTreePrivate {

public:
    QHash<QTreeWidgetItem *, vtkPolyData *> mesh_items;
    QHash<QTreeWidgetItem *, vtkImageData *> volume_items;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewTree::gnomonInspectorViewTree(QWidget *parent) : QTreeWidget(parent), d(new gnomonInspectorViewTreePrivate)
{
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

void gnomonInspectorViewTree::insert(vtkPolyData *mesh)
{
    if(!mesh) {
        qDebug() << Q_FUNC_INFO << "mesh is NULL";
        return;
    }

    if(d->mesh_items.values().contains(mesh)) {
        return;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "toto" << "Mesh");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

    // if(numActor *actor = d->view->manager()->actor(mesh))
    //     item->setCheckState(2, actor->isVisible() ? Qt::Checked : Qt::Unchecked);
    // else
    //     item->setCheckState(2, Qt::Unchecked);

    d->mesh_items.insert(item, mesh);

    // foreach(numField *field, d->pool)
    //     this->insert(field);

    // d->pool.clear();
}

void gnomonInspectorViewTree::insert(vtkImageData *volume)
{
    if(!volume) {
        qDebug() << Q_FUNC_INFO << "volume is NULL";
        return;
    }

    if(d->volume_items.values().contains(volume))
        return;

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << "titi" << "Volume");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

    d->volume_items.insert(item, volume);
}


gnomonInspectorViewTree::~gnomonInspectorViewTree(void)
{

}

//
// gnomonInspectorViewTree.cpp ends here
