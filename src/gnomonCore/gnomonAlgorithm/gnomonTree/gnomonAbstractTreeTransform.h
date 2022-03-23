#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonTree/gnomonTree.h"

class dtkCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractTreeTransform : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractTreeTransform(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInput(gnomonTreeSeries *) = 0;
    virtual gnomonTreeSeries *input() = 0;

    virtual gnomonTreeSeries *output() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonTree") {
            return {"setInput"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonTree") {
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

DTK_DECLARE_OBJECT        (gnomonAbstractTreeTransform *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeTransform, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeTransform, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTreeTransform, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractTreeTransform, GNOMONCORE_EXPORT, treeTransform);
}

//
// gnomonAbstractTreeTransform.h ends here
