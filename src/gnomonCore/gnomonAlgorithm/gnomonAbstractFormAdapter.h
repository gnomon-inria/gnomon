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

#include "gnomonForm/gnomonAbstractDynamicForm.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
template <typename T>
class GNOMONCORE_EXPORT gnomonAbstractFormAdapter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractFormAdapter(void) = default;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInput(T *form_series) = 0;
    virtual T *input() = 0;

    virtual gnomonAbstractDynamicForm *output() = 0;

public:
    virtual QString target(void) = 0;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
};


//
// gnomonAbstractFormAdapter.h ends here