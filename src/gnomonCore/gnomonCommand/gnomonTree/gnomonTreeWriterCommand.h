#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonTreeWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonTreeWriterCommand(void);
    ~gnomonTreeWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setTree(gnomonTreeSeries *image_series);
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "treeWriter";
    static QStringList availablePlugins();

private:
    class gnomonTreeWriterCommandPrivate *d;
};
