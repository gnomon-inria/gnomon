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

protected:
    gnomonAbstractAlgorithm *action = nullptr;
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
