#include "gnomonCoreParameterGrpahical.h"


gnomonCoreParameterGrpahical::gnomonCoreParameterGrpahical(void) : dtkCoreParameterBase<gnomonCoreParameterGrpahical>()
{
    m_object = new gnomonCoreParameterGrpahicalObject(this);
}

gnomonCoreParameterGrpahical::~gnomonCoreParameterGrpahical(void)
{
    delete m_object;
}

gnomonCoreParameterGrpahical::gnomonCoreParameterGrpahical(const QString& label)
{
    m_object = new gnomonCoreParameterGrpahicalObject(this);
    m_label = label;
}

QString gnomonCoreParameterGrpahical::name(void) const 
{
    return m_n;
}

void gnomonCoreParameterGrpahical::setName(QString &pn)
{
    m_n = pn;
    m_object->notifyName(m_n);
}

void gnomonCoreParameterGrpahical::setValue(QString &id)
{
    // TODO: depends on type of graphic element
}