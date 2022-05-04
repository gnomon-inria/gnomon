#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand.h>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex>

class GNOMONCORE_EXPORT gnomonCellComplexReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonCellComplexReaderCommand();
    ~gnomonCellComplexReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonCellComplexSeries> cellComplex();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;
    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellComplexReader";
    static QStringList availablePlugins();

private:
    class gnomonCellComplexReaderCommandPrivate *d;
};
