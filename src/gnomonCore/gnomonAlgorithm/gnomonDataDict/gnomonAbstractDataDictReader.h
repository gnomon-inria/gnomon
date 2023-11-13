#pragma once

#include <gnomonCoreExport.h>
#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
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
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonDataDictSeries> dataDict(void) = 0;

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
    virtual QString preview(void) {
        return "";
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonAbstractDataDictReader *)
DTK_DECLARE_PLUGIN(gnomonAbstractDataDictReader, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDictReader, GNOMONCORE_EXPORT, dataDictReader, gnomonCore)


//
// gnomonAbstractDataDictReader.h ends here
