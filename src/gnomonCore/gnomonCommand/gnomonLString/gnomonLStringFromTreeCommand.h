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

#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonLStringFromTreeCommand : public gnomonAbstractCommand
{
public:
     gnomonLStringFromTreeCommand(void) = delete;
     gnomonLStringFromTreeCommand(const QString&);
    ~gnomonLStringFromTreeCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonTreeSeries *tree_serie);
    gnomonTreeSeries *input();

    void setParameter(const QString&, const QVariant&);

public:
    gnomonLStringSeries *output();
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonLStringFromTreeCommandPrivate *d;
};

//
// gnomonLStringFromTreeCommand.h ends here
