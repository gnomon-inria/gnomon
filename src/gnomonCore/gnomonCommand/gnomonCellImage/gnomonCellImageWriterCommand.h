#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

class GNOMONCORE_EXPORT gnomonCellImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonCellImageWriterCommand();
    ~gnomonCellImageWriterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setCellImage(std::shared_ptr<gnomonCellImageSeries> image_series);
    void setAlgorithmName(const QString &) override;

public:
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void clear(void) override;

public:
    inline static const QString groupName = "cellImageWriter";

private:
    class gnomonCellImageWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonCellImageWriterCommand)