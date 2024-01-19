#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>
#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>

class GNOMONCORE_EXPORT gnomonDataFrameReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonDataFrameReaderCommand();
    ~gnomonDataFrameReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonDataFrameSeries> dataFrame();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "dataFrameReader";

private:
    class gnomonDataFrameReaderCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonDataFrameReaderCommand)