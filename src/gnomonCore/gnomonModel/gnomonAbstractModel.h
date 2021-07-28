// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonCoreExport.h>

#include <dtkCore>

class dtkCoreParameter;

class GNOMONCORE_EXPORT gnomonAbstractModel
{
public:
    virtual ~gnomonAbstractModel(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;
    virtual QMap<QString, dtkCoreParameter *> parameters(void) const = 0;
};

DTK_DECLARE_OBJECT        (gnomonAbstractModel *)
DTK_DECLARE_PLUGIN        (gnomonAbstractModel, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractModel, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractModel, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractModel, GNOMONCORE_EXPORT, abstractModel);
}

//
// gnomonAbstractModel.h ends here
