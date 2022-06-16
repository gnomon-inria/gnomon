#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>

class GNOMONCORE_EXPORT gnomonDataFrameWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonDataFrameWriterCommand();
    ~gnomonDataFrameWriterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setDataFrame(std::shared_ptr<gnomonDataFrameSeries> image_series);
    void setAlgorithmName(const QString &) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

public:
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "dataFrameWriter";
    static QStringList availablePlugins();

private:
    class gnomonDataFrameWriterCommandPrivate *d;
};
