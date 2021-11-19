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

#include "gnomonCoreParameterColorObject.h"

#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorMap
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(void) : dtkCoreParameterBase<gnomonCoreParameterColorMap>()
{
    m_object = new gnomonCoreParameterColorMapObject(this);
}

gnomonCoreParameterColorMap::~gnomonCoreParameterColorMap(void)
{
    delete m_object;
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const dtkCoreParameter *p) : dtkCoreParameterBase<gnomonCoreParameterColorMap>()
{
    m_object = new gnomonCoreParameterColorMapObject(this);
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return;
    }
    *this = p->variant();
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QMap<double, QColor>& cm, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterColorMap>(), m_c(cm)
{
    m_object = new gnomonCoreParameterColorMapObject(this);
    m_doc = doc;
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QString& cm_id, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterColorMap>()
{
    m_object = new gnomonCoreParameterColorMapObject(this);
    m_doc = doc;
    this->setValue(cm_id);
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QVariant& v) : dtkCoreParameterBase<gnomonCoreParameterColorMap>()
{
    m_object = new gnomonCoreParameterColorMapObject(this);
    if (v.canConvert<gnomonCoreParameterColorMap>()) {
        auto o(v.value<gnomonCoreParameterColorMap>());
        *this = o;

    } else if (v.canConvert<QMap<double, QColor>>()) {
        this->setValue(v.value<QMap<double, QColor>>());

    } else if (v.canConvert<QString>()) {
        this->setValue(v.toString());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<gnomonCoreParameterColorMap>())
                  << ". Nothing is done.";
    }
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const gnomonCoreParameterColorMap& o) : dtkCoreParameterBase<gnomonCoreParameterColorMap>(o), m_c(o.m_c), m_n(o.m_n)
{
    m_object = new gnomonCoreParameterColorMapObject(this);
}

gnomonCoreParameterColorMap& gnomonCoreParameterColorMap::operator = (const dtkCoreParameter *p)
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return *this;
    }
    return *this = p->variant();
}

gnomonCoreParameterColorMap& gnomonCoreParameterColorMap::operator = (const QVariant& v)
{
    if (v.canConvert<gnomonCoreParameterColorMap>()) {
        *this = v.value<gnomonCoreParameterColorMap>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_n = map["clut"].toString();

        m_c.clear();

        auto keys = map["keys"].toList();
        auto colors = map["colors"].toList();
        for(int i=0; i< keys.size(); ++i) {
            m_c[keys[i].toDouble()] = colors[i].value<QColor>();
        }
        m_object->notifyColorMap(m_c);

    } else if (v.canConvert<QMap<double, QColor>>()) {
        this->setValue(v.value<QMap<double, QColor>>());

    } else if (v.canConvert<QString>()) {
        this->setValue(v.toString());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<gnomonCoreParameterColorMap>())
                  << ". Nothing is done.";
        return *this;
    }
    return *this;
}

gnomonCoreParameterColorMap& gnomonCoreParameterColorMap::operator = (const gnomonCoreParameterColorMap& o)
{
  if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_c = o.m_c;
        m_n = o.m_n;
        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorMap(m_c);
        m_object->notifyName(m_n);
    }
    return *this;
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
    m_object->notifyColorMap(m_c);
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
            colormap[val] = QColor(r,g,b);
        }
        n = n.nextSibling();
    }

    if(m_c != colormap) {
        m_c = colormap;
        m_n = clut;
        m_object->notifyColorMap(m_c);
        m_object->notifyName(m_n);
    }
}

void gnomonCoreParameterColorMap::setValue(const QVariant& v)
{
    if (v.canConvert<gnomonCoreParameterColorMap>()) {
        *this = v.value<gnomonCoreParameterColorMap>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        this->m_n = map["clut"].toString();

        this->m_c.clear();

        auto keys = map["keys"].toList();
        auto colors = map["colors"].toList();
        for(int i=0; i< keys.size(); ++i) {
            this->m_c[keys[i].toDouble()] = colors[i].value<QColor>();
        }
        m_object->notifyColorMap(m_c);

    } else if (v.canConvert<QMap<double, QColor>>()) {
        this->setValue(v.value<QMap<double, QColor>>());

    } else if (v.canConvert<QString>()) {
        this->setValue(v.toString());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<gnomonCoreParameterColorMap>())
                  << ". Nothing is done.";
        return;
    }
    return;
}

void gnomonCoreParameterColorMap::setName(const QString& clut)
{
    m_n = clut;
    m_object->notifyName(m_n);
}

QVariantHash gnomonCoreParameterColorMap::toVariantHash(void) const
{
    QVariantHash hash = dtkCoreParameterBase<gnomonCoreParameterColorMap>::toVariantHash();
    hash.insert("clut", m_n);

    QList<QVariant> keys; keys.reserve(m_c.size());
    QList<QVariant> colors; colors.reserve(m_c.size());
    QMap<double, QColor>::const_iterator i = m_c.constBegin();
    while (i != m_c.constEnd()) {
        keys << QVariant::fromValue(i.key());
        colors << QVariant::fromValue(i.value());
        ++i;
    }
    hash.insert("keys", keys);
    hash.insert("colors", colors);

    return hash;
}

dtkCoreParameterObject *gnomonCoreParameterColorMap::object(void)
{
    return m_object;
}


/*
// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterLookupTable
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(const gnomonLookupTable& lut, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterLookupTable>(), m_l(lut)
{
    m_doc = doc;
}

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(const gnomonCoreParameterLookupTable& o) : dtkCoreParameterBase<gnomonCoreParameterLookupTable>(o), m_l(o.m_l)
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

gnomonCoreParameterLookupTable& gnomonCoreParameterLookupTable::operator = (const QVariant& v)
{
        if (v.canConvert<gnomonCoreParameterLookupTablep>()) {
        *this = v.value<gnomonCoreParameterLookupTable>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_n = map["clut"].toString();

        m_c.clear();

        auto keys = map["keys"].toList();
        auto colors = map["colors"].toList();
        for(int i=0; i< keys.size(); ++i) {
            m_c[keys[i].toDouble()] = colors[i].value<QColor>();
        }

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<gnomonCoreParameterColorMap>())
                  << ". Nothing is done.";
        this->syncFail();
        return *this;
    }
    this->sync();
    return *this;
}

gnomonCoreParameterLookupTable& gnomonCoreParameterLookupTable::operator = (const gnomonCoreParameterLookupTable& o)
{
  if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_l = o.m_l;
        this->sync();
    }
    return *this;
}

QVariantHash gnomonCoreParameterLookupTable::toVariantHash(void) const
{
    QVariantHash hash = dtkCoreParameterBase<gnomonCoreParameterLookupTable>::toVariantHash();
    hash.insert("lut", m_l);

    return hash;
}
*/
//
// gnomonCoreParameterColor.cpp ends here
