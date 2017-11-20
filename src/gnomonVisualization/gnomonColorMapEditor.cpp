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
    QLabel *label_name;

    QWidget * colormap_widget;
    QPushButton *button_import;
    gnomonColorMapTable *colormap_table;
};

gnomonColorMapEditor::gnomonColorMapEditor(QWidget *parent) : QWidget(parent), d(new gnomonColorMapEditorPrivate)
{
    d->value[0] = Qt::black;
    d->value[1] = Qt::white;
    d->name = "";

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->label_name = new QLabel(d->name);
    layout->addWidget(d->label_name);

    d->colormap_widget = new QWidget();
    QHBoxLayout *colormap_layout = new QHBoxLayout(d->colormap_widget);
    colormap_layout->setContentsMargins(0, 0, 0, 0);

    QPalette palette ;
    palette.setBrush(QPalette::Background, Qt::transparent);

    d->button_import = new QPushButton("Import");
    d->button_import->setPalette(palette);
    d->button_import->setAutoFillBackground(false);
    d->button_import->setStyleSheet(gnomonStyleSheet());
    d->button_import->setFixedWidth(120);
    colormap_layout->addWidget(d->button_import);

    d->colormap_table = new gnomonColorMapTable();
    d->colormap_table->setMinimumHeight(20);
    d->colormap_table->setMinimumWidth(120);
    colormap_layout->addWidget(d->colormap_table);

    layout->addWidget(d->colormap_widget);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    connect(d->button_import, SIGNAL(clicked()), this, SLOT(importColorMap()));
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
    d->label_name->setText(d->name);
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

void gnomonColorMapEditor::importColorMap(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import color lookup table");

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
