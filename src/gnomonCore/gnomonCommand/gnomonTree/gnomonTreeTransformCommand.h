#pragma once


#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class gnomonTree;

class GNOMONCORE_EXPORT gnomonTreeTransformCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonTreeTransformCommand(void);
    ~gnomonTreeTransformCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonTreeSeries> tree_series);
    std::shared_ptr<gnomonTreeSeries> input();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    std::shared_ptr<gnomonTreeSeries> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    inline static const QString groupName = "treeTransform";

private:
    class gnomonTreeTransformCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonTreeTransformCommand)