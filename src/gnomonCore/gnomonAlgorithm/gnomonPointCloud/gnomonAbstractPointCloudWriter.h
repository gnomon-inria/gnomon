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

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractPointCloudWriter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractPointCloudWriter(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setPointCloud(gnomonPointCloudSeries *pointCloud) = 0;
    virtual void setPath(const QString& path) = 0;

public:
    virtual QStringList extensions(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractPointCloudWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractPointCloudWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractPointCloudWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractPointCloudWriter, GNOMONCORE_EXPORT, pointCloudWriter);
}

//
// gnomonAbstractImageWriter.h ends here
