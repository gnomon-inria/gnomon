#pragma once


#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class gnomonTree;

class GNOMONCORE_EXPORT gnomonTreeTransformCommand : public gnomonAbstractCommand
{
public:
     gnomonTreeTransformCommand(void);
    ~gnomonTreeTransformCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonTreeSeries *tree_series);
    gnomonTreeSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonTreeSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "treeTransform";
    static QStringList availablePlugins();

private:
    class gnomonTreeTransformCommandPrivate *d;
};
