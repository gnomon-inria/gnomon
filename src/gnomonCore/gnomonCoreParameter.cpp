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

#include "gnomonCoreParameter.h"

#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameter
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameter::gnomonCoreParameter(const QString& doc) : m_doc(doc)
{
}

QString gnomonCoreParameter::doc(void) const
{
    return m_doc;
}

void gnomonCoreParameter::setValue(const QVariant& v)
{
    return;
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterBool
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterBool::gnomonCoreParameterBool(bool val, const QString& doc) : gnomonCoreParameter(doc), m_value(val)
{
}

bool gnomonCoreParameterBool::value(void) const
{
    return m_value;
}

void gnomonCoreParameterBool::setValue(bool val)
{
    m_value = val;
}

void gnomonCoreParameterBool::setValue(const QVariant& v)
{
    m_value = v.toBool();
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterString
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterString::gnomonCoreParameterString(const QString& s, const QString& doc) : gnomonCoreParameter(doc), m_s(s)
{
}

QString gnomonCoreParameterString::value(void) const
{
    return m_s;
}

void gnomonCoreParameterString::setValue(const QString& s)
{
    m_s = s;
}

void gnomonCoreParameterString::setValue(const QVariant& v)
{
    m_s = v.toString();
}
// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterStringList
// ///////////////////////////////////////////////////////////////////


gnomonCoreParameterStringList::gnomonCoreParameterStringList(const QString& value, const QStringList& values, const QString& doc) : gnomonCoreParameter(doc), m_values(values)
{
    Q_ASSERT(m_values.contains(value));
    this->setCurrentIndex(m_values.indexOf(value));
}

gnomonCoreParameterStringList::gnomonCoreParameterStringList(const QStringList& values, const QString& doc) : gnomonCoreParameter(doc), m_values(values)
{
}

int gnomonCoreParameterStringList::size(void) const
{
    return m_values.size();
}

int gnomonCoreParameterStringList::currentIndex(void) const
{
    return m_current_index;
}

// QString gnomonCoreParameterStringList::currentValue(void) const
QString gnomonCoreParameterStringList::value(void) const
{
    if (!m_values.empty()) {
        return m_values.at(m_current_index);
    }
    return QString();
}

QStringList gnomonCoreParameterStringList::values(void) const
{
    return m_values;
}

void gnomonCoreParameterStringList::setCurrentIndex(int current_index)
{
    m_current_index = current_index;
}

void gnomonCoreParameterStringList::addValue(const QString& val)
{
    if (!m_values.contains(val)) {
        m_values.append(val);
    };
}

void gnomonCoreParameterStringList::removeValue(const QString& val)
{
    m_values.removeAll(val);
    if (m_values.empty()) {
        m_current_index = 0;
    } else if (m_current_index == m_values.size()) {
        --m_current_index;
    }
}

void gnomonCoreParameterStringList::setValues(const QStringList& val)
{
    QString c_val = m_values.at(m_current_index);

    m_values.clear();
    for (const auto& v : val) {
        if (!m_values.contains(v)) {
            m_values.append(v);
        }
    }

    int index = m_values.indexOf(c_val);
    if(index >= 0) {
        m_current_index = index;
    }
}

void gnomonCoreParameterStringList::setValue(const QString& val)
{
    int index = m_values.indexOf(val);
    if(index >= 0) { 
        m_current_index = index; 
    }
}

void gnomonCoreParameterStringList::setValue(const QVariant& v)
{
    QString val = v.toString();
    int index = m_values.indexOf(val);
    if(index >= 0)
    { m_current_index = index; }
}


// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorMap
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QMap<double, QColor>& c, const QString& doc) : gnomonCoreParameter(doc), m_c(c)
{
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QString& clut, const QString& doc) : gnomonCoreParameter(doc)
{
    this->setValue(clut);
}

QMap<double, QColor> gnomonCoreParameterColorMap::value(void) const
{
    return m_c;
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
    m_c = colormap;
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterVariant
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterVariant::gnomonCoreParameterVariant(const QVariant& v, const QString& doc) : gnomonCoreParameter(doc), m_v(v)
{
}

QVariant gnomonCoreParameterVariant::value(void) const
{
    return m_v;
}

void gnomonCoreParameterVariant::setValue(const QVariant& v)
{
    m_v = v;
}

//
// gnomonCoreParameter.cpp ends here
