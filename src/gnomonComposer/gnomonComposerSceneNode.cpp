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

#include "gnomonComposerSceneNode.h"
#include "gnomonComposerSceneNode_p.h"

// /////////////////////////////////////////////////////////////////
// gnomonComposerSceneNode
// /////////////////////////////////////////////////////////////////

gnomonComposerSceneNode::gnomonComposerSceneNode(void) : dtkComposerSceneNode(), d(new gnomonComposerSceneNodePrivate)
{
    d->color = QColor(Qt::darkGray);

    d->rect = QRectF(0, 0, 150, 50);
    d->offset = QPointF(50, 50);

    this->setAcceptDrops(false);
    this->layout();
    this->setTitle("");
}

gnomonComposerSceneNode::~gnomonComposerSceneNode(void)
{
    delete d;

    d = NULL;
}

void gnomonComposerSceneNode::resetPos(const QPointF& pos, const QRectF& rect)
{
    QPointF center(0, 0);
    QRectF box(0, 0, 0, 0);

    if (!pos.isNull()) {
        center = pos + QPointF(0.5 * rect.width(), 0.5 * rect.height());
    }
}

void gnomonComposerSceneNode::layout(void)
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

void gnomonComposerSceneNode::resize(const QSizeF& size)
{
    d->rect = QRectF(d->rect.topLeft(), size);
}

void gnomonComposerSceneNode::resize(qreal width, qreal height)
{
    d->rect = QRectF(d->rect.topLeft(), QSizeF(width, height));
}

QRectF gnomonComposerSceneNode::boundingRect(void) const
{
    return d->rect.adjusted(-2, -2, 2, 2);
}

void gnomonComposerSceneNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void gnomonComposerSceneNode::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->ignore();
}

void gnomonComposerSceneNode::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->ignore();
}

void gnomonComposerSceneNode::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->ignore();
}

void gnomonComposerSceneNode::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->ignore();
}

//
// gnomonComposerSceneNode.cpp ends here
