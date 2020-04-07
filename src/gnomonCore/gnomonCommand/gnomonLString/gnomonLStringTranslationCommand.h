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
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonLStringTranslationCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonLStringTranslationCommand(void) = delete;
     gnomonLStringTranslationCommand(const QString&);
    ~gnomonLStringTranslationCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInputTree(gnomonTreeSeries *tree_series);
    gnomonTreeSeries *inputTree();
    void setInputLString(gnomonLStringSeries *lString_series);
    gnomonLStringSeries *inputLString();
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    void setParameter(const QString&, const QVariant&);

public:
    gnomonTreeSeries *outputTree();
    gnomonLStringSeries *outputLString();
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonLStringTranslationCommandPrivate *d;
};

//
// gnomonLStringTranslationCommand.h ends here
