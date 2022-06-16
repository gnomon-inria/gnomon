#pragma once

#include <gnomonCoreExport>

#include <dtkCore/dtkCoreParameters>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>

#include "gnomonAbstractAlgorithm.h"
// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
template <typename T>
class GNOMONCORE_EXPORT gnomonAbstractFormAdapter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractFormAdapter(void) = default;

public:
    virtual void setInput(std::shared_ptr<T> form_series) = 0;
    virtual std::shared_ptr<T> input() = 0;

    static inline QString defaultSetter(QString formName) {
        // TODO: find a clever way to do this
        // if(formName == T::formName()) {  //well that's called a CRTP ;) 
            return {"setInput"};
        // }
        // return {};
    };
    static inline QString defaultGetter(QString formName) {
        // TODO: find a clever way to do this
        // if(formName == T::formName()) {
            return {"input"};
        // }
        // return {};
    };

    virtual std::shared_ptr<gnomonAbstractDynamicForm> output() = 0;
    static inline QString defaultOutput(QString formName) {
        return {"output"};
    };

public:
    virtual QString target(void) = 0;

};


//
// gnomonAbstractFormAdapter.h ends here
