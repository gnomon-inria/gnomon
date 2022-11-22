#include "gnomonCoreParameterColorTableObject.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorObject implementation
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterColorTableObject::gnomonCoreParameterColorTableObject(gnomonCoreParameterColorTable *p) : dtkCoreParameterObject(p), m_param(p)
{

}

gnomonCoreParameterColorTableObject::~gnomonCoreParameterColorTableObject(void)
{
    m_param = nullptr;
}

void gnomonCoreParameterColorTableObject::setColorTable(const gnomonColorTable& map)
{
    m_param->setValue(map);
}

gnomonColorTable gnomonCoreParameterColorTableObject::colorTable(void) const
{
    return m_param->value();
}

void gnomonCoreParameterColorTableObject::setValue(const QVariantMap& color_map)
{
    gnomonColorTable map;
    for (auto it = color_map.begin(); it != color_map.end(); ++it) {
        map[it.key().toDouble()] = it.value().value<QColor>();
    }
    m_param->setValue(map);
}

QVariantMap gnomonCoreParameterColorTableObject::value(void) const
{
    gnomonColorTable map = m_param->value();
    QVariantMap color_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        color_map[QString::number(it.key())] = QVariant(it.value());
    }
    return color_map;
}

gnomonCoreParameterColorTable *gnomonCoreParameterColorTableObject::parameter(void)
{
    return m_param;
}

QColor gnomonCoreParameterColorTableObject::color(long i) const
{
    return m_param->color(i);
}

void gnomonCoreParameterColorTableObject::setColor(long i, const QColor& color)
{
    m_param->setColor(i, color);
}

void gnomonCoreParameterColorTableObject::notifyColorTable(const gnomonColorTable& map)
{
    emit colorTableChanged(map);
    QVariantMap color_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        color_map[QString::number(it.key())] = QVariant(it.value());
    }
    emit valueChanged(color_map);
}

//
// gnomonCoreParameterColorObject.cpp ends here
