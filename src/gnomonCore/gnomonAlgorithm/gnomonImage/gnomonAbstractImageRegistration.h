#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

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

    virtual std::shared_ptr<gnomonImageSeries> output() = 0;
    virtual std::shared_ptr<gnomonDataDictSeries> outputTransformation() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"image"};
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
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageRegistration, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageRegistration, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageRegistration, GNOMONCORE_EXPORT, imageRegistration);
}

//
// gnomonAbstractImageRegistration.h ends here
