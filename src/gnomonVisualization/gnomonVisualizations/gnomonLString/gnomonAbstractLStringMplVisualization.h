#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include "gnomonVisualizations/gnomonAbstractMplVisualization.h"


class GNOMONVISUALIZATION_EXPORT gnomonAbstractLStringMplVisualization : public gnomonAbstractMplVisualization
{
    Q_OBJECT

public:
    gnomonAbstractLStringMplVisualization(void) : gnomonAbstractMplVisualization() {}
    virtual ~gnomonAbstractLStringMplVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractLStringMplVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringMplVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringMplVisualization, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractLStringMplVisualization, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractLStringMplVisualization, GNOMONVISUALIZATION_EXPORT, lStringMplVisualization);
}

//
// gnomonAbstractLStringMplVisualization.h ends here
