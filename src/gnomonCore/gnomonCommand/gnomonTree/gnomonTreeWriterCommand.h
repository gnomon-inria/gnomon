#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonTreeWriterCommand : public gnomonAbstractWriterCommand {
public:
     gnomonTreeWriterCommand(void);
    ~gnomonTreeWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setTree(std::shared_ptr<gnomonTreeSeries> image_series);
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

public:
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void clear(void) override;

public:
    inline static const QString groupName = "treeWriter";

private:
    class gnomonTreeWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonTreeWriterCommand)
