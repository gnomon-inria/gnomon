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

#include "gnomonLandmark.h"

#include "gnomonAbstractAlgorithm.h"

class gnomonImagesSerie;
class gnomonCoreParameter;


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImagesFusion : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractImagesFusion(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;

public:
    virtual void addImagesSerie(gnomonImagesSerie *) = 0;
    virtual void removeImagesSeries(void) = 0;

    virtual void addLandmarks(const std::vector<gnomonLandmark>&) = 0;
    virtual void removeLandmarks(void) = 0;

public:
    virtual gnomonImagesSerie *output() = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImagesFusion *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImagesFusion, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImagesFusion, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImagesFusion, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImagesFusion, GNOMONCORE_EXPORT, imagesFusion);
}

//
// gnomonAbstractImagesFusion.h ends here
