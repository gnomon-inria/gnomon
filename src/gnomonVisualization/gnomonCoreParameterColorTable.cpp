#include "gnomonCoreParameterColorTable.h"

#include "gnomonCoreParameterColorTableObject.h"

#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonColorTable
// ///////////////////////////////////////////////////////////////////

gnomonColorTable::gnomonColorTable(const gnomonColorTable& o) : colors(o.colors) ,textures(o.textures)
{
}

gnomonColorTable& gnomonColorTable::operator = (const gnomonColorTable& o)
{
    if(this != &o) {
        this->colors = o.colors;
        this->textures = o.textures;
    }

    return *this;
}

bool gnomonColorTable::operator != (const gnomonColorTable& o)
{
    return (this->colors == o.colors) && (this->textures == o.textures);
}

int gnomonColorTable::size(void) const
{
    return this->indices().size();
}

QList<long> gnomonColorTable::gnomonColorTable::indices(void) const
{
    QList<long> indices;
    indices.append(this->colors.keys());
    indices.append(this->textures.keys());
    std::sort(indices.begin(), indices.end());
    return indices;
}

long gnomonColorTable::indexAt(int pos) const
{
    return this->indices()[pos];
}

void gnomonColorTable::clear(void)
{
    this->colors.clear();
    this->textures.clear();
}

bool gnomonColorTable::isTexture(long index) const
{
    return this->textures.contains(index);
}

QString gnomonColorTable::textureFile(long index) const
{
    return this->textures[index];
}

bool gnomonColorTable::isColor(long index) const
{
    return this->colors.contains(index);
}

QColor gnomonColorTable::color(long index) const
{
    return this->colors[index];
}

void gnomonColorTable::setColor(long index, const QColor& color)
{
    if (this->textures.contains(index)) {
        this->textures.remove(index);
    }
    this->colors[index] = color;
}

void gnomonColorTable::setTexture(long index, const QString& texture)
{
    if (this->colors.contains(index)) {
        this->colors.remove(index);
    }
    this->textures[index] = texture;
}


// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorTable
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterColorTable::gnomonCoreParameterColorTable(void) : dtkCoreParameterBase<gnomonCoreParameterColorTable>()
{
    m_object = new gnomonCoreParameterColorTableObject(this);
}

gnomonCoreParameterColorTable::~gnomonCoreParameterColorTable(void)
{
    delete m_object;
}

gnomonCoreParameterColorTable::gnomonCoreParameterColorTable(const dtkCoreParameter *p) : dtkCoreParameterBase<gnomonCoreParameterColorTable>()
{
    m_object = new gnomonCoreParameterColorTableObject(this);
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return;
    }
    *this = p->variant();
}

gnomonCoreParameterColorTable::gnomonCoreParameterColorTable(const QString& label, const gnomonColorTable& cm, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterColorTable>()
{
    m_c = cm;
    m_object = new gnomonCoreParameterColorTableObject(this);
    m_label = label;
    m_doc = doc;
}

gnomonCoreParameterColorTable::gnomonCoreParameterColorTable(const QVariant& v) : dtkCoreParameterBase<gnomonCoreParameterColorTable>()
{
    m_object = new gnomonCoreParameterColorTableObject(this);
    if (v.canConvert<gnomonCoreParameterColorTable>()) {
        auto o(v.value<gnomonCoreParameterColorTable>());
        *this = o;

    } else if (v.canConvert<gnomonColorTable>()) {
        this->setValue(v.value<gnomonColorTable>());

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterColorTable>().name()
                  << ". Nothing is done.";
    }
}

gnomonCoreParameterColorTable::gnomonCoreParameterColorTable(const gnomonCoreParameterColorTable& o) : dtkCoreParameterBase<gnomonCoreParameterColorTable>(o), m_c(o.m_c)
{
    m_object = new gnomonCoreParameterColorTableObject(this);
}

gnomonCoreParameterColorTable& gnomonCoreParameterColorTable::operator = (const dtkCoreParameter *p)
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return *this;
    }
    return *this = p->variant();
}

gnomonCoreParameterColorTable& gnomonCoreParameterColorTable::operator = (const QVariant& v)
{
    if (v.canConvert<gnomonCoreParameterColorTable>()) {
        *this = v.value<gnomonCoreParameterColorTable>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_c.clear();

        auto keys = map["keys"].toList();
        auto colors = map["colors"].toList();
        for(int i=0; i< keys.size(); ++i) {
            if (colors[i].canConvert<QColor>()) {
                m_c.setColor(keys[i].toInt(), colors[i].value<QColor>());
            } else if (colors[i].canConvert<QString>()) {
                m_c.setTexture(keys[i].toInt(), colors[i].value<QString>());
            }
        }
        m_object->notifyColorTable(m_c);

    } else if (v.canConvert<gnomonColorTable>()) {
        this->setValue(v.value<gnomonColorTable>());

    } else if (v.canConvert<QVariantHash>()) {
        auto hash = v.toHash();

        m_label = hash["label"].toString();
        m_doc = hash["doc"].toString();

        gnomonColorTable cmap;
        auto keys = hash["keys"].toList();
        auto colors = hash["colors"].toList();
        int i = 0;
        for (auto key : keys) {
            if (colors[i].canConvert<QColor>()) {
                m_c.setColor(key.value<long>(), colors[i].value<QColor>());
            } else if (colors[i].canConvert<QString>()) {
                m_c.setTexture(key.value<long>(), colors[i].value<QString>());
            }
            ++i;
        }

        m_c = cmap;

        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorTable(m_c);

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterColorTable>().name()
                  << ". Nothing is done.";
        return *this;
    }
    return *this;
}

gnomonCoreParameterColorTable& gnomonCoreParameterColorTable::operator = (const gnomonCoreParameterColorTable& o)
{
  if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_c = o.m_c;
        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorTable(m_c);
    }
    return *this;
}

gnomonColorTable gnomonCoreParameterColorTable::value(void) const
{
    return m_c;
}

void gnomonCoreParameterColorTable::setValue(const gnomonColorTable& c)
{
    m_c = c;
    m_object->notifyColorTable(m_c);
}

void gnomonCoreParameterColorTable::setValue(const QVariant& v)
{
    if (v.canConvert<gnomonCoreParameterColorTable>()) {
        *this = v.value<gnomonCoreParameterColorTable>();

    }
     else if (v.canConvert<QVariantHash>()) {
         auto map = v.toHash();

         this->m_c.clear();

         auto keys = map["keys"].toList();
         auto colors = map["colors"].toList();
         for(int i=0; i< keys.size(); ++i) {
             if (colors[i].canConvert<QColor>()) {
                 m_c.setColor(keys[i].toInt(), colors[i].value<QColor>());
             } else if (colors[i].canConvert<QString>()) {
                 m_c.setTexture(keys[i].toInt(), colors[i].value<QString>());
             }
         }
         m_object->notifyColorTable(m_c);

    }
    else if (v.canConvert<gnomonColorTable>()) {
        this->setValue(v.value<gnomonColorTable>());

    } else if (v.canConvert<QVariantHash>()) {
        auto hash = v.toHash();

        m_label = hash["label"].toString();
        m_doc = hash["doc"].toString();

        gnomonColorTable cmap;
        auto keys = hash["keys"].toList();
        auto colors = hash["colors"].toList();
        int i = 0;
        for (auto key : keys) {
            if (colors[i].canConvert<QColor>()) {
                m_c.setColor(key.value<long>(), colors[i].value<QColor>());
            } else if (colors[i].canConvert<QString>()) {
                m_c.setTexture(key.value<long>(), colors[i].value<QString>());
            }
            ++i;
        }
        m_c = cmap;

        m_object->notifyLabel(m_label);
        m_object->notifyDoc(m_doc);
        m_object->notifyColorTable(m_c);

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterColorTable>().name()
                  << ". Nothing is done.";
        return;
    }
    return;
}

QColor gnomonCoreParameterColorTable::color(long i) const
{
    if (m_c.isColor(i)) {
        return m_c.color(i);
    } else {
        return QColor();
    }
}

void gnomonCoreParameterColorTable::setColor(long i, const QColor& color) {
    m_c.setColor(i, color);
    this->sync();
}

QString gnomonCoreParameterColorTable::texture(long i) const
{
    if (m_c.isTexture(i)) {
        return m_c.textureFile(i);
    } else {
        return QString();
    }
}

void gnomonCoreParameterColorTable::setTexture(long i, const QString& texture) {
    m_c.setTexture(i, texture);
    this->sync();
}

void gnomonCoreParameterColorTable::clearColors(void)
{
    m_c.clear();
}

int gnomonCoreParameterColorTable::colorIndexCount(void) const
{
    return m_c.size();
}

QList<long> gnomonCoreParameterColorTable::colorIndices(void) const
{
    return m_c.indices();
}

long gnomonCoreParameterColorTable::colorIndexAt(int index) const
{
    if (index < m_c.size()) {
        return m_c.indices()[index];
    } else {
        return -1;
    }
}

QVariantHash gnomonCoreParameterColorTable::toVariantHash(void) const
{
    QVariantHash hash = dtkCoreParameterBase<gnomonCoreParameterColorTable>::toVariantHash();

    QList<QVariant> keys; keys.reserve(m_c.size());
    QList<QVariant> colors; colors.reserve(m_c.size());
    for (auto i : m_c.indices())
    {
        keys << QVariant::fromValue(i);
        if (m_c.isColor(i)) {
            colors << QVariant::fromValue(m_c.color(i));
        } else if (m_c.isTexture(i)) {
            colors << QVariant::fromValue(m_c.textureFile(i));
        }
        ++i;
    }
    hash.insert("keys", keys);
    hash.insert("colors", colors);

    return hash;
}

dtkCoreParameterObject *gnomonCoreParameterColorTable::object(void)
{
    return m_object;
}


GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream& s, const gnomonCoreParameterColorTable& p)
{
    s << p.label();
    s << p.documentation();

    return s;
}

GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream& s, gnomonCoreParameterColorTable& p)
{
    QString label; s >> label;
    QString doc; s >> doc;

    p = gnomonCoreParameterColorTable();
    return s;
}

GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug dbg, gnomonCoreParameterColorTable p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label " << p.label() << ", "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}


DTK_DEFINE_PARAMETER(gnomonCoreParameterColorTable, g_ctable);
