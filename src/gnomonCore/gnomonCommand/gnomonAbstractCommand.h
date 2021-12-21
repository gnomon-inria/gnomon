#pragma once

#include <gnomonCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class gnomonAbstractCommand
{
public:
    using orderedMap = std::vector< std::pair <QString, QString > >; // to respect the order of inserting


public:
    gnomonAbstractCommand() = default;
    virtual ~gnomonAbstractCommand();

public:
    virtual void redo() = 0;
    virtual void undo() = 0;

    QString documentation(){
      return action->documentation();
    };

    const QString& algorithmName() {
        return this->algorithm_name;
    };

    virtual void setAlgorithmName(const QString &name) = 0;

    const QString& factoryName() {
        return this->factory_name;
    };

public:
    inline virtual dtkCoreParameters parameters() const {return this->action->parameters();};
    inline virtual void setParameter(const QString& parameter, const QVariant& value) {
        this->action->setParameter(parameter, value);
    };
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs() = 0;
    virtual orderedMap inputTypes() = 0;
    virtual void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) {}
    virtual void addInputForm(gnomonAbstractDynamicForm *form) {}


    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs() = 0;
    virtual orderedMap outputTypes() = 0;

protected:
    gnomonAbstractAlgorithm *action = nullptr;
    QString algorithm_name = "";
    QString factory_name = "";

};

// ///////////////////////////////////////////////////////////////////

inline gnomonAbstractCommand::~gnomonAbstractCommand()
{
    if (action) {
        delete action;
        action = nullptr;
    }
}
