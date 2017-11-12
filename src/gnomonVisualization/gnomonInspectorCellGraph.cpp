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
#include "gnomonDoubleEditor.h"
#include "gnomonDoubleRangeEditor.h"
#include "gnomonStringEditor.h"

// /////////////////////////////////////////////////////////////////
// gnomonInspectorCellGraphPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorCellGraphPrivate
{
public:
    gnomonStringEditor *vertexPropertyEditor;

    gnomonDoubleEditor *vertexSizeEditor;
    gnomonDoubleEditor *edgeOpacityEditor;
    gnomonDoubleEditor *edgeLinewidthEditor;

    QMap<QString, gnomonDoubleRangeEditor *> sliceEditors;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorCellGraph
// /////////////////////////////////////////////////////////////////

gnomonInspectorCellGraph::gnomonInspectorCellGraph(QWidget *parent) : gnomonInspector(parent), d(new gnomonInspectorCellGraphPrivate)
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
    layout->setSpacing(0);
    layout->addWidget(d->vertexPropertyEditor);
    layout->addWidget(d->vertexSizeEditor);
    layout->addWidget(d->edgeOpacityEditor);
    layout->addWidget(d->edgeLinewidthEditor);

    QMap<QString, void(gnomonInspectorCellGraph::*)(void)> sliceSignals;
    sliceSignals["x"] = &gnomonInspectorCellGraph::xSliceUpdated;
    sliceSignals["y"] = &gnomonInspectorCellGraph::ySliceUpdated;
    sliceSignals["z"] = &gnomonInspectorCellGraph::zSliceUpdated;

    for (const auto& dim : sliceSignals.keys()) {
        d->sliceEditors[dim] = new gnomonDoubleRangeEditor();
        QString sliceName(dim);
        sliceName.append(" Slice");
        d->sliceEditors[dim]->setName(sliceName);
        connect(d->sliceEditors[dim], &gnomonDoubleRangeEditor::valueMinChanged, this, sliceSignals[dim]);
        connect(d->sliceEditors[dim], &gnomonDoubleRangeEditor::valueMaxChanged, this, sliceSignals[dim]);
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
