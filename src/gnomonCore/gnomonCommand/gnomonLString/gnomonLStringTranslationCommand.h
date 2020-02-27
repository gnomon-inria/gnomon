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

    void setParameter(const QString&, const QVariant&);

public:
    gnomonTreeSeries *outputTree();
    gnomonLStringSeries *outputLString();
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonLStringTranslationCommandPrivate *d;
};

//
// gnomonLStringTranslationCommand.h ends here
