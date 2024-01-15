#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>
#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonLStringTranslationCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonLStringTranslationCommand(void);
    ~gnomonLStringTranslationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInputTree(std::shared_ptr<gnomonTreeSeries> tree_series);
    std::shared_ptr<gnomonTreeSeries> inputTree();
    void setInputLString(std::shared_ptr<gnomonLStringSeries> lString_series);
    std::shared_ptr<gnomonLStringSeries> inputLString();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    std::shared_ptr<gnomonTreeSeries> outputTree();
    std::shared_ptr<gnomonLStringSeries> outputLString();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

public:
    inline static const QString groupName = "lStringTranslation";

private:
    class gnomonLStringTranslationCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonLStringTranslationCommand)
//
// gnomonLStringTranslationCommand.h ends here
