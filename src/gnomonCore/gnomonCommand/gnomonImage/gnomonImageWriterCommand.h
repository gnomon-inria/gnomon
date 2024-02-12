#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>

class GNOMONCORE_EXPORT gnomonImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonImageWriterCommand(void);
    ~gnomonImageWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setImage(std::shared_ptr<gnomonImageSeries> image_series);
    void setAlgorithmName(const QString &) override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

public:
    orderedMap inputTypes() override;

public:
    inline static const QString groupName = "imageWriter";

private:
    class gnomonImageWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonImageWriterCommand)