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
    void setForm(gnomonAbstractDynamicForm *) override;
    void setCellComplex(gnomonCellComplexSeries *cellcomplex);
    void setAlgorithmName(const QString &) override;

public:
    QStringList extensions(void) override;
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellComplexWriter";
    static QStringList availablePlugins();

private:
    class gnomonCellComplexWriterCommandPrivate *d;
};
