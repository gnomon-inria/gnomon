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

#include <gnomonCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonAbstractConstructorCommand : public gnomonAbstractCommand
{
public:
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override {
        return {};
    }

    orderedMap inputTypes() override {
        return {};
    }

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override {
        dtkWarn() << Q_FUNC_INFO << "Constructor takes no input form";
    }
};


//
// gnomonAbstractConstructorCommand.h ends here
