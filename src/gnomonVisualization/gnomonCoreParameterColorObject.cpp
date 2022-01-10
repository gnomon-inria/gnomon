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
}

void gnomonCoreParameterColorMapObject::notifyName(const QString& name)
{
    emit nameChanged(name);
}

//
// gnomonCoreParameterColorObject.cpp ends here
