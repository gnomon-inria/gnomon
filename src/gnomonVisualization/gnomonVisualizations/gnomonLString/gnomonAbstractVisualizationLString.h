#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include <gnomonVisualizations/gnomonAbstractVisualization.h>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationLString : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationLString(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationLString(void) = default;

public:
	virtual void setLString(std::shared_ptr<gnomonLStringSeries> lString) = 0;
	virtual std::shared_ptr<gnomonLStringSeries> lString(void) = 0;

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
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationLString *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationLString, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationLString, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationLString, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationLString, GNOMONVISUALIZATION_EXPORT, visualizationLString);
}

//
// gnomonAbstractVisualizationLString.h ends here
