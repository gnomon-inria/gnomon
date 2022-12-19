#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>
#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractMatplotlibVisualizationTree : public gnomonAbstractMatplotlibVisualization
{
    Q_OBJECT

public:
    gnomonAbstractMatplotlibVisualizationTree(void) : gnomonAbstractMatplotlibVisualization() {}
    virtual ~gnomonAbstractMatplotlibVisualizationTree(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractMatplotlibVisualizationTree *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMatplotlibVisualizationTree, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMatplotlibVisualizationTree, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMatplotlibVisualizationTree, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractMatplotlibVisualizationTree, GNOMONVISUALIZATION_EXPORT, matplotlibVisualizationTree);
}

//
// gnomonAbstractMatplotlibVisualizationTree.h ends here
