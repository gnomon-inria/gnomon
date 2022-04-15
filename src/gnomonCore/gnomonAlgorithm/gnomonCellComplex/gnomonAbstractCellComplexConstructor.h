#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractCellComplexConstructor : public gnomonAbstractAlgorithm
{

    //Inputs
public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;

    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };



    // Outputs
public:
    virtual gnomonCellComplexSeries *output() const = 0;
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"output"};
        }
        return {};
    };

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellComplexConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellComplexConstructor, GNOMONCORE_EXPORT, cellComplexConstructor);
}

//
// gnomonAbstractCellComplexConstructor.h ends here
