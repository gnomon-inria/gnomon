#pragma once

#include <gnomonCoreExport.h>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

class dtkCoreParameter;

class GNOMONCORE_EXPORT gnomonAbstractModel
{
public:
    virtual ~gnomonAbstractModel(void) = default;

public:
    virtual void reset(void) = 0;
    virtual void step(double time, double dt) = 0;
    virtual void run(double timeMin, double timeMax, double dt) = 0;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;
    virtual dtkCoreParameters parameters(void) const = 0;
    virtual QMap<QString, QString> parameterGroups(void) { return QMap<QString, QString>(); };
};

DTK_DECLARE_OBJECT        (gnomonAbstractModel *)
DTK_DECLARE_PLUGIN        (gnomonAbstractModel, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractModel, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractModel, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractModel, GNOMONCORE_EXPORT, abstractModel);
}

//
// gnomonAbstractModel.h ends here
