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
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setCellImage(gnomonCellImageSeries *image_series);
    void setAlgorithmName(const QString &) override;

public:
    QStringList extensions(void) override;
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageWriter";
    static QStringList availablePlugins();

private:
    class gnomonCellImageWriterCommandPrivate *d;
};
