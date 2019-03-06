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

#include <QUndoCommand>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include <gnomonCore>

class gnomonAbstractCommand : public QUndoCommand
{
public:
     gnomonAbstractCommand(void) = default;
    ~gnomonAbstractCommand(void);

public:
    virtual void redo(void) = 0;
    virtual void undo(void) = 0;

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
