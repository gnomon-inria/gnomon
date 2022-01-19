#pragma once

#include <gnomonCoreExport.h> 
#include <dtkCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractDataDictWriter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractDataDictWriter(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setDataDict(gnomonDataDictSeries *datadict) = 0;
    virtual void setPath(const QString& path) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonDataDict") {
            return {"setDataDict"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonDataDict") {
            return {"dataDict"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        return {};
    };

public:
    virtual QStringList extensions(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonAbstractDataDictWriter *)
DTK_DECLARE_PLUGIN(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT, dataDictWriter);
}

//
// gnomonAbstractDataDictWriter.h ends here
