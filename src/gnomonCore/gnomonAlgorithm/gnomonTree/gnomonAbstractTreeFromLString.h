#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonTree/gnomonTree.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractTreeFromLString : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractTreeFromLString(void) = default;

public:
    virtual void setInput(std::shared_ptr<gnomonLStringSeries> ) = 0;
    virtual std::shared_ptr<gnomonLStringSeries> input() = 0;

    virtual std::shared_ptr<gnomonTreeSeries> output() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"setInput"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"input"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonTree") {
            return {"output"};
        }
        return {};
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractTreeFromLString *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT, treeFromLString);
}

//
// gnomonAbstractTreeFromLString.h ends here
