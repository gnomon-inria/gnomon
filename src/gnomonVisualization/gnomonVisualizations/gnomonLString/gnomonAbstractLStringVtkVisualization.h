#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include <gnomonVisualizations/gnomonAbstractVtkVisualization.h>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>

class GNOMONVISUALIZATION_EXPORT gnomonAbstractLStringVtkVisualization : public gnomonAbstractVtkVisualization
{
    Q_OBJECT

public:
    gnomonAbstractLStringVtkVisualization(void) : gnomonAbstractVtkVisualization() {}
    virtual ~gnomonAbstractLStringVtkVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractLStringVtkVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringVtkVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringVtkVisualization, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractLStringVtkVisualization, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractLStringVtkVisualization, GNOMONVISUALIZATION_EXPORT, lStringVtkVisualization);
}

//
// gnomonAbstractLStringVtkVisualization.h ends here
