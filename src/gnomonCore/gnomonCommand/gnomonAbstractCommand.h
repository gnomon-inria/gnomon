#pragma once

#include <gnomonCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class gnomonAbstractCommand
{
public:
    using orderedMap = std::vector< std::pair <QString, QString > >; // to respect the order of inserting
public:
     gnomonAbstractCommand(void) = default;
    virtual ~gnomonAbstractCommand(void);

public:
    virtual void redo(void) = 0;
    virtual void undo(void) = 0;

    QString documentation(void){
      return action->documentation();
    };

    const QString& algorithmName(void) {
        return this->algorithm_name;
    };

    virtual void setAlgorithmName(const QString &name) = 0;

    const QString& factoryName(void) {
        return this->factory_name;
    };

public:
    virtual dtkCoreParameters parameters(void) const {dtkCoreParameters empty; return empty;};
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual orderedMap inputTypes(void) { orderedMap empty; return empty;};
    virtual void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) {};
    virtual void addInputForm(gnomonAbstractDynamicForm *form) {};


    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual orderedMap outputTypes(void) {orderedMap empty; return empty;};

protected:
    gnomonAbstractAlgorithm *action = nullptr;
    QString algorithm_name = "";
    QString factory_name = "";

};

// ///////////////////////////////////////////////////////////////////

inline gnomonAbstractCommand::~gnomonAbstractCommand(void)
{
    if (action) {
        delete action;
        action = nullptr;
    }
}

//
// gnomonAbstractCommand.h ends here
