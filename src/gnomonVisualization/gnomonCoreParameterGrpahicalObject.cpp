// gnomonCoreParameterGrpahicalObject.cpp
//

#include "gnomonCoreParameterGrpahicalObject.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterGrpahicalObject implementation
// ///////////////////////////////////////////////////////////////////


gnomonCoreParameterGrpahicalObject::gnomonCoreParameterGrpahicalObject(void) : dtkCoreParameterObject<dtkCoreParameterObject>()
{
    gnomonCoreParameterGrpahical *m_param = new gnomonCoreParameterGrpahical();
}

gnomonCoreParameterGrpahicalObject::~gnomonCoreParameterGrpahicalObject(void)
{
    m_param = nullptr;
}

void gnomonCoreParameterGrpahicalObject::setIdentifier(const QString& id)
{
    m_param->setValue(id);
}

QString gnomonCoreParameterGrpahicalObject::identifier(void) const
{
    return m_param->name();
}

void gnomonCoreParameterGrpahicalObject::notifyName(const QString& name)
{
    emit nameChanged(name);
}

//
// gnomonCoreParameterGrpahicalObject.cpp ends here