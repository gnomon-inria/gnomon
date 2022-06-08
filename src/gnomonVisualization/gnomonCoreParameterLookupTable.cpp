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

#include "gnomonCoreParameterLookupTable.h"

#include "gnomonCoreParameterLookupTableObject.h"

#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterLookupTable
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(void) : dtkCoreParameterBase<gnomonCoreParameterLookupTable>()
{
    m_object = new gnomonCoreParameterLookupTableObject(this);
}

gnomonCoreParameterLookupTable::~gnomonCoreParameterLookupTable(void)
{
    delete m_object;
}

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(const dtkCoreParameter *p) : dtkCoreParameterBase<gnomonCoreParameterLookupTable>()
{
    m_object = new gnomonCoreParameterLookupTableObject(this);
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return;
    }
    *this = p->variant();
}

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(const QString& label, const gnomonLookupTable& lut, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterLookupTable>(), m_l(lut)
{
    m_object = new gnomonCoreParameterLookupTableObject(this);
    m_label = label;
    m_doc = doc;
}

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(const QVariant& v) : dtkCoreParameterBase<gnomonCoreParameterLookupTable>()
{
    m_object = new gnomonCoreParameterLookupTableObject(this);
    if (v.canConvert<gnomonCoreParameterLookupTable>()) {
        auto o(v.value<gnomonCoreParameterLookupTable>());
        *this = o;

    } else if (v.canConvert<gnomonLookupTable>()) {
        this->setValue(v.value<gnomonLookupTable>());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterLookupTable>().name()
                  << ". Nothing is done.";
    }
}

gnomonCoreParameterLookupTable::gnomonCoreParameterLookupTable(const gnomonCoreParameterLookupTable& o) : dtkCoreParameterBase<gnomonCoreParameterLookupTable>(o), m_l(o.m_l)
{
    m_object = new gnomonCoreParameterLookupTableObject(this);
}

gnomonCoreParameterLookupTable& gnomonCoreParameterLookupTable::operator = (const dtkCoreParameter *p)
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return *this;
    }
    return *this = p->variant();
}

gnomonCoreParameterLookupTable& gnomonCoreParameterLookupTable::operator = (const QVariant& v)
{
    if (v.canConvert<gnomonCoreParameterLookupTable>()) {
        *this = v.value<gnomonCoreParameterLookupTable>();

    } else if (v.canConvert<gnomonLookupTable>()) {
        this->setValue(v.value<gnomonLookupTable>());

    } else if (v.canConvert<QVariantHash>()) {
            auto hash = v.toHash();

            m_label = hash["label"].toString();
            m_doc = hash["doc"].toString();

            auto cmap_hash = hash["colormap"].toHash();
            QString cmap_name = cmap_hash["clut"].toString();

            gnomonColorMap cmap;
            auto keys = cmap_hash["keys"].toList();
            auto colors = cmap_hash["colors"].toList();
            int i = 0;
            for (auto key : keys) {
                cmap[key.value<double>()] = colors[i].value<QColor>();
                ++i;
            }

            auto range_hash = hash["value_range"].toHash();
            double range_min = range_hash["min"].value<double>();
            double range_max = range_hash["max"].value<double>();

            bool visible = hash["visibility"].value<bool>();

            m_l.setName(cmap_name);
            m_l.setColorMap(cmap);
            //m_l.setValueRange(QList<double>({range_min, range_max}));
            m_l.setValueMin(range_min);
            m_l.setValueMax(range_max);
            m_l.setVisibility(visible);

            m_object->notifyLabel(m_label);
            m_object->notifyDoc(m_doc);
            m_object->notifyColorMap(m_l.colorMap());
            m_object->notifyColorMapName(m_l.colorMapName());
            m_object->notifyValueRangeMin(m_l.valueMin());
            m_object->notifyValueRangeMax(m_l.valueMax());
            m_object->notifyVisibility(m_l.visibility());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterLookupTable>().name()
                  << ". Nothing is done.";
        return *this;
    }
    return *this;
}

gnomonCoreParameterLookupTable& gnomonCoreParameterLookupTable::operator = (const gnomonCoreParameterLookupTable& o)
{
    if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_l = o.m_l;
        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorMap(m_l.colorMap());
        m_object->notifyColorMapName(m_l.colorMapName());
        m_object->notifyValueRangeMin(m_l.valueMin());
        m_object->notifyValueRangeMax(m_l.valueMax());
        m_object->notifyVisibility(m_l.visibility());
    }
    return *this;
}

gnomonLookupTable gnomonCoreParameterLookupTable::value(void) const
{
    return m_l;
}

gnomonColorMap gnomonCoreParameterLookupTable::colorMap(void) const
{
    return m_l.colorMap();
}

QString gnomonCoreParameterLookupTable::name(void) const
{
    return m_l.colorMapName();
}

double gnomonCoreParameterLookupTable::valueRangeMin(void) const
{
    return m_l.valueMin();
}

QBindable<double> gnomonCoreParameterLookupTable::bindableValueRangeMin(void)
{
    return m_l.bindableValueMin();
}

double gnomonCoreParameterLookupTable::valueRangeMax(void) const
{
    return m_l.valueMax();
}

QBindable<double> gnomonCoreParameterLookupTable::bindableValueRangeMax(void)
{
    return m_l.bindableValueMax();
}

bool gnomonCoreParameterLookupTable::visibility(void) const
{
    return m_l.visibility();
}

QBindable<bool> gnomonCoreParameterLookupTable::bindableVisibility(void)
{
    return m_l.bindableVisibility();
}

QStringList gnomonCoreParameterLookupTable::availableCluts(void)
{
    QDir clut_dir(":gnomon/cluts/");
    QStringList clut_filenames = clut_dir.entryList(QStringList() << "*.clut", QDir::Files);
    QStringList cluts;
    for (auto& clut_filename : clut_filenames) {
        cluts << clut_filename.split(".clut")[0];
    }
    return cluts;
}

void gnomonCoreParameterLookupTable::setColorMap(const gnomonColorMap& c)
{
    m_l.setColorMap(c);
    m_object->notifyColorMap(c);
}

void gnomonCoreParameterLookupTable::setColorMap(const QString& clut)
{
    if (clut != m_l.colorMapName()) {
        m_l.setColorMap(clut);
        m_object->notifyColorMap(m_l.colorMap());
        m_object->notifyColorMapName(clut);
    }
}

void gnomonCoreParameterLookupTable::setValue(const QVariant& v)
{
    if (v.canConvert<gnomonCoreParameterLookupTable>()) {
        *this = v.value<gnomonCoreParameterLookupTable>();

    } else if (v.canConvert<gnomonLookupTable>()) {
        this->setValue(v.value<gnomonLookupTable>());

    } else if (v.canConvert<QVariantHash>()) {
        auto hash = v.toHash();

        m_label = hash["label"].toString();
        m_doc = hash["doc"].toString();

        auto cmap_hash = hash["colormap"].toHash();
        QString cmap_name = cmap_hash["clut"].toString();

        gnomonColorMap cmap;
        auto keys = cmap_hash["keys"].toList();
        auto colors = cmap_hash["colors"].toList();
        int i = 0;
        for (auto key : keys) {
            cmap[key.value<double>()] = colors[i].value<QColor>();
            ++i;
        }

        auto range_hash = hash["value_range"].toHash();
        double range_min = range_hash["min"].value<double>();
        double range_max = range_hash["max"].value<double>();

        bool visible = hash["visibility"].value<bool>();

        m_l.setName(cmap_name);
        m_l.setColorMap(cmap);
        //m_l.setValueRange(QList<double>({range_min, range_max}));
        m_l.setValueMin(range_min);
        m_l.setValueMax(range_max);
        m_l.setVisibility(visible);

        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorMap(m_l.colorMap());
        m_object->notifyColorMapName(m_l.colorMapName());
        m_object->notifyValueRangeMin(m_l.valueMin());
        m_object->notifyValueRangeMax(m_l.valueMax());
        m_object->notifyVisibility(m_l.visibility());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterLookupTable>().name()
                  << ". Nothing is done.";
        return;
    }
    return;
}

void gnomonCoreParameterLookupTable::setValue(const gnomonLookupTable& lut)
{
    m_l = lut;
    m_object->notifyColorMap(m_l.colorMap());
    m_object->notifyColorMapName(m_l.colorMapName());
    m_object->notifyValueRangeMin(m_l.valueMin());
    m_object->notifyValueRangeMax(m_l.valueMax());
    m_object->notifyVisibility(m_l.visibility());
}


void gnomonCoreParameterLookupTable::setName(const QString& clut)
{
    m_l.setName(clut) ;
    m_object->notifyColorMapName(clut);
}


void gnomonCoreParameterLookupTable::setValueRangeMin(double value)
{
    //m_l.setValueRange(QList<double>({value, m_l.valueRange()[1]}));
    m_l.setValueMin(value);
    m_object->notifyValueRangeMin(value);
}

void gnomonCoreParameterLookupTable::setValueRangeMax(double value)
{
    //m_l.setValueRange(QList<double>({m_l.valueRange()[0], value}));
    m_l.setValueMax(value);
    m_object->notifyValueRangeMax(value);
}

void gnomonCoreParameterLookupTable::setVisibility(bool value)
{
    m_l.setVisibility(value);
    m_object->notifyVisibility(value);
}

QVariantHash gnomonCoreParameterLookupTable::toVariantHash(void) const
{
    QVariantHash hash = dtkCoreParameterBase<gnomonCoreParameterLookupTable>::toVariantHash();

    QVariantHash cmap_hash;
    cmap_hash.insert("clut", m_l.colorMapName());

    gnomonColorMap cmap = m_l.colorMap();
    QList<QVariant> keys; keys.reserve(cmap.size());
    QList<QVariant> colors; colors.reserve(cmap.size());
    gnomonColorMap::const_iterator i = cmap.constBegin();
    while (i != cmap.constEnd()) {
        keys << QVariant::fromValue(i.key());
        colors << QVariant::fromValue(i.value());
        ++i;
    }
    cmap_hash.insert("keys", keys);
    cmap_hash.insert("colors", colors);
    hash.insert("colormap", cmap_hash);

    QVariantHash range_hash;
    range_hash.insert("min", m_l.valueMin());
    range_hash.insert("max", m_l.valueMax());
    hash.insert("value_range", range_hash);

    hash.insert("visibility", m_l.visibility());

    return hash;
}

dtkCoreParameterObject *gnomonCoreParameterLookupTable::object(void)
{
    return m_object;
}


GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream& s, const gnomonCoreParameterLookupTable& p)
{
    s << p.label();
    s << p.name();
    s << p.valueRangeMin() << p.valueRangeMax();
    s << p.visibility();
    s << p.documentation();

    return s;
}

GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream& s, gnomonCoreParameterLookupTable& p)
{
    QString label; s >> label;
    QString clut; s >> clut;
    double range_min; s >> range_min;
    double range_max; s >> range_max;
    bool visible; s >> visible;
    QString doc; s >> doc;

    gnomonLookupTable lut(clut, QList<double>({range_min, range_max}), visible);
    p = gnomonCoreParameterLookupTable(label, lut, doc);
    return s;
}

GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug dbg, gnomonCoreParameterLookupTable p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label " << p.label() << ", "
                  << "colormap_name " << p.name() << ", "
                  << "value_range [" << p.valueRangeMin() << "," << p.valueRangeMax() << "], "
                  << "visibility" << p.visibility()
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}

DTK_DEFINE_PARAMETER(gnomonCoreParameterLookupTable, g_lut);

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

void gnomonCoreParameterLookupTable::setColorMap(const gnomonLookupTable& lut)
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
                  << QMetaType::typeName(qMetaTypeId<gnomonCoreParameterLookupTable>())
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
// gnomonCoreParameterLookupTable.cpp ends here
