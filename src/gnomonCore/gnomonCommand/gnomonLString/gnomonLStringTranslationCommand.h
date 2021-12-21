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

class GNOMONCORE_EXPORT gnomonLStringTranslationCommand : public gnomonAbstractCommand
{
public:
     gnomonLStringTranslationCommand();
    ~gnomonLStringTranslationCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInputTree(gnomonTreeSeries *tree_series);
    gnomonTreeSeries *inputTree();
    void setInputLString(gnomonLStringSeries *lString_series);
    gnomonLStringSeries *inputLString();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

public:
    gnomonTreeSeries *outputTree();
    gnomonLStringSeries *outputLString();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

public:
    static bool isEmpty();

private:
    class gnomonLStringTranslationCommandPrivate *d;
};

//
// gnomonLStringTranslationCommand.h ends here
