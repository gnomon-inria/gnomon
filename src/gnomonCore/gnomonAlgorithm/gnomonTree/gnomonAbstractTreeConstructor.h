#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonTree/gnomonTree.h"

class GNOMONCORE_EXPORT gnomonAbstractTreeConstructor : public gnomonAbstractAlgorithm
{
public:
    virtual std::shared_ptr<gnomonTreeSeries> output() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonTree") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractTreeConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractTreeConstructor, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeConstructor, GNOMONCORE_EXPORT, treeConstructor, gnomonCore)


//
// gnomonAbstractTreeConstructor.h ends here
