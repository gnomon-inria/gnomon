#pragma once

#include <gnomonCoreExport.h>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAbstractModel.h"

#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>

class GNOMONCORE_EXPORT gnomonAbstractLStringEvolutionModel : public gnomonAbstractModel
{
    Q_OBJECT

public:
    virtual ~gnomonAbstractLStringEvolutionModel(void) = default;

public:
    void step(double time, double dt) override { stepAndReturn(time, dt); };
    virtual std::shared_ptr<gnomonLString> stepAndReturn(double time, double dt) = 0;
    virtual std::shared_ptr<gnomonLStringSeries> state(void) = 0;
    virtual std::shared_ptr<gnomonLStringSeries> initialState(void) = 0;
    virtual void setInitialState(std::shared_ptr<gnomonLStringSeries>) = 0;

// TODO: Not sure the model abstraction should know about LSystem code :/
public:
    virtual void setLSystem(const QString& code) = 0;
    virtual const QString& lSystemCode(void) = 0;

    virtual int derivationLength(void) = 0;
    virtual void setDerivationLength(int) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"setInitialState"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"initialState"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonLString") {
            return {"state"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT        (gnomonAbstractLStringEvolutionModel *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringEvolutionModel, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringEvolutionModel, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractLStringEvolutionModel, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractLStringEvolutionModel, GNOMONCORE_EXPORT, lStringEvolutionModel);
}

//
// gnomonAbstractLStringEvolutionModel.h ends here
