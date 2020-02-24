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

#include "gnomonForm/gnomonImage/gnomonImage.h"

class gnomonImage;


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractImageReader(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;
	virtual QString documentation(void) override = 0;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual gnomonImageSeries *image() = 0;

public:
    virtual QStringList extensions(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageReader, GNOMONCORE_EXPORT, imageReader);
}

//
// gnomonAbstractImageReader.h ends here
