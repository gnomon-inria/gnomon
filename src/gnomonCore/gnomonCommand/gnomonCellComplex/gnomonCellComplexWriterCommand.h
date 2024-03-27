#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex>

class GNOMONCORE_EXPORT gnomonCellComplexWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonCellComplexWriterCommand();
    ~gnomonCellComplexWriterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> ) override;
    void setCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellcomplex);
    void setAlgorithmName(const QString &) override;

public:
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

public:
    inline static const QString groupName = "cellComplexWriter";

private:
    class gnomonCellComplexWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonCellComplexWriterCommand)