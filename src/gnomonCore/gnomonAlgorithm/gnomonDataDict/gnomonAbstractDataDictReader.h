#pragma once

#include <gnomonCoreExport.h> 
#include <dtkCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractDataDictReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractDataDictReader(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual gnomonDataDictSeries *DataDict(void) = 0;

    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonDataDict") {
            return {"dataDict"};
        }
        return {};
    };

public:
    virtual QStringList extensions(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonAbstractDataDictReader *)
DTK_DECLARE_PLUGIN(gnomonAbstractDataDictReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDictReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDataDictReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractDataDictReader, GNOMONCORE_EXPORT, dataDictReader);
}

//
// gnomonAbstractDataDictReader.h ends here
