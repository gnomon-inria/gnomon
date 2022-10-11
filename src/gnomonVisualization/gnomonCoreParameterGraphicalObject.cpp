// gnomonCoreParameterGraphicalObject.cpp
//

#include "gnomonCoreParameterGraphicalObject.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterGraphicalObject implementation
// ///////////////////////////////////////////////////////////////////


gnomonCoreParameterGraphicalObject::gnomonCoreParameterGraphicalObject(gnomonCoreParameterGraphical *p) : dtkCoreParameterObject(p), m_param(p) 
{
    
}

gnomonCoreParameterGraphicalObject::~gnomonCoreParameterGraphicalObject(void)
{
    m_param = nullptr;
}

void gnomonCoreParameterGraphicalObject::setIdentifier(const QString& id)
{
    m_param->setValue(id);
}

QString gnomonCoreParameterGraphicalObject::identifier(void) const
{
    return m_param->name();
}

void gnomonCoreParameterGraphicalObject::notifyName(const QString& name)
{
    emit nameChanged(name);
}

//
// gnomonCoreParameterGraphicalObject.cpp ends here