#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

class dtkCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageRegistration : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractImageRegistration(void) = default;

public:
    virtual void setImage(std::shared_ptr<gnomonImageSeries> ) = 0;
    virtual std::shared_ptr<gnomonImageSeries> image() = 0;
    virtual void setInitialTransformation(std::shared_ptr<gnomonDataDictSeries>) = 0;
    virtual std::shared_ptr<gnomonDataDictSeries> initialTransformation() = 0;

    virtual std::shared_ptr<gnomonImageSeries> output() = 0;
    virtual std::shared_ptr<gnomonDataDictSeries> outputTransformation() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setImage"};
        } else if(formName == "gnomonDataDict") {
            return {"setInitialTransformation"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"image"};
        } else if(formName == "gnomonDataDict") {
            return {"initialTransformation"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonImage") {
            return {"output"};
        } else if(formName == "gnomonDataDict") {
            return {"outputTransformation"};
        }
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageRegistration *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageRegistration, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageRegistration, GNOMONCORE_EXPORT, imageRegistration, gnomonCore)


//
// gnomonAbstractImageRegistration.h ends here
