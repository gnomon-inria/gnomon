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

template<typename T>
class gnomonAbstractCommand : public QUndoCommand
{
public:
     gnomonAbstractCommand(void) = default;
    ~gnomonAbstractCommand(void);

public:
    virtual void redo(void) = 0;
    virtual void undo(void) = 0;

protected:
    T *action = nullptr;
};

// ///////////////////////////////////////////////////////////////////

template<typename T>
inline gnomonAbstractCommand<T>::~gnomonAbstractCommand(void)
{
    if (action) {
        delete action;
        action = nullptr;
    }
}

//
// gnomonAbstractCommand.h ends here
