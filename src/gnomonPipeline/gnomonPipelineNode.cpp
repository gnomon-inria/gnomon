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

#include "gnomonPipelinePort.h"
#include "gnomonPipelineEdge.h"

#include "gnomonPipelineNode.h"
#include "gnomonPipelineNode_p.h"

#include <dtkCoreParameter>

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodePrivate
// /////////////////////////////////////////////////////////////////

QString gnomonPipelineNodePrivate::variantParameterString(const QVariant& parameter)
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

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNode
// /////////////////////////////////////////////////////////////////

gnomonPipelineNode::gnomonPipelineNode(void) : d(new gnomonPipelineNodePrivate)
{
    /*d->color = QColor(Qt::darkGray);

    d->rect = QRectF(0, 0, 200, 50);
    d->offset = QPointF(50, 50);

    this->setAcceptDrops(false);
    // this->layout()();
    this->setTitle("");*/
}

gnomonPipelineNode::gnomonPipelineNode(Type t) : d(new gnomonPipelineNodePrivate)
{
    d->type = t;
}

gnomonPipelineNode::~gnomonPipelineNode(void)
{
    delete d;
    d = nullptr;
}

const QString& gnomonPipelineNode::name(void)
{
    return d->name;
}

void gnomonPipelineNode::setName(const QString& node_name)
{
    if (node_name != d->name) {
        d->name = node_name;
        emit nameChanged();
    }
}

const QString& gnomonPipelineNode::description(void)
{
    return d->description;
}

void gnomonPipelineNode::setDescription(const QString& desc)
{
    if (desc != d->description) {
        d->description = desc;
        emit descriptionChanged();
    }
}

const QString& gnomonPipelineNode::version(void)
{
    return d->version;
}

void gnomonPipelineNode::setVersion(const QString& desc)
{
    if (desc != d->version) {
        d->version = desc;
        emit versionChanged();
    }
}

const QString& gnomonPipelineNode::algorithmClass(void)
{
    return d->algorithm_class;
}

const QString& gnomonPipelineNode::algorithmPlugin(void)
{
    return d->algorithm;
}

const QColor& gnomonPipelineNode::color(void)
{
    return d->color;
}

gnomonPipelineNode::Type gnomonPipelineNode::type(void)
{
    return d->type;
}


QJsonObject gnomonPipelineNode::parameters(void)
{
    QJsonObject param;
    return param;
}
QJsonObject gnomonPipelineNode::morphonet_data(void)
{
    QJsonObject m_data;
    return m_data;
}

QStringList gnomonPipelineNode::parametersName(void) {
    return {};
}

void gnomonPipelineNode::configureParameter(const QString &name, dtkCoreParameter *param) {

}

const QPointF& gnomonPipelineNode::position(void)
{
    return d->position;
}

void gnomonPipelineNode::setPosition(const QPointF& pos)
{
    d->position = pos;
    emit positionChanged();
}


/* void gnomonPipelineNode::layout(void)
{
    d->rect = QRectF(0, 0, 200, 50);

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

    for (gnomonPipelineEdge *edge : this->inputEdges()) {
        edge->adjust();
        updateRect |= edge->boundingRect();
    }

    for (gnomonPipelineEdge *edge : this->outputEdges()) {
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

    qreal radius = 10.0;
    qreal shadow_distance = 3.;

    QPolygonF shadow_polygon;
    shadow_polygon<<QPointF(d->rect.left()+shadow_distance,d->rect.top()+radius+shadow_distance);
    shadow_polygon<<QPointF(d->rect.left()+radius+shadow_distance,d->rect.top()+shadow_distance);
    shadow_polygon<<QPointF(d->rect.right()+shadow_distance,d->rect.top()+shadow_distance);
    shadow_polygon<<QPointF(d->rect.right()+shadow_distance,d->rect.bottom()-radius+shadow_distance/2);
    shadow_polygon<<QPointF(d->rect.right()-radius+shadow_distance/2,d->rect.bottom()+shadow_distance);
    shadow_polygon<<QPointF(d->rect.left()+shadow_distance,d->rect.bottom()+shadow_distance);

//    QLinearGradient gradient(d->rect.left()+shadow_distance, d->rect.top()+shadow_distance, d->rect.left()+shadow_distance, d->rect.bottom()+shadow_distance);
////    gradient.setColorAt(0.0, d->color);
//    gradient.setColorAt(0.0, Qt::black);
//    gradient.setColorAt(1.0 - shadow_distance/d->rect.height(), QColor(0,0,0,128));
//    gradient.setColorAt(1.0, QColor(0,0,0,0));
////    gradient.setColorAt(10 / d->rect.height(),d->color.darker().darker());
////    gradient.setColorAt(1.0, d->color.darker().darker().darker());

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0,0,0,96));
    painter->drawConvexPolygon(shadow_polygon);

    QPolygonF polygon;
    polygon<<QPointF(d->rect.left(),d->rect.top()+radius);
    polygon<<QPointF(d->rect.left()+radius,d->rect.top());
    polygon<<QPointF(d->rect.right(),d->rect.top());
    polygon<<QPointF(d->rect.right(),d->rect.bottom()-radius);
    polygon<<QPointF(d->rect.right()-radius,d->rect.bottom());
    polygon<<QPointF(d->rect.left(),d->rect.bottom());

    if (this->isSelected()) {
        painter->setPen(QPen(Qt::magenta, 3, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
//        painter->drawRoundedRect(d->rect.adjusted(-1, -1, 1, 1), radius, radius);
        painter->drawConvexPolygon(polygon);
    }


//    painter->setPen(QPen(d->color.darker().darker(), 1, Qt::SolidLine));
//    painter->setPen(QPen(QColor(Qt::black), 1, Qt::SolidLine));
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(d->color));

//    painter->drawRoundedRect(d->rect, radius, radius);
    painter->drawConvexPolygon(polygon);

//    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
//    painter->setBrush(Qt::NoBrush);
//    painter->drawRoundedRect(d->rect.adjusted(2, 2, -2, -2), radius-1, radius-1);
//    painter->drawRoundedRect(d->rect.adjusted(-2, -2, 2, 2), radius+1, radius+1);

    // Drawing node's title
    qreal margin = 5.0;

    QFont font = painter->font();
    QFontMetricsF metrics(font);

    QString title_text = metrics.elidedText(d->algorithm_class, Qt::ElideMiddle, this->boundingRect().width() - 2 - 4 * margin);
    QPointF title_pos  = QPointF(2 * margin, 2 * margin + metrics.xHeight());

    painter->setPen(QPen(QColor(Qt::black)));
    painter->drawText(title_pos, title_text);

    QPolygonF label_polygon;
    label_polygon<<QPointF(d->rect.left()+5*margin,d->rect.top()+4*margin+2+radius);
    label_polygon<<QPointF(d->rect.left()+5*margin+radius,d->rect.top()+4*margin+2);
    label_polygon<<QPointF(d->rect.right()-5*margin,d->rect.top()+4*margin+2);
    label_polygon<<QPointF(d->rect.right()-5*margin,d->rect.bottom());
    label_polygon<<QPointF(d->rect.left()+5*margin,d->rect.bottom());

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(d->color.lighter()));
    painter->drawConvexPolygon(label_polygon);

    font.setPointSize(10);
    QFontMetricsF label_metrics(font);

    QString label_text = label_metrics.elidedText(d->algorithm, Qt::ElideMiddle, this->boundingRect().width() - 2 - 12 * margin);
    QPointF label_pos = QPointF(200 - 6 * margin - label_metrics.boundingRect(label_text).width(), 4 * margin + 2 + metrics.xHeight() + label_metrics.xHeight());

    painter->setFont(font);
    painter->setPen(QPen(QColor(Qt::black)));
    painter->drawText(label_pos, label_text);
} */

QList<gnomonPipelineEdge *> gnomonPipelineNode::inputEdges(void)
{
    return d->input_edges;
}

QList<gnomonPipelineEdge *> gnomonPipelineNode::outputEdges(void)
{
    return d->output_edges;
}

int gnomonPipelineNode::inputEdgeCount(void)
{
    return d->input_edges.size();
}

int gnomonPipelineNode::outputEdgeCount(void)
{
    return d->output_edges.size();
}

gnomonPipelineEdge *gnomonPipelineNode::inputEdgeAt(int index)
{
    if (index < d->input_edges.size()) {
        return d->input_edges[index];
    } else {
        return nullptr;
    }
}

gnomonPipelineEdge *gnomonPipelineNode::outputEdgeAt(int index)
{
    if (index < d->output_edges.size()) {
        return d->output_edges[index];
    } else {
        return nullptr;
    }
}

void gnomonPipelineNode::addInputEdge(gnomonPipelineEdge *edge)
{
    d->input_edges << edge;
    d->input_edges_map.insert(edge->target()->name(), edge);
}

void gnomonPipelineNode::addOutputEdge(gnomonPipelineEdge *edge)
{
    d->output_edges << edge;
}

void gnomonPipelineNode::removeInputEdge(gnomonPipelineEdge *edge)
{
    d->input_edges.removeAll(edge);
    d->input_edges_map.remove(edge->target()->name());
}

void gnomonPipelineNode::removeOutputEdge(gnomonPipelineEdge *edge)
{
    d->output_edges.removeAll(edge);
}

const QMap<QString, gnomonPipelinePort *>& gnomonPipelineNode::inputPorts(void)
{
    return d->input_ports;
}

const QMap<QString, gnomonPipelinePort *>& gnomonPipelineNode::outputPorts(void)
{
    return d->output_ports;
}

QStringList gnomonPipelineNode::inputPortsNames(void)
{
    return d->input_ports.keys();
}

QStringList gnomonPipelineNode::outputPortsNames(void)
{
    return d->output_ports.keys();
}


gnomonPipelinePort *gnomonPipelineNode::inputPort(const QString& name)
{
    if (d->input_ports.contains(name)) {
        return d->input_ports[name];
    } else {
        return nullptr;
    }
}

gnomonPipelinePort *gnomonPipelineNode::outputPort(const QString& name)
{
    if (d->output_ports.contains(name)) {
        return d->output_ports[name];
    } else {
        return nullptr;
    }
}

int gnomonPipelineNode::addInputPort(const QString& name, gnomonPipelinePort *port)
{
    d->input_ports[name] = port;
    emit inputPortsChanged();
    return d->input_ports.size() - 1;
}

void gnomonPipelineNode::addOutputPort(const QString& name, gnomonPipelinePort *port)
{
    d->output_ports[name] = port;
    emit outputPortsChanged();
}

void gnomonPipelineNode::removeInputPort(gnomonPipelinePort *port)
{
    const QString key = d->input_ports.key(port, "");
    if (key != "") {
        d->input_ports.remove(key);
    }
}

void gnomonPipelineNode::removeOutputPort(gnomonPipelinePort *port)
{
    const QString key = d->output_ports.key(port, "");
    if (key != "") {
        d->output_ports.remove(key);
    }
}

QString gnomonPipelineNode::toToml(void)
{
    QString node_string = "[" + d->name + "]\n" + "\n";
    return node_string;
}

QString gnomonPipelineNode::toLuigiClass(void)
{
    return "";
}

const QJsonObject gnomonPipelineNode::toJson(void)
{
    QJsonObject json;
    json.insert("name", d->name);
    json.insert("plugin_group", d->algorithm_class);
    json.insert("plugin_name", d->algorithm);
    json.insert("plugin_version", d->version);
    json.insert("description", d->description);

    QJsonObject in;
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        in.insert(input_name, QJsonValue::Null);
    }
    json.insert("inputs", in);

    QJsonArray out;
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out.append(output_name);
    }
    json.insert("outputs", out);

    return json;
}

gnomonPipelineEdge *gnomonPipelineNode::inputEdgeFromPort(const QString& portName) {
    if(d->input_edges_map.contains(portName)) {
        return d->input_edges_map[portName];
    }
    return nullptr;
}

//
// gnomonPipelineNode.cpp ends here
