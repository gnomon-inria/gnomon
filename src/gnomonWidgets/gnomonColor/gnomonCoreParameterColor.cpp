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

#include "gnomonCoreParameterColor.h"

#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorMap
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QMap<double, QColor>& c, const QString& doc) : gnomonCoreParameter(doc), m_c(c), m_n("no name")
{
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QString& clut, const QString& doc) : gnomonCoreParameter(doc), m_n(clut)
{
    this->setValue(clut);
}

QMap<double, QColor> gnomonCoreParameterColorMap::value(void) const
{
    return m_c;
}

QString gnomonCoreParameterColorMap::name(void) const
{
    return m_n;
}


void gnomonCoreParameterColorMap::setValue(const QMap<double, QColor>& c)
{
    m_c = c;
}

void gnomonCoreParameterColorMap::setValue(const QString& clut)
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

    if(m_c != colormap) {
        m_c = colormap;
        m_n = clut;
        emit valueChanged();
    }
}

void gnomonCoreParameterColorMap::setName(const QString& clut)
{
    m_n = clut;
}

void gnomonCoreParameterColorMap::copy(gnomonCoreParameter *other)
{
    if (gnomonCoreParameterColorMap *param = dynamic_cast<gnomonCoreParameterColorMap *>(other)) {
        m_n = param->name();
        m_c = param->value();
        emit valueChanged();
    }
}


// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterLookupTable
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(const gnomonLookupTable& lut, const QString& doc) : gnomonCoreParameter(doc), m_l(lut)
{
}

gnomonLookupTable gnomonCoreParameterLookupTable::value(void) const
{
    return m_l;
}

void gnomonCoreParameterLookupTable::setValue(const gnomonLookupTable& lut)
{
    m_l = lut;
    m_l.setColorMap(lut.colorMapName());
}

void gnomonCoreParameterLookupTable::copy(gnomonCoreParameter *other)
{
    if (gnomonCoreParameterLookupTable *param = dynamic_cast<gnomonCoreParameterLookupTable *>(other)) {
        m_l = param->value();
        emit valueChanged();
    }
}


//
// gnomonCoreParameterColor.cpp ends here