#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include "gnomonVisualizations/gnomonAbstractQmlVisualization.h"


class GNOMONVISUALIZATION_EXPORT gnomonAbstractLStringQmlVisualization : public gnomonAbstractQmlVisualization
{
    Q_OBJECT

public:
    gnomonAbstractLStringQmlVisualization(void) : gnomonAbstractQmlVisualization() {}
    virtual ~gnomonAbstractLStringQmlVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractLStringQmlVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringQmlVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringQmlVisualization, GNOMONVISUALIZATION_EXPORT, lStringQmlVisualization, gnomonVisualization)


//
// gnomonAbstractLStringQmlVisualization.h ends here
