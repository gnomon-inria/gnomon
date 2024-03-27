// gnomonCoreParameterLookupTableObject.cpp
//

#include "gnomonCoreParameterLookupTableObject.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterLookupTableObject implementation
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterLookupTableObject::gnomonCoreParameterLookupTableObject(gnomonCoreParameterLookupTable *p) : dtkCoreParameterObject(p), m_param(p)
{

}

gnomonCoreParameterLookupTableObject::~gnomonCoreParameterLookupTableObject(void)
{
    m_param = nullptr;
}

void gnomonCoreParameterLookupTableObject::setColorMap(const QVariantMap& color_map)
{
    gnomonColorMap map;
    for (auto it = color_map.begin(); it != color_map.end(); ++it) {
        map[it.key().toDouble()] = it.value().value<QColor>();
    }
    m_param->setColorMap(map);
}

QVariantMap gnomonCoreParameterLookupTableObject::colorMap(void) const
{
    gnomonColorMap map = m_param->colorMap();
    QVariantMap color_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        color_map[QString::number(it.key())] = QVariant(it.value());
    }
    return color_map;
}


QVariantMap gnomonCoreParameterLookupTableObject::getColorMap(const QString &clut = "viridis") {
    gnomonColorMap map = m_param->getColorMap(clut);
    QVariantMap color_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        color_map[QString::number(it.key())] = QVariant(it.value());
    }
    return color_map;
}

void gnomonCoreParameterLookupTableObject::setColorMapName(const QString &id)
{
    m_param->setColorMap(id);
}

QString gnomonCoreParameterLookupTableObject::colorMapName(void) const
{
    return m_param->name();
}

void gnomonCoreParameterLookupTableObject::setValueMin(double value)
{
    m_param->setValueMin(value);
}

double gnomonCoreParameterLookupTableObject::valueMin(void) const
{
    return m_param->valueMin();
}

void gnomonCoreParameterLookupTableObject::setValueMax(double value)
{
    m_param->setValueMax(value);
}

double gnomonCoreParameterLookupTableObject::valueMax(void) const
{
    return m_param->valueMax();
}

void gnomonCoreParameterLookupTableObject::setRangeMin(double value)
{
    m_param->setRangeMin(value);
}

double gnomonCoreParameterLookupTableObject::rangeMin(void) const
{
    return m_param->rangeMin();
}

void gnomonCoreParameterLookupTableObject::setRangeMax(double value)
{
    m_param->setRangeMax(value);
}

double gnomonCoreParameterLookupTableObject::rangeMax(void) const
{
    return m_param->rangeMax();
}

void gnomonCoreParameterLookupTableObject::setVisibility(bool value)
{
    m_param->setVisibility(value);
}

bool gnomonCoreParameterLookupTableObject::visibility(void) const
{
    return m_param->visibility();
}

QStringList gnomonCoreParameterLookupTableObject::availableCluts() const
{
    return m_param->availableCluts();
}

gnomonCoreParameterLookupTable *gnomonCoreParameterLookupTableObject::parameter(void)
{
    return m_param;
}

void gnomonCoreParameterLookupTableObject::notifyColorMap(const gnomonColorMap& map)
{
    QVariantMap color_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        color_map[QString::number(it.key())] = QVariant(it.value());
    }
    emit colorMapChanged(color_map);
    emit valueChanged(m_param->value());
}

void gnomonCoreParameterLookupTableObject::notifyColorMapName(const QString &name)
{
    emit colorMapNameChanged(name);
    emit valueChanged(m_param->value());
}

void gnomonCoreParameterLookupTableObject::notifyValueMin(double value)
{
    emit valueMinChanged(value);
    emit valueChanged(m_param->value());
}

void gnomonCoreParameterLookupTableObject::notifyValueMax(double value)
{
    emit valueMaxChanged(value);
    emit valueChanged(m_param->value());
}

void gnomonCoreParameterLookupTableObject::notifyRangeMin(double value)
{
    emit rangeMinChanged(value);
    emit valueChanged(m_param->value());
}

void gnomonCoreParameterLookupTableObject::notifyRangeMax(double value)
{
    emit rangeMaxChanged(value);
    emit valueChanged(m_param->value());
}

void gnomonCoreParameterLookupTableObject::notifyVisibility(bool value)
{
    emit visibilityChanged(value);
    emit valueChanged(m_param->value());
}

//
// gnomonCoreParameterLookupTableObject.cpp ends here
