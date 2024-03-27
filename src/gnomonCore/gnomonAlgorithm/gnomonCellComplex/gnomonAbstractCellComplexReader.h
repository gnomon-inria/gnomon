#pragma once

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractCellComplexReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractCellComplexReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonCellComplexSeries> cellComplex(void) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"cellComplex"};
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

DTK_DECLARE_OBJECT(gnomonAbstractCellComplexReader *)
DTK_DECLARE_PLUGIN(gnomonAbstractCellComplexReader, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexReader, GNOMONCORE_EXPORT, cellComplexReader, gnomonCore)


//
// gnomonAbstractCellComplexReader.h ends here
