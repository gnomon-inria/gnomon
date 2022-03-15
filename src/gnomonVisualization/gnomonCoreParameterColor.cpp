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

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QString& label, const gnomonColorMap& cm, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterColorMap>(), m_c(cm)
{
    m_object = new gnomonCoreParameterColorMapObject(this);
    m_label = label;
    m_doc = doc;
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QString& label, const QString& cm_id, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterColorMap>()
{
    m_object = new gnomonCoreParameterColorMapObject(this);
    m_label = label;
    m_doc = doc;
    this->setValue(cm_id);
}

gnomonCoreParameterColorMap::gnomonCoreParameterColorMap(const QVariant& v) : dtkCoreParameterBase<gnomonCoreParameterColorMap>()
{
    m_object = new gnomonCoreParameterColorMapObject(this);
    if (v.canConvert<gnomonCoreParameterColorMap>()) {
        auto o(v.value<gnomonCoreParameterColorMap>());
        *this = o;

    } else if (v.canConvert<gnomonColorMap>()) {
        this->setValue(v.value<gnomonColorMap>());

    } else if (v.canConvert<QString>()) {
        this->setValue(v.toString());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterColorMap>().name()
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

    } else if (v.canConvert<gnomonColorMap>()) {
        this->setValue(v.value<gnomonColorMap>());

    } else if (v.canConvert<QString>()) {
        this->setValue(v.toString());

    } else if (v.canConvert<QVariantHash>()) {
        auto hash = v.toHash();

        m_label = hash["label"].toString();
        m_doc = hash["doc"].toString();

        QString cmap_name = hash["clut"].toString();

        gnomonColorMap cmap;
        auto keys = hash["keys"].toList();
        auto colors = hash["colors"].toList();
        int i = 0;
        for (auto key : keys) {
            cmap[key.value<double>()] = colors[i].value<QColor>();
            ++i;
        }

        m_n = cmap_name;
        m_c = cmap;

        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorMap(m_c);
        m_object->notifyName(m_n);

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterColorMap>().name()
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

gnomonColorMap gnomonCoreParameterColorMap::value(void) const
{
    return m_c;
}

QString gnomonCoreParameterColorMap::name(void) const
{
    return m_n;
}

QStringList gnomonCoreParameterColorMap::availableCluts(void)
{
    QDir clut_dir(":gnomon/cluts/");
    QStringList clut_filenames = clut_dir.entryList(QStringList() << "*.clut", QDir::Files);
    QStringList cluts;
    for (auto& clut_filename : clut_filenames) {
        cluts << clut_filename.split(".clut")[0];
    }
    return cluts;
}

void gnomonCoreParameterColorMap::setValue(const gnomonColorMap& c)
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

    gnomonColorMap colormap;

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

    } else if (v.canConvert<gnomonColorMap>()) {
        this->setValue(v.value<gnomonColorMap>());

    } else if (v.canConvert<QString>()) {
        this->setValue(v.toString());

    } else if (v.canConvert<QVariantHash>()) {
        auto hash = v.toHash();

        m_label = hash["label"].toString();
        m_doc = hash["doc"].toString();

        QString cmap_name = hash["clut"].toString();

        gnomonColorMap cmap;
        auto keys = hash["keys"].toList();
        auto colors = hash["colors"].toList();
        int i = 0;
        for (auto key : keys) {
            cmap[key.value<double>()] = colors[i].value<QColor>();
            ++i;
        }

        m_n = cmap_name;
        m_c = cmap;

        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorMap(m_c);
        m_object->notifyName(m_n);

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterColorMap>().name()
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
    gnomonColorMap::const_iterator i = m_c.constBegin();
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


GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream& s, const gnomonCoreParameterColorMap& p)
{
    s << p.label();
    s << p.name();
    s << p.documentation();

    return s;
}

GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream& s, gnomonCoreParameterColorMap& p)
{
    QString label; s >> label;
    QString clut; s >> clut;
    QString doc; s >> doc;

    p = gnomonCoreParameterColorMap(label, clut, doc);
    return s;
}

GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug dbg, gnomonCoreParameterColorMap p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label " << p.label() << ", "
                  << "colormap_name " << p.name() << ", "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}


DTK_DEFINE_PARAMETER(gnomonCoreParameterColorMap, g_cmap);

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
