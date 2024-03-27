#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"

class gnomonDataFrame;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractDataFrameReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractDataFrameReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonDataFrameSeries> dataFrame(void) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonDataFrame") {
            return {"dataFrame"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractDataFrameReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataFrameReader, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataFrameReader, GNOMONCORE_EXPORT, dataFrameReader, gnomonCore)


//
// gnomonAbstractDataFrameReader.h ends here
