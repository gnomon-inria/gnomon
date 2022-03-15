#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractBinaryImageFromImage : public gnomonAbstractAlgorithm
{
    //Inputs
public:
    virtual void setInput(gnomonImageSeries *image) = 0;
    virtual void setInitialization(gnomonBinaryImageSeries *init) {dtkWarn()<<Q_FUNC_INFO<<"Not implemented";};

    // Outputs
public:
    virtual gnomonImageSeries *input(void) const = 0;
    virtual inline gnomonBinaryImageSeries *initialization(void) const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setInput"};
        } else if (formName == "gnomonBinaryImage") {
            return {"setInitialization"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"input"};
        } else if (formName == "gnomonBinaryImage") {
            return {"initialization"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonBinaryImage") {
            return {"output"};
        }
        return {};
    };

    virtual gnomonBinaryImageSeries *output() const = 0;


public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters() const override = 0;


public:
    virtual void run() override = 0;
    virtual QString documentation() override = 0;

};
DTK_DECLARE_OBJECT(gnomonAbstractBinaryImageFromImage *)
DTK_DECLARE_PLUGIN(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT, binaryImageFromImage);
}

//
// gnomonAbstractBinaryImageFromImage.h ends here
