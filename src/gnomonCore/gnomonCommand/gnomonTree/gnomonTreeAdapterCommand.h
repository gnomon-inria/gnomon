#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonTreeAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonTreeAdapterCommand(void);
    ~gnomonTreeAdapterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonTreeSeries> tree_series);
    void setAlgorithmName(const QString& algo_name) override;

public:
    std::shared_ptr<gnomonTreeSeries> input();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    std::shared_ptr<gnomonAbstractDynamicForm> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "treeAdapter";

private:
    class gnomonTreeAdapterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonTreeAdapterCommand)