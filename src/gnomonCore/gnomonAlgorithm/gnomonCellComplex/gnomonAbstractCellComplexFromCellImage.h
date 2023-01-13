#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"

class GNOMONCORE_EXPORT gnomonAbstractCellComplexFromCellImage : public gnomonAbstractAlgorithm
{
public:
    virtual void setInput(std::shared_ptr<gnomonCellImageSeries> cellimage) = 0;

public:
    virtual std::shared_ptr<gnomonCellImageSeries> input(void) = 0;
    virtual std::shared_ptr<gnomonCellComplexSeries> output(void) const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"setInput"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"input"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"output"};
        }
        return {};
    };

};

DTK_DECLARE_OBJECT(gnomonAbstractCellComplexFromCellImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT, cellComplexFromCellImage);
}

//
// gnomonAbstractCellComplexFromCellImage.h ends here
