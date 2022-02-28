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
    QString algorithm_class;
    QString algorithm;

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
    QList<gnomonPipelineEdge *> output_edges;

public:
    QString variantParameterString(const QVariant& parameter);
};



//
// gnomonPipelineNodePrivate.h ends here
