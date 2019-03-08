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

#include "gnomonInspectorCellGraph.h"
#include "gnomonActor/gnomonActorLegacy/gnomonActorMeshCellGraph.h"

#include <gnomonCore>
#include <gnomonWidgets>

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
    gnomonDoubleRangeEditor *vertexPropertyRangeEditor;

    gnomonStringEditor *filterPropertyEditor;
    gnomonDoubleRangeEditor *filterPropertyRangeEditor;

    gnomonColorMapEditor *colorEditor;

public:
    QMap<QString, gnomonDoubleRangeEditor *> sliceEditors;

};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorCellGraph
// /////////////////////////////////////////////////////////////////

gnomonInspectorCellGraph::gnomonInspectorCellGraph(QWidget *parent) : QFrame(parent), d(new gnomonInspectorCellGraphPrivate)
{
    d->vertexPropertyEditor = new gnomonStringEditor(this);
    d->vertexPropertyEditor->setName("Property Name");

    d->colorEditor = new gnomonColorMapEditor(this);
    // d->colorEditor->setName("Color Map");

    d->vertexPropertyRangeEditor = new gnomonDoubleRangeEditor(this);
    d->vertexPropertyRangeEditor->setName("Property Range");

    d->vertexSizeEditor = new gnomonDoubleEditor(this);
    d->vertexSizeEditor->setName("Vertex Size");
    d->vertexSizeEditor->setRange(0,10);
    d->vertexSizeEditor->setValue(1);

    d->edgeOpacityEditor = new gnomonDoubleEditor(this);
    d->edgeOpacityEditor->setName("Edge Opacity");
    d->edgeOpacityEditor->setRange(0,1);
    d->edgeOpacityEditor->setValue(0.25);

    d->edgeLinewidthEditor = new gnomonDoubleEditor(this);
    d->edgeLinewidthEditor->setName("Edge Linewidth");
    d->edgeLinewidthEditor->setRange(0,10);
    d->edgeLinewidthEditor->setValue(2);

    d->filterPropertyEditor = new gnomonStringEditor(this);
    d->filterPropertyEditor->setName("Filter Property Name");

    d->filterPropertyRangeEditor = new gnomonDoubleRangeEditor(this);
    d->filterPropertyRangeEditor->setName("Filter Property Range");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->vertexPropertyEditor);
    layout->addWidget(d->colorEditor);
    layout->addWidget(d->vertexPropertyRangeEditor);
    layout->addWidget(d->vertexSizeEditor);
    layout->addWidget(d->edgeOpacityEditor);
    layout->addWidget(d->edgeLinewidthEditor);

    for (const auto& dim : QList<QString>{"x","y","z"}) {
        d->sliceEditors[dim] = new gnomonDoubleRangeEditor(this);
        QString sliceName(dim);
        sliceName.append(" Slice");
        d->sliceEditors[dim]->setName(sliceName);
        layout->addWidget(d->sliceEditors[dim]);
    }
    
    layout->addWidget(d->filterPropertyEditor);
    layout->addWidget(d->filterPropertyRangeEditor);

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
    connect(d->vertexPropertyEditor, &gnomonStringEditor::valueChanged, [=] () { actor->setVertexProperty(d->vertexPropertyEditor->value()); });
    connect(d->vertexPropertyEditor, &gnomonStringEditor::valueChanged, [=] () { this->updateVertexPropertyRange(actor); });

    d->colorEditor->setValue(actor->colormap());    
    connect(d->colorEditor, &gnomonColorMapEditor::valueChanged, [=] () { actor->setColorMap(d->colorEditor->value()); });

    connect(d->vertexPropertyRangeEditor, &gnomonDoubleRangeEditor::valueMinChanged, [=] () { actor->setVertexPropertyRange(d->vertexPropertyRangeEditor->value()); });
    connect(d->vertexPropertyRangeEditor, &gnomonDoubleRangeEditor::valueMaxChanged, [=] () { actor->setVertexPropertyRange(d->vertexPropertyRangeEditor->value()); });

    this->updateVertexPropertyRange(actor);

    connect(d->vertexSizeEditor, &gnomonDoubleEditor::valueChanged, [=] () { actor->setVertexSize(d->vertexSizeEditor->value()); });
    connect(d->edgeOpacityEditor, &gnomonDoubleEditor::valueChanged, [=] () { actor->setEdgeOpacity(d->edgeOpacityEditor->value()); });
    connect(d->edgeLinewidthEditor, &gnomonDoubleEditor::valueChanged, [=] () { actor->setEdgeLinewidth(d->edgeLinewidthEditor->value()); });

    for (const auto& dim : d->sliceEditors.keys()) {
        QString barycenterProp("barycenter_");
        barycenterProp.append(dim);
        QMap<long, QVariant> positions = actor->cellGraph()->vertexProperty(barycenterProp);
        QList<double> points;
        for (const auto& vertexId : actor->cellGraph()->vertexIds()) {
            points<<positions[vertexId].value<double>();
        }
        double pointMin = floor(*std::min_element(points.begin(),points.end()));
        double pointMax = ceil(*std::max_element(points.begin(),points.end()));
        d->sliceEditors[dim]->setRange(pointMin,pointMax);
        d->sliceEditors[dim]->setValueMin(pointMin);
        d->sliceEditors[dim]->setValueMax(pointMax);
        connect(d->sliceEditors[dim], &gnomonDoubleRangeEditor::valueMinChanged, [=] () { actor->setSlice(dim, d->sliceEditors[dim]->value()); });
        connect(d->sliceEditors[dim], &gnomonDoubleRangeEditor::valueMaxChanged, [=] () { actor->setSlice(dim, d->sliceEditors[dim]->value()); });
    }

    d->filterPropertyEditor->setList(vertexProperties);
    connect(d->filterPropertyEditor, &gnomonStringEditor::valueChanged, [=] () { actor->setFilterProperty(d->filterPropertyEditor->value()); });
    connect(d->filterPropertyEditor, &gnomonStringEditor::valueChanged, [=] () { this->updateFilterPropertyRange(actor); });

    connect(d->filterPropertyRangeEditor, &gnomonDoubleRangeEditor::valueMinChanged, [=] () { actor->setFilterPropertyRange(d->filterPropertyRangeEditor->value()); });
    connect(d->filterPropertyRangeEditor, &gnomonDoubleRangeEditor::valueMaxChanged, [=] () { actor->setFilterPropertyRange(d->filterPropertyRangeEditor->value()); });

    this->updateFilterPropertyRange(actor);
}

void gnomonInspectorCellGraph::updateVertexPropertyRange(gnomonActorMeshCellGraph *actor) const
{
    d->vertexPropertyRangeEditor->setRange(actor->rangeMin(),actor->rangeMax());
    d->vertexPropertyRangeEditor->setValueMin(actor->rangeMin());
    d->vertexPropertyRangeEditor->setValueMax(actor->rangeMax());
}

void gnomonInspectorCellGraph::updateFilterPropertyRange(gnomonActorMeshCellGraph *actor) const
{
    d->filterPropertyRangeEditor->setRange(actor->filterRangeMin(),actor->filterRangeMax());
    d->filterPropertyRangeEditor->setValueMin(actor->filterRangeMin());
    d->filterPropertyRangeEditor->setValueMax(actor->filterRangeMax());
}

//
// gnomonInspectorCellGraph.cpp ends here
