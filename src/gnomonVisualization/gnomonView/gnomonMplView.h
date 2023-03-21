#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>

#include "gnomonAbstractView.h"

class gnomonAbstractDynamicForm;
class gnomonAbstractVisualization;
class gnomonAbstractMplVisualization;

class GNOMONVISUALIZATION_EXPORT gnomonMplView  : public gnomonAbstractView
{
    Q_OBJECT

public:
     gnomonMplView(QObject *parent = nullptr);
    ~gnomonMplView(void);

public:
    Q_PROPERTY(int figureNumber READ figureNumber WRITE setFigureNumber NOTIFY figureNumberChanged);

public:
    void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractVisualization> = nullptr) override;
    void removeForm(const QString& name) override;

public:
    void setAdaptedForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractMplVisualization> visualization = nullptr);

public:
    void setIsModifiedForm(const QString&);

public slots:
    void setFigureNumber(int num);

public:
    int figureNumber(void);

signals:
    void formVisuParametersChanged(void);
    void formVisualizationChanged(void);

public:
    void updateVisualizations(void);

public:
    Q_INVOKABLE void setFormVisuName(const QString& name, const QString& visu_name);

signals:
    void figureNumberChanged(int);

public:
    Q_INVOKABLE void notifyFormSelected(int index, QString form_type);
    Q_INVOKABLE int lastFormIndexSelected();
    Q_INVOKABLE QString lastFromTypeSelected();
    Q_INVOKABLE QString lastVisuSelected(QString form_type);

public slots:
    void render(void) override;
    void update(void) override;
    void clear(void) override;

private:
    class gnomonMplViewPrivate *dd;
};

Q_DECLARE_METATYPE(gnomonMplView *);
//
// gnomonMplView.h ends here
