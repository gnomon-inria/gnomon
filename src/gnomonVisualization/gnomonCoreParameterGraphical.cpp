#include "gnomonCoreParameterGraphical.h"
#include "gnomonCoreParameterGraphicalObject.h"
#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterGraphical
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterGraphical::gnomonCoreParameterGraphical(void) : dtkCoreParameterBase<gnomonCoreParameterGraphical>()
{
    m_object = new gnomonCoreParameterGraphicalObject(this);
}

gnomonCoreParameterGraphical::gnomonCoreParameterGraphical(const dtkCoreParameter *p):  dtkCoreParameterBase<gnomonCoreParameterGraphical>()
{
    m_object = new gnomonCoreParameterGraphicalObject(this);
    if(!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return;
    }
    // *this = p->variant();
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
    qDebug() << Q_FUNC_INFO <<"Not implemented yet";
}

void gnomonCoreParameterGraphical::setValue(const QVariant &v)
{
    // TODO: depends on type of graphic element
    qDebug() << Q_FUNC_INFO <<"Not implemented yet";
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


GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream& s, const gnomonCoreParameterGraphical& p)
{
    s << p.label();
    s << p.name();
    s << p.documentation();

    return s;
}

GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream& s, gnomonCoreParameterGraphical& p)
{
    QString label; s >> label;

    p = gnomonCoreParameterGraphical(label);
    return s;
}

GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug dbg, gnomonCoreParameterGraphical p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label " << p.label() << ", "
                  << "graphical_parameter_name " << p.name() << ", "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}


DTK_DEFINE_PARAMETER(gnomonCoreParameterGraphical, g_gparam);


//
// gnomonCoreParameterGraphical.cpp ends here
