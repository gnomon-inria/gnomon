// Version: $Id: b3634632a3e3eac8bcc4a3b15a61a881bb42aa64 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonColorMapEditor.h"
#include <cmath>

#include <QtXml>
#include <gnomonStyle>

// /////////////////////////////////////////////////////////////////
// gnomonColorMapTable
// /////////////////////////////////////////////////////////////////

class gnomonColorMapTable : public QWidget
{
public:
     gnomonColorMapTable(QWidget *parent = 0);
    ~gnomonColorMapTable(void);

    void addColorPoint(double value, const QColor& color);
    void removeColorPoints(void);

public:
    void paintEvent(QPaintEvent *event);

public:
    QMap<double, QColor> color_points;
};

gnomonColorMapTable::gnomonColorMapTable(QWidget *parent) : QWidget(parent)
{
    color_points[0] = Qt::black;
    color_points[1] = Qt::white;
}

gnomonColorMapTable::~gnomonColorMapTable(void)
{

}

void gnomonColorMapTable::addColorPoint(double value, const QColor& color)
{
    color_points[value] = color;
    this->update();
}

void gnomonColorMapTable::removeColorPoints(void)
{
    color_points.clear();
}

void gnomonColorMapTable::paintEvent(QPaintEvent* event)
// (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Q_UNUSED(option);
    // Q_UNUSED(widget);

    QRect rectangle = event->rect();
    QPainter painter(this);

    // painter->save();

    int x = rectangle.bottomLeft().x();
    int y = rectangle.topRight().y();
    int lx = rectangle.width();
    int ly = rectangle.height();

    // QPen pen(Qt::black, 1);

    QLinearGradient linearGradient(x, y, x + lx, y);

    if (color_points.keys().first() > 0) {
        QColor color = color_points[color_points.keys().first()];
        color.setAlpha(255);
        linearGradient.setColorAt(0.0, color);
    }

    for (const auto& val : color_points.keys()) {
        QColor color = color_points[val];
        color.setAlpha(255);
        linearGradient.setColorAt(val, color);
    }

    if (color_points.keys().last()<1) {
        QColor color = color_points[color_points.keys().last()];
        color.setAlpha(255);
        linearGradient.setColorAt(1.0, color);
    }

    // painter->setPen(pen);
    // painter.setBrush(linearGradient);
    // painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(x, y, lx, ly, linearGradient);

    // painter->restore();
}

// /////////////////////////////////////////////////////////////////
// gnomonColorMapEditor
// /////////////////////////////////////////////////////////////////

class gnomonColorMapEditorPrivate
{
public:
    QString name;
    QMap<double, QColor> value;

public:
    QComboBox *colormap_box;
    gnomonColorMapTable *colormap_table;
};

gnomonColorMapEditor::gnomonColorMapEditor(QWidget *parent) : QWidget(parent), d(new gnomonColorMapEditorPrivate)
{
    d->value[0] = Qt::black;
    d->value[1] = Qt::white;
    d->name = "";

    QPalette palette ;
    palette.setBrush(QPalette::Background, Qt::transparent);

    d->colormap_box = new QComboBox(this);

    d->colormap_box->addItem("0CMY_cyan.clut");
    d->colormap_box->addItem("0CMY_magenta.clut");
    d->colormap_box->addItem("0CMY_yellow.clut");
    d->colormap_box->addItem("0RGB_blue.clut");
    d->colormap_box->addItem("0RGB_green.clut");
    d->colormap_box->addItem("0RGB_red.clut");
    d->colormap_box->addItem("1Flashy_blue.clut");
    d->colormap_box->addItem("1Flashy_green.clut");
    d->colormap_box->addItem("1Flashy_orange.clut");
    d->colormap_box->addItem("1Flashy_purple.clut");
    d->colormap_box->addItem("1Flashy_red.clut");
    d->colormap_box->addItem("1Flashy_turquoise.clut");
    d->colormap_box->addItem("acidity.clut");
    d->colormap_box->addItem("atmosphere.clut");
    d->colormap_box->addItem("Blues.clut");
    d->colormap_box->addItem("bone.clut");
    d->colormap_box->addItem("BrBG.clut");
    d->colormap_box->addItem("bronze.clut");
    d->colormap_box->addItem("BuGn.clut");
    d->colormap_box->addItem("BuGrRd.clut");
    d->colormap_box->addItem("chestnut.clut");
    d->colormap_box->addItem("cold.clut");
    d->colormap_box->addItem("curvature.clut");
    d->colormap_box->addItem("density.clut");
    d->colormap_box->addItem("geo_jet.clut");
    d->colormap_box->addItem("glasbey.clut");
    d->colormap_box->addItem("GnBu.clut");
    d->colormap_box->addItem("Greens.clut");
    d->colormap_box->addItem("grey.clut");
    d->colormap_box->addItem("Greys.clut");
    d->colormap_box->addItem("hot.clut");
    d->colormap_box->addItem("inferno.clut");
    d->colormap_box->addItem("jet.clut");
    d->colormap_box->addItem("leaf.clut");
    d->colormap_box->addItem("morocco.clut");
    d->colormap_box->addItem("ocean.clut");
    d->colormap_box->addItem("Oranges.clut");
    d->colormap_box->addItem("PiYG.clut");
    d->colormap_box->addItem("primordia_jet.clut");
    d->colormap_box->addItem("PuBu.clut");
    d->colormap_box->addItem("PuBuGn.clut");
    d->colormap_box->addItem("PuOr.clut");
    d->colormap_box->addItem("PuRd.clut");
    d->colormap_box->addItem("Purples.clut");
    d->colormap_box->addItem("quercus.clut");
    d->colormap_box->addItem("RdBu.clut");
    d->colormap_box->addItem("RdYlBu.clut");
    d->colormap_box->addItem("RdYlGn.clut");
    d->colormap_box->addItem("Reds.clut");
    d->colormap_box->addItem("temperature.clut");
    d->colormap_box->addItem("tourmaline.clut");
    d->colormap_box->addItem("viridis.clut");
    d->colormap_box->addItem("wine.clut");
    d->colormap_box->addItem("YlGn.clut");
    d->colormap_box->addItem("YlOrBr.clut");

    d->colormap_table = new gnomonColorMapTable();
    d->colormap_table->setFixedHeight(40);
    d->colormap_table->setMinimumWidth(120);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(d->colormap_box);
    layout->addWidget(d->colormap_table);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    connect(d->colormap_box, SIGNAL(currentTextChanged(const QString&)), this, SLOT(importColorMap(const QString&)));
}

gnomonColorMapEditor::~gnomonColorMapEditor(void)
{
    delete d;

    d = NULL;
}

const QString& gnomonColorMapEditor::name(void) const
{
    return d->name;
}

const QMap<double, QColor>& gnomonColorMapEditor::value(void) const
{
    return d->value;
}

void gnomonColorMapEditor::setName(const QString& name)
{
    d->name = name;
}

void gnomonColorMapEditor::setValue(const QMap<double, QColor>& value)
{
    d->value = value;
    d->colormap_table->removeColorPoints();

    for (const auto& v : value.keys())
        d->colormap_table->addColorPoint(v,value[v]);
    d->colormap_table->update();

    emit valueChanged(d->value);
}

void gnomonColorMapEditor::importColorMap(const QString& clut)
{
    QString fileName = QString(":gnomon/cluts/%1").arg(clut);

    QDomDocument doc("clut");

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

    file.close();

    QMap<double, QColor> colormap;

    QDomElement root = doc.documentElement();
    double min = root.attribute("min").toDouble();
    double max = root.attribute("max").toDouble();
    int    log = root.attribute("log").toInt();

    QDomNode n = root.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {

            qreal v = e.attribute("v").toDouble();
            qreal a = e.attribute("a").toDouble();

            int r = e.attribute("r").toInt();
            int g = e.attribute("g").toInt();
            int b = e.attribute("b").toInt();

            double val = (v-min)/(max-min);
            colormap[val] = QColor(r,g,b);
        }
        n = n.nextSibling();
    }

    this->setValue(colormap);
}

//
// gnomonColorMapEditor.cpp ends here
