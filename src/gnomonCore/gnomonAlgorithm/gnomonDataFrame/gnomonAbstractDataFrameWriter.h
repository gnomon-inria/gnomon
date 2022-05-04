#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

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
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataFrameWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDataFrameWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractDataFrameWriter, GNOMONCORE_EXPORT, dataFrameWriter);
}

//
// gnomonAbstractImageWriter.h ends here
