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

#pragma once

#include <QtCore>
#include <QtGui>


class gnomonPipelineNodePrivate
{
public:
    QString name;
    QString description;
    QString version;
    QString algorithm_class;
    QString algorithm;

public:
    gnomonPipelineNode::Type type = gnomonPipelineNode::NODE_DEFAULT;

public:
    QColor color;
    QPointF position;

/*public:
    QPointF offset;

public:
    QRectF rect;*/

public:
    QMap<QString, gnomonPipelinePort *> input_ports;
    QMap<QString, gnomonPipelinePort *> output_ports;

    QList<gnomonPipelineEdge *>  input_edges;
    QMap<QString, gnomonPipelineEdge *> input_edges_map;
    QList<gnomonPipelineEdge *> output_edges;

public:
    QString variantParameterString(const QVariant& parameter);
};



//
// gnomonPipelineNodePrivate.h ends here
