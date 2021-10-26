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
#include <QtWidgets>


class gnomonPipelineNodePrivate
{
public:
    QString algorithm_class;
    QString algorithm;

public:
    QColor color;

/*public:
    QPointF offset;

public:
    QRectF rect;*/

public:
    QList<gnomonPipelinePort *>  input_ports;
    QList<gnomonPipelinePort *> output_ports;

    QList<gnomonPipelineEdge *>  input_edges;
    QList<gnomonPipelineEdge *> output_edges;

public:
    QString variantParameterString(const QVariant& parameter);
};



//
// gnomonPipelineNodePrivate.h ends here
