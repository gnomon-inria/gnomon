#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>
#include "gnomonVisualizations/gnomonAbstractMplVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractTreeMplVisualization : public gnomonAbstractMplVisualization
{
    Q_OBJECT

public:
    gnomonAbstractTreeMplVisualization(void) : gnomonAbstractMplVisualization() {}
    virtual ~gnomonAbstractTreeMplVisualization(void) = default;

public:
    virtual void setTree(std::shared_ptr<gnomonTree> tree) = 0;
    virtual std::shared_ptr<gnomonTree> tree(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonTree") {
            return {"setTree"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonTree") {
            return {"tree"};
        }
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractTreeMplVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeMplVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeMplVisualization, GNOMONVISUALIZATION_EXPORT, treeMplVisualization, gnomonVisualization)


//
// gnomonAbstractTreeMplVisualization.h ends here
