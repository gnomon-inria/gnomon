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

#include <gnomonCoreExport.h>

#include <QUndoCommand>

template<typename T>
class gnomonAbstractCommand : public QUndoCommand
{
public:
    gnomonAbstractCommand(void) = default;
    virtual ~gnomonAbstractCommand(void) = default;

public:
    virtual void redo(void) = 0;
    virtual void undo(void) = 0;

protected:
    T* action = nullptr;
};

//
// gnomonAbstractCommand.h ends here
