#pragma once

#include "gnomonAbstractAlgorithmCommand.h"

class gnomonAbstractConstructorCommand : public gnomonAbstractAlgorithmCommand
{
public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override {
        return {};
    }

    orderedMap inputTypes() override {
        return {};
    }

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override {
        dtkWarn() << Q_FUNC_INFO << "Constructor takes no input form";
    }
};


//
// gnomonAbstractConstructorCommand.h ends here
