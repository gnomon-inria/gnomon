#pragma once

#include <gnomonCoreExport>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractTreeReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractTreeReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonTreeSeries> tree(void) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonTree") {
            return {"tree"};
        }
        return {};
    };

public:
    virtual QStringList extensions(void) = 0;
    virtual QString preview(void) {
        return "";
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractTreeReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeReader, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeReader, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTreeReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractTreeReader, GNOMONCORE_EXPORT, treeReader);
}

//
// gnomonAbstractTreeReader.h ends here
