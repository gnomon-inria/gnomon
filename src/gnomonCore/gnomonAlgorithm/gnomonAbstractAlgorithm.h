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

#include <QtCore>
#include <dtkCore>

class gnomonCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractAlgorithm : public QRunnable
{
public:
             gnomonAbstractAlgorithm(void) = default;
    virtual ~gnomonAbstractAlgorithm(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const = 0;

public:
    virtual void run(void) = 0;
    virtual QString documentation(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractAlgorithm *)
DTK_DECLARE_PLUGIN        (gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT, algorithm);
}

//
// gnomonAbstractAlgorithm.h ends here
