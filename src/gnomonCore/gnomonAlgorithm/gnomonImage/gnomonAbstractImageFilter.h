#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"

class dtkCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageFilter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractImageFilter(void) = default;

public:
    virtual void setInput(std::shared_ptr<gnomonImageSeries> image_series) = 0;
    virtual std::shared_ptr<gnomonImageSeries> input() = 0;

    virtual inline void setMask(std::shared_ptr<gnomonBinaryImageSeries> mask) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };
    virtual inline std::shared_ptr<gnomonBinaryImageSeries> mask() {
        dtkWarn()<<Q_FUNC_INFO <<"Not implemented";
        return nullptr;
    };

    virtual std::shared_ptr<gnomonImageSeries> output() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setInput"};
        } else if(formName == "gnomonBinaryImage") {
            return {"setMask"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"input"};
        } else if(formName == "gnomonBinaryImage") {
            return {"mask"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonImage") {
            return {"output"};
        }
        return {};
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageFilter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageFilter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageFilter, GNOMONCORE_EXPORT, imageFilter, gnomonCore)


//
// gnomonAbstractImageFilter.h ends here
