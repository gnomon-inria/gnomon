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
class gnomonAbstractUndoCommand : public QUndoCommand
{
public:
    gnomonAbstractUndoCommand(void) = default;
    virtual ~gnomonAbstractUndoCommand(void) = default;

public:
    T* getCommand(void) {return command;}

public:
    virtual void redo(void) = 0;
    virtual void undo(void) = 0;

protected:
    T* command = nullptr;
};

//
// gnomonAbstractUndoCommand.h ends here
