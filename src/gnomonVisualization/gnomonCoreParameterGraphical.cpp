#include "gnomonCoreParameterGraphical.h"
#include "gnomonCoreParameterGraphicalObject.h"

gnomonCoreParameterGraphical::gnomonCoreParameterGraphical(void) : dtkCoreParameterBase<gnomonCoreParameterGraphical>()
{
    m_object = new gnomonCoreParameterGraphicalObject(this);
}

gnomonCoreParameterGraphical::~gnomonCoreParameterGraphical(void)
{
    delete m_object;
}

gnomonCoreParameterGraphical::gnomonCoreParameterGraphical(const QString& label)
{
    m_object = new gnomonCoreParameterGraphicalObject(this);
    m_label = label;
}

QString gnomonCoreParameterGraphical::name(void) const 
{
    return m_n;
}

void gnomonCoreParameterGraphical::setName(const QString &pn)
{
    m_n = pn;
    m_object->notifyName(m_n);
}

void gnomonCoreParameterGraphical::setValue(const QString &id)
{
    // TODO: depends on type of graphic element
    qDebug() << Q_FUNC_INFO <<"Not emplemented yet";
}

void gnomonCoreParameterGraphical::setValue(const QVariant &v)
{
    // TODO: depends on type of graphic element
    qDebug() << Q_FUNC_INFO <<"Not emplemented yet";
}

QVariantHash gnomonCoreParameterGraphical::toVariantHash(void) const
{
    QVariantHash hash = dtkCoreParameterBase<gnomonCoreParameterGraphical>::toVariantHash();
    return hash;
}

dtkCoreParameterObject *gnomonCoreParameterGraphical::object(void)
{
    return m_object;
}