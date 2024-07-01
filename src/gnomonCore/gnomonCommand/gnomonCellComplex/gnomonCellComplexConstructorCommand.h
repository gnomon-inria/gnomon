#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>

#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex>

class GNOMONCORE_EXPORT gnomonCellComplexConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonCellComplexConstructorCommand();
    ~gnomonCellComplexConstructorCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonCellComplexSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    inline static const QString groupName = "cellComplexConstructor";

private:
    class gnomonCellComplexConstructorCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonCellComplexConstructorCommand)