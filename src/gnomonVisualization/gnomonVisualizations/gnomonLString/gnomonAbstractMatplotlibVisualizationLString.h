// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore>

#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"

class gnomonViewMatplotlib;

class gnomonLString;
class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractMatplotlibVisualizationLString : public gnomonAbstractMatplotlibVisualization
{
    Q_OBJECT

public:
    gnomonAbstractMatplotlibVisualizationLString(void) : gnomonAbstractMatplotlibVisualization() {}
    virtual ~gnomonAbstractMatplotlibVisualizationLString(void) = default;

public:
	virtual void setLString(gnomonLString *lString) = 0;
	virtual gnomonLString *lString(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"setLString"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"lString"};
        }
        return {};
    };

public:
    virtual void setParameter(const QString&, const QVariant&) override = 0;
    virtual void setParameters(const dtkCoreParameters&) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;

public:
    virtual QImage imageRendering(void) override = 0;

public slots:
    virtual void update(void) override = 0;
    virtual void render(void) override = 0;
    virtual void clear(void) override = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMatplotlibVisualizationLString *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT, matplotlibVisualizationLString);
}

//
// gnomonAbstractMatplotlibVisualizationLString.h ends here