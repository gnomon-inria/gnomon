#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractDataFrameWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractDataFrameWriter(void) = default;

public:
    virtual void setDataFrame(std::shared_ptr<gnomonDataFrameSeries> DataFrame) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonDataFrame") {
            return {"setDataFrame"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonDataFrame") {
            return {"dataFrame"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractDataFrameWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataFrameWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataFrameWriter, GNOMONCORE_EXPORT, dataFrameWriter, gnomonCore)


//
// gnomonAbstractImageWriter.h ends here
