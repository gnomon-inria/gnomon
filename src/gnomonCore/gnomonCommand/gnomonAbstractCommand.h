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

class gnomonAbstractCommand
{
public:
     gnomonAbstractCommand(void) = default;
    ~gnomonAbstractCommand(void);

public:
    virtual void redo(void) = 0;
    virtual void undo(void) = 0;

    QString documentation(void){
      return action->documentation();
    };

    const QString& algorithmName(void) {
        return this->algorithm_name;
    };

public:
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};

public:
    static const QString factoryName(void) {return "";};

protected:
    gnomonAbstractAlgorithm *action = nullptr;

    QString algorithm_name = "";
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
