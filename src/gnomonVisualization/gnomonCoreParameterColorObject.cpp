// gnomonCoreParameterColorObject.cpp
//

#include "gnomonCoreParameterColorObject.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorObject implementation
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterColorMapObject::gnomonCoreParameterColorMapObject(gnomonCoreParameterColorMap *p) : dtkCoreParameterObject(p), m_param(p)
{

}

gnomonCoreParameterColorMapObject::~gnomonCoreParameterColorMapObject(void)
{
    m_param = nullptr;
}

void gnomonCoreParameterColorMapObject::setColorMap(const QMap<double, QColor>& map)
{
    m_param->setValue(map);
}

QMap<double, QColor> gnomonCoreParameterColorMapObject::colorMap(void) const
{
    return m_param->value();
}

void gnomonCoreParameterColorMapObject::setIdentifier(const QString& id)
{
    m_param->setValue(id);
}

QString gnomonCoreParameterColorMapObject::identifier(void) const
{
    return m_param->name();
}

void gnomonCoreParameterColorMapObject::setValue(const QVariantMap& color_map)
{
    QMap<double, QColor> map;
    for (auto it = color_map.begin(); it != color_map.end(); ++it) {
        map[it.key().toDouble()] = it.value().value<QColor>();
    }
    m_param->setValue(map);
}

QVariantMap gnomonCoreParameterColorMapObject::value(void) const
{
    QMap<double, QColor> map = m_param->value();
    QVariantMap color_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        color_map[QString::number(it.key())] = QVariant(it.value());
    }
    return color_map;
}

void gnomonCoreParameterColorMapObject::setName(const QString& name)
{
    m_param->setName(name);
}

QString gnomonCoreParameterColorMapObject::name(void) const
{
    return m_param->name();
}

QStringList gnomonCoreParameterColorMapObject::availableCluts() const
{
    return m_param->availableCluts();
}

gnomonCoreParameterColorMap *gnomonCoreParameterColorMapObject::parameter(void)
{
    return m_param;
}

void gnomonCoreParameterColorMapObject::notifyColorMap(const QMap<double, QColor>& map)
{
    emit colorMapChanged(map);
    QVariantMap color_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        color_map[QString::number(it.key())] = QVariant(it.value());
    }
    emit valueChanged(color_map);
}

void gnomonCoreParameterColorMapObject::notifyName(const QString& name)
{
    emit nameChanged(name);
}

//
// gnomonCoreParameterColorObject.cpp ends here
