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

gnomonLookupTable::gnomonLookupTable(const gnomonColorMap& c, const QList<double> a, const QList<double> r, bool v) : colormap(c), min(a[0]), max(a[1]),
                                                                                                                      r_min(r[0]), r_max(r[1]), visible(v)
{
    this->name = "no_name";
}

gnomonLookupTable::gnomonLookupTable(const QString& clut, const QList<double> a, const QList<double> r, bool v) : min(a[0]), max(a[1]),
                                                                                                                  r_min(r[0]), r_max(r[1]), visible(v)
{
    this->setColorMap(clut);
}

gnomonLookupTable::gnomonLookupTable(const gnomonLookupTable& o) : name(o.name) ,colormap(o.colormap),
                                                                   min(o.min), max(o.max),
                                                                   r_min(o.r_min), r_max(o.r_max), visible(o.visible)
{
}

gnomonLookupTable& gnomonLookupTable::operator = (const gnomonLookupTable& o)
{
    if(this != &o) {
        this->name = o.name;
        this->colormap = o.colormap;
        this->min = o.min;
        this->max = o.max;
        this->r_min = o.r_min;
        this->r_max = o.r_max;
        this->visible = o.visible;
    }

    return *this;
}

bool gnomonLookupTable::operator != (const gnomonLookupTable& o)
{
    if(this->name == o.name && this->visible == o.visible) {
        if((this->min == o.min) && (this->max == o.max) ) {
            if((this->r_max = o.r_max) && (this->r_min = o.r_min)) {
                return this->colormap == o.colormap;
            }
        }
    }

    return false;
}


const QString& gnomonLookupTable::colorMapName(void) const
{
    return this->name;
}

const gnomonColorMap& gnomonLookupTable::colorMap(void) const
{
    return this->colormap;
}

double gnomonLookupTable::valueMin(void) const
{
    return this->min;
}

double gnomonLookupTable::rangeMin(void) const
{
    return this->r_min;
}

double gnomonLookupTable::valueMax(void) const
{
    return this->max;
}

double gnomonLookupTable::rangeMax(void) const
{
    return this->r_max;
}

bool gnomonLookupTable::visibility(void) const
{
    return this->visible;
}

void gnomonLookupTable::setName(const QString& n)
{
    this->name = n;
}

void gnomonLookupTable::setColorMap(const gnomonColorMap& c)
{
    this->colormap = c;
}

void gnomonLookupTable::setColorMap(const QString& clut)
{
    gnomonColorMap colormap = getColorMap(clut);

    this->colormap = colormap;
}

gnomonColorMap & gnomonLookupTable::getColorMap(const QString &clut) {
    static QHash<QString, gnomonColorMap> memoize;
    if(memoize.contains(clut)) {
        return memoize[clut];
    }
    name = clut;

    QString fileName = QString(":gnomon/cluts/%1.clut").arg(clut);

    QDomDocument doc("clut");

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return this->colormap;

    if (!doc.setContent(&file)) {
        file.close();
        return this->colormap;
    }

    file.close();

    gnomonColorMap _colormap;

    QDomElement root = doc.documentElement();
    double min = root.attribute("min").toDouble();
    double max = root.attribute("max").toDouble();
    int    log = root.attribute("log").toInt();

    Q_UNUSED(log);

    QDomNode n = root.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {

            qreal v = e.attribute("v").toDouble();
            qreal a = e.attribute("a").toDouble();

            Q_UNUSED(a);

            int r = e.attribute("r").toInt();
            int g = e.attribute("g").toInt();
            int b = e.attribute("b").toInt();

            double val = (v-min)/(max-min);
            _colormap[val] = QColor(r, g, b);
        }
        n = n.nextSibling();
    }

    memoize.insert(clut, _colormap);
    return memoize[clut];
}


void gnomonLookupTable::setValueMin(double m)
{
    this->min = m;
}

void gnomonLookupTable::setRangeMin(double m)
{
    this->r_min = m;
}

void gnomonLookupTable::setValueMax(double M)
{
    this->max = M;
}

void gnomonLookupTable::setRangeMax(double M)
{
    this->r_max = M;
}

void gnomonLookupTable::setVisibility(bool vis)
{
    this->visible = vis;
}

//
// gnomonLookupTableColor.cpp ends here
