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

#include "gnomonActorMeshCellGraph.h"
#include "gnomonDoubleEditor.h"
#include "gnomonDoubleRangeEditor.h"
#include "gnomonInspectorCellGraph.h"
#include "gnomonStringEditor.h"

#include <gnomonCore>

// /////////////////////////////////////////////////////////////////
// gnomonInspectorCellGraphPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorCellGraphPrivate
{
public:
    gnomonDoubleEditor *vertexSizeEditor;
    gnomonDoubleEditor *edgeOpacityEditor;
    gnomonDoubleEditor *edgeLinewidthEditor;
    gnomonStringEditor *vertexPropertyEditor;

public:
    QMap<QString, gnomonDoubleRangeEditor *> sliceEditors;

public:
    QMap<QString, void(gnomonInspectorCellGraph::*)(void)> sliceSignals;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorCellGraph
// /////////////////////////////////////////////////////////////////

gnomonInspectorCellGraph::gnomonInspectorCellGraph(QWidget *parent) : QFrame(parent), d(new gnomonInspectorCellGraphPrivate)
{
    d->vertexPropertyEditor = new gnomonStringEditor(this);
    d->vertexPropertyEditor->setName("Property Name");

    connect(d->vertexPropertyEditor, &gnomonStringEditor::valueChanged, this, &gnomonInspectorCellGraph::vertexPropertyUpdated);

    d->vertexSizeEditor = new gnomonDoubleEditor(this);
    d->vertexSizeEditor->setName("Vertex Size");
    d->vertexSizeEditor->setRange(0,10);
    d->vertexSizeEditor->setValue(1);

    connect(d->vertexSizeEditor, &gnomonDoubleEditor::valueChanged, this, &gnomonInspectorCellGraph::vertexSizeUpdated);

    d->edgeOpacityEditor = new gnomonDoubleEditor(this);
    d->edgeOpacityEditor->setName("Edge Opacity");
    d->edgeOpacityEditor->setRange(0,1);
    d->edgeOpacityEditor->setValue(0.5);

    connect(d->edgeOpacityEditor, &gnomonDoubleEditor::valueChanged, this, &gnomonInspectorCellGraph::edgeOpacityUpdated);

    d->edgeLinewidthEditor = new gnomonDoubleEditor(this);
    d->edgeLinewidthEditor->setName("Edge Linewidth");
    d->edgeLinewidthEditor->setRange(0,10);
    d->edgeLinewidthEditor->setValue(2);

    connect(d->edgeLinewidthEditor, &gnomonDoubleEditor::valueChanged, this, &gnomonInspectorCellGraph::edgeLinewidthUpdated);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->vertexPropertyEditor);
    layout->addWidget(d->vertexSizeEditor);
    layout->addWidget(d->edgeOpacityEditor);
    layout->addWidget(d->edgeLinewidthEditor);

    d->sliceSignals["x"] = &gnomonInspectorCellGraph::xSliceUpdated;
    d->sliceSignals["y"] = &gnomonInspectorCellGraph::ySliceUpdated;
    d->sliceSignals["z"] = &gnomonInspectorCellGraph::zSliceUpdated;

    for (const auto& dim : d->sliceSignals.keys()) {
        d->sliceEditors[dim] = new gnomonDoubleRangeEditor(this);
        QString sliceName(dim);
        sliceName.append(" Slice");
        d->sliceEditors[dim]->setName(sliceName);
        connect(d->sliceEditors[dim], &gnomonDoubleRangeEditor::valueMinChanged, this, d->sliceSignals[dim]);
        connect(d->sliceEditors[dim], &gnomonDoubleRangeEditor::valueMaxChanged, this, d->sliceSignals[dim]);
        layout->addWidget(d->sliceEditors[dim]);
    }

    this->setLayout(layout);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorCellGraph::~gnomonInspectorCellGraph(void)
{
    for (const auto& dim : d->sliceEditors.keys())
        delete d->sliceEditors[dim];

    delete d;

    d = NULL;
}

void gnomonInspectorCellGraph::setActor(gnomonActorMeshCellGraph *actor) const
{
    QStringList vertexProperties = actor->cellGraph()->vertexPropertyNames();
    vertexProperties.insert(0,"");

    d->vertexPropertyEditor->setList(vertexProperties);

    connect(this, &gnomonInspectorCellGraph::vertexPropertyUpdated, [=] () { actor->setVertexProperty(this->vertexProperty()); });
    connect(this, &gnomonInspectorCellGraph::vertexSizeUpdated, [=] () { actor->setVertexSize(this->vertexSize()); });
    connect(this, &gnomonInspectorCellGraph::edgeOpacityUpdated, [=] () { actor->setEdgeOpacity(this->edgeOpacity()); });
    connect(this, &gnomonInspectorCellGraph::edgeLinewidthUpdated, [=] () { actor->setEdgeLinewidth(this->edgeLinewidth()); });

    for (const auto& dim : d->sliceSignals.keys()) {
        QString barycenterProp("barycenter_");
        barycenterProp.append(dim);
        QMap<long, QVariant> positions = actor->cellGraph()->vertexProperty(barycenterProp);
        QList<double> points;
        for (const auto& vertexId : actor->cellGraph()->vertexIds()) {
            points<<positions[vertexId].value<double>();
        }
        double pointMin = floor(*std::min_element(points.begin(),points.end()));
        double pointMax = ceil(*std::max_element(points.begin(),points.end()));
        this->sliceEditor(dim)->setRange(pointMin,pointMax);
        this->sliceEditor(dim)->setValueMin(pointMin);
        this->sliceEditor(dim)->setValueMax(pointMax);
        connect(this, d->sliceSignals[dim], [=] () {
            actor->setSlice(dim, this->slice(dim));
        });
    }
}

gnomonStringEditor *gnomonInspectorCellGraph::vertexPropertyEditor(void) const
{
    return d->vertexPropertyEditor;
}

gnomonDoubleRangeEditor *gnomonInspectorCellGraph::sliceEditor(const QString& dim) const
{
    return d->sliceEditors[dim];
}

const QString& gnomonInspectorCellGraph::vertexProperty(void) const
{
    return d->vertexPropertyEditor->value();
}

double gnomonInspectorCellGraph::vertexSize(void) const
{
    return d->vertexSizeEditor->value();
}

double gnomonInspectorCellGraph::edgeOpacity(void) const
{
    return d->edgeOpacityEditor->value();
}

double gnomonInspectorCellGraph::edgeLinewidth(void) const
{
    return d->edgeLinewidthEditor->value();
}

const QList<double>& gnomonInspectorCellGraph::slice(const QString& dim) const
{
    return d->sliceEditors[dim]->value();
}

//
// gnomonInspectorCellGraph.cpp ends here
