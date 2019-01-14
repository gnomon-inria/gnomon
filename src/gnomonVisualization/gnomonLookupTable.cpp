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

#include "gnomonLookupTable.h"

#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonLookupTable
// ///////////////////////////////////////////////////////////////////

gnomonLookupTable::gnomonLookupTable(const QMap<double, QColor>& c, const QList<double> r, bool v) : colormap(c), value_range(r), visible(v)
{
    this->name = "no_name";
}

gnomonLookupTable::gnomonLookupTable(const QString& clut, const QList<double> r, bool v) : value_range(r), visible(v)
{
    this->setColorMap(clut);
}

const QString& gnomonLookupTable::colorMapName(void) const
{
    return this->name;
}

const QMap<double, QColor>& gnomonLookupTable::colorMap(void) const
{
    return this->colormap;
}

const QList<double>& gnomonLookupTable::valueRange(void) const
{
    return this->value_range;
}

bool gnomonLookupTable::visibility(void) const
{
    return this->visible;
}

void gnomonLookupTable::setName(const QString& n)
{
    this->name = n;
}

void gnomonLookupTable::setColorMap(const QMap<double, QColor>& c)
{
    this->colormap = c;
}

void gnomonLookupTable::setColorMap(const QString& clut)
{
    this->name = clut;
    
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

    this->colormap = colormap;
}


void gnomonLookupTable::setValueRange(const QList<double>& range)
{
    this->value_range = range;
}

void gnomonLookupTable::setVisibility(bool vis)
{
    this->visible = vis;
}

//
// gnomonLookupTableColor.cpp ends here