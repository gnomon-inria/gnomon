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

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonLStringTranslationCommand : public gnomonAbstractCommand
{
public:
     gnomonLStringTranslationCommand(void);
    ~gnomonLStringTranslationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInputTree(gnomonTreeSeries *tree_series);
    gnomonTreeSeries *inputTree();
    void setInputLString(gnomonLStringSeries *lString_series);
    gnomonLStringSeries *inputLString();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    gnomonTreeSeries *outputTree();
    gnomonLStringSeries *outputLString();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "lStringTranslation";
    static QStringList availablePlugins();

private:
    class gnomonLStringTranslationCommandPrivate *d;
};

//
// gnomonLStringTranslationCommand.h ends here
