#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractCellComplexConstructor : public gnomonAbstractAlgorithm
{
public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };

public:
    virtual std::shared_ptr<gnomonCellComplexSeries> output() const = 0;
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractCellComplexConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT, cellComplexConstructor);
}

//
// gnomonAbstractCellComplexConstructor.h ends here
