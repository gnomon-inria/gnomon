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

#include <float.h>

#include "gnomonPipelineNode.h"
#include "gnomonPipelineNode_p.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNode
// /////////////////////////////////////////////////////////////////

gnomonPipelineNode::gnomonPipelineNode(void) : dtkComposerSceneNode(), d(new gnomonPipelineNodePrivate)
{
    d->color = QColor(Qt::darkGray);

    d->rect = QRectF(0, 0, 150, 50);
    d->offset = QPointF(50, 50);

    this->setAcceptDrops(false);
    this->layout();
    this->setTitle("");
}

gnomonPipelineNode::~gnomonPipelineNode(void)
{
    delete d;

    d = NULL;
}

void gnomonPipelineNode::layout(void)
{
    d->rect = QRectF(0, 0, 150, 50);

    // /////////////////////////////////////////////////////////////////
    // Port location
    // /////////////////////////////////////////////////////////////////

    int header = 15;

    int port_margin_top = 10;
    int port_margin_bottom = 10;
    int port_margin_left = 10;
    int port_spacing = 10;

    for (int i = 0; i < this->inputPorts().count(); i++)
        this->inputPorts().at(i)->setPos(QPointF(port_margin_left, i * this->inputPorts().at(i)->boundingRect().height() + i * port_spacing + port_margin_top + header));

    for (int i = 0; i < this->outputPorts().count(); i++)
        this->outputPorts().at(i)->setPos(QPointF(d->rect.right() - port_margin_left - this->outputPorts().at(i)->boundingRect().width(), i * this->outputPorts().at(i)->boundingRect().height() + i * port_spacing + port_margin_top + header));


    // /////////////////////////////////////////////////////////////////
    // Height calculation
    // /////////////////////////////////////////////////////////////////

    if (this->inputPorts().count() || this->outputPorts().count()) {
        if (this->inputPorts().count() >= this->outputPorts().count())
            d->rect = QRectF(d->rect.topLeft(), QSize(d->rect.width(), this->inputPorts().count() * this->inputPorts().at(0)->boundingRect().height() + port_margin_top + port_margin_bottom + (this->inputPorts().count() - 1) * port_spacing + header));
        else
            d->rect = QRectF(d->rect.topLeft(), QSize(d->rect.width(), this->outputPorts().count() * this->outputPorts().at(0)->boundingRect().height() + port_margin_top + port_margin_bottom + (this->outputPorts().count() - 1) * port_spacing + header));
    }

    // /////////////////////////////////////////////////////////////////
    // Update edges geometry
    // /////////////////////////////////////////////////////////////////

    QRectF updateRect;

    for (dtkComposerSceneEdge *edge : this->inputEdges()) {
        edge->adjust();
        updateRect |= edge->boundingRect();
    }

    for (dtkComposerSceneEdge *edge : this->outputEdges()) {
        edge->adjust();
        updateRect |= edge->boundingRect();
    }

    this->update(updateRect);

}

void gnomonPipelineNode::resize(qreal width, qreal height)
{
    d->rect = QRectF(d->rect.topLeft(), QSizeF(width, height));
}

QRectF gnomonPipelineNode::boundingRect(void) const
{
    return d->rect.adjusted(-2, -2, 2, 2);
}

void gnomonPipelineNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    qreal radius = 5.0;

    if (this->isSelected()) {
        painter->setPen(QPen(Qt::magenta, 3, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(d->rect.adjusted(-1, -1, 1, 1), radius, radius);
    }

    QLinearGradient gradient(d->rect.left(), d->rect.top(), d->rect.left(), d->rect.bottom());
    gradient.setColorAt(0.0, d->color);
    gradient.setColorAt(10 / d->rect.height(),d->color.darker().darker());
    gradient.setColorAt(1.0, d->color.darker().darker().darker());

    painter->setPen(QPen(d->color.darker().darker(), 1, Qt::SolidLine));
    painter->setBrush(gradient);

    painter->drawRoundedRect(d->rect, radius, radius);

    // Drawing node's title

    qreal margin = 5.0;

    QFont font = painter->font();
    QFontMetricsF metrics(font);

    QString title_text = metrics.elidedText(this->title(), Qt::ElideMiddle, this->boundingRect().width() - 2 - 4 * margin);
    QPointF title_pos;

    title_pos = QPointF(2 * margin, 2 * margin + metrics.xHeight());
    painter->setPen(QPen(QColor(Qt::white)));
    painter->drawText(title_pos, title_text);
}


QString gnomonPipelineNode::toToml(const QString& node_name)
{
    QString node_string = "[" + node_name + "]\n" + "\n";
    return node_string;
}

QString gnomonPipelineNode::variantParameterString(const QVariant& parameter)
{
    QString parameter_string = "";

    bool int_status;
    parameter.toInt(&int_status);
    bool double_status;
    parameter.toDouble(&double_status);

    if (int_status | double_status) {
         parameter_string = parameter.toString();
    } else if (parameter.canConvert<QString>()) {
         parameter_string = "\"" + parameter.toString() + "\"";
    } else if (parameter.canConvert<QStringList>()) {
        QStringList list = parameter.toStringList();
        parameter_string = "[";
        for (int i=0; i<list.size(); i++) {
            if (i>0)
                parameter_string += ", ";
            parameter_string += "\"" + list[i] + "\"";
        }
        parameter_string += "]";
    } else if (parameter.canConvert<QList<QVariant>>()) {
        QList<QVariant> list = parameter.toList();
        parameter_string = "[";
        for (int i=0; i<list.size(); i++) {
            if (i>0)
                parameter_string += ", ";
            parameter_string += list[i].toString();
        }
        parameter_string += "]";
    }

    return parameter_string;
}

//
// gnomonPipelineNode.cpp ends here
