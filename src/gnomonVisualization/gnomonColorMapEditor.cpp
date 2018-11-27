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
    QMap<QString, QMap<double, QColor> > colormaps;
    QString current_map;

public:
    QComboBox *colormap_box;
    gnomonColorMapTable *colormap_table;
};

gnomonColorMapEditor::gnomonColorMapEditor(QWidget *parent) : QWidget(parent), d(new gnomonColorMapEditorPrivate)
{
    d->name = "";

    QPalette palette ;
    palette.setBrush(QPalette::Background, Qt::transparent);

    d->colormap_box = new QComboBox(this);

    d->colormap_box->addItem("0CMY_cyan");
    d->colormap_box->addItem("0CMY_magenta");
    d->colormap_box->addItem("0CMY_yellow");
    d->colormap_box->addItem("0RGB_blue");
    d->colormap_box->addItem("0RGB_green");
    d->colormap_box->addItem("0RGB_red");
    d->colormap_box->addItem("1Flashy_blue");
    d->colormap_box->addItem("1Flashy_green");
    d->colormap_box->addItem("1Flashy_orange");
    d->colormap_box->addItem("1Flashy_purple");
    d->colormap_box->addItem("1Flashy_red");
    d->colormap_box->addItem("1Flashy_turquoise");
    d->colormap_box->addItem("acidity");
    d->colormap_box->addItem("atmosphere");
    d->colormap_box->addItem("Blues");
    d->colormap_box->addItem("bone");
    d->colormap_box->addItem("BrBG");
    d->colormap_box->addItem("bronze");
    d->colormap_box->addItem("BuGn");
    d->colormap_box->addItem("BuGrRd");
    d->colormap_box->addItem("chestnut");
    d->colormap_box->addItem("cold");
    d->colormap_box->addItem("curvature");
    d->colormap_box->addItem("density");
    d->colormap_box->addItem("geo_jet");
    d->colormap_box->addItem("glasbey");
    d->colormap_box->addItem("GnBu");
    d->colormap_box->addItem("Greens");
    d->colormap_box->addItem("grey");
    d->colormap_box->addItem("Greys");
    d->colormap_box->addItem("hot");
    d->colormap_box->addItem("inferno");
    d->colormap_box->addItem("jet");
    d->colormap_box->addItem("leaf");
    d->colormap_box->addItem("morocco");
    d->colormap_box->addItem("ocean");
    d->colormap_box->addItem("Oranges");
    d->colormap_box->addItem("PiYG");
    d->colormap_box->addItem("primordia_jet");
    d->colormap_box->addItem("PuBu");
    d->colormap_box->addItem("PuBuGn");
    d->colormap_box->addItem("PuOr");
    d->colormap_box->addItem("PuRd");
    d->colormap_box->addItem("Purples");
    d->colormap_box->addItem("quercus");
    d->colormap_box->addItem("RdBu");
    d->colormap_box->addItem("RdYlBu");
    d->colormap_box->addItem("RdYlGn");
    d->colormap_box->addItem("Reds");
    d->colormap_box->addItem("temperature");
    d->colormap_box->addItem("tourmaline");
    d->colormap_box->addItem("viridis");
    d->colormap_box->addItem("wine");
    d->colormap_box->addItem("YlGn");
    d->colormap_box->addItem("YlOrBr");

    d->colormap_table = new gnomonColorMapTable();
    d->colormap_table->setFixedHeight(40);
    d->colormap_table->setMinimumWidth(120);

    d->current_map = "grey";
    d->colormap_box->setCurrentText(d->current_map);
    importColorMap(d->current_map);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
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
    if(!d->colormaps.contains(d->current_map)) {
        qWarning() << Q_FUNC_INFO << "shouldn't happen";
    }

    return d->colormaps[d->current_map];
}

void gnomonColorMapEditor::setName(const QString& name)
{
    d->name = name;
}

void gnomonColorMapEditor::setValue(const QMap<double, QColor>& value)
{
    if(!d->colormaps.key(value, "").isEmpty() ) {
        d->current_map = d->colormaps.key(value);
    } else {
        qWarning() << Q_FUNC_INFO << "insert no_name color map";
        d->current_map = "no_name";
        d->colormaps.insert(d->current_map, value);
        d->colormap_box->addItem(d->current_map);
    }

    d->colormap_box->setCurrentText(d->current_map);
    d->colormap_table->removeColorPoints();

    for (const auto& v : value.keys())
        d->colormap_table->addColorPoint(v,value[v]);
    d->colormap_table->update();

    emit valueChanged(value);
}

void gnomonColorMapEditor::importColorMap(const QString& clut)
{
    QString fileName = QString(":gnomon/cluts/%1.clut").arg(clut);

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

    d->colormaps.insert(clut, colormap);
    this->setValue(colormap);
}

//
// gnomonColorMapEditor.cpp ends here
