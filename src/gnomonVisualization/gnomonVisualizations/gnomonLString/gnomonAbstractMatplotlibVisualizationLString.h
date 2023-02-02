#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"


class GNOMONVISUALIZATION_EXPORT gnomonAbstractMatplotlibVisualizationLString : public gnomonAbstractMatplotlibVisualization
{
    Q_OBJECT

public:
    gnomonAbstractMatplotlibVisualizationLString(void) : gnomonAbstractMatplotlibVisualization() {}
    virtual ~gnomonAbstractMatplotlibVisualizationLString(void) = default;

public:
    virtual void setLString(std::shared_ptr<gnomonLString> lString) = 0;
    virtual std::shared_ptr<gnomonLString> lString(void) = 0;

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

DTK_DECLARE_OBJECT        (gnomonAbstractMatplotlibVisualizationLString *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractMatplotlibVisualizationLString, GNOMONVISUALIZATION_EXPORT, matplotlibVisualizationLString);
}

//
// gnomonAbstractMatplotlibVisualizationLString.h ends here
