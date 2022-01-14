// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

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
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInput(gnomonImageSeries *image_series) = 0;
    virtual gnomonImageSeries *input() = 0;

    virtual inline void setMask(gnomonBinaryImageSeries *mask) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };
    virtual inline gnomonBinaryImageSeries *mask() {
        dtkWarn()<<Q_FUNC_INFO <<"Not implemented";
        return nullptr;
    };

    virtual gnomonImageSeries *output() = 0;

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
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageFilter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageFilter, GNOMONCORE_EXPORT, imageFilter);
}

//
// gnomonAbstractImageFilter.h ends here
