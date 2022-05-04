#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonTree/gnomonTree.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractTreeWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractTreeWriter(void) = default;

public:
    virtual void setTree(std::shared_ptr<gnomonTreeSeries> tree) = 0;

public:
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
    static inline QString defaultOutput(QString formName) {
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractTreeWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTreeWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractTreeWriter, GNOMONCORE_EXPORT, treeWriter);
}

//
// gnomonAbstractImageWriter.h ends here
