#pragma once

#include "gnomonCommand/gnomonAbstractWriterCommand.h"

#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"

class GNOMONCORE_EXPORT gnomonBinaryImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonBinaryImageWriterCommand();
    ~gnomonBinaryImageWriterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> ) override;
    void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binaryImage);
    void setAlgorithmName(const QString &) override;

public:
    orderedMap inputTypes() override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void clear(void) override;

public:
    inline static const QString groupName = "binaryImageWriter";

private:
    class gnomonBinaryImageWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonBinaryImageWriterCommand)