#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>

#include "gnomonAbstractView.h"

class gnomonAbstractDynamicForm;
class gnomonAbstractMatplotlibVisualization;

class GNOMONVISUALIZATION_EXPORT gnomonViewMatplotlib  : public gnomonAbstractView
{
    Q_OBJECT

public:
     gnomonViewMatplotlib(QObject *parent = nullptr);
    ~gnomonViewMatplotlib(void);

public:
    Q_PROPERTY(int figureNumber READ figureNumber WRITE setFigureNumber NOTIFY figureNumberChanged);

public:
    void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> ) override;
    void removeForm(const QString& name) override;

public:
    void setAdaptedForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> , gnomonAbstractMatplotlibVisualization * = nullptr);

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
    Q_INVOKABLE QString formVisuName(const QString& name);
    Q_INVOKABLE QVariantList formVisualizations(const QString& name);
    Q_INVOKABLE void setFormVisuName(const QString& name, const QString& visu_name);
    Q_INVOKABLE QJSValue formVisuParameters(const QString& name);
    Q_INVOKABLE QVariant formVisuParameter(const QString& name, const QString& parameter_name);
    Q_INVOKABLE void setFormVisuParameter(const QString& name, const QString& parameter_name, const QVariant& value);

    Q_INVOKABLE void setFormVisible(const QString& name, bool visible);

signals:
    void figureNumberChanged(int);

public:
    Q_INVOKABLE void notifyFormSelected(int index, QString formType);
    Q_INVOKABLE int lastFormIndexSelected();
    Q_INVOKABLE QString lastFromTypeSelected();
    Q_INVOKABLE QString lastVisuSelected(QString formType);

public slots:
    void render(void);
    void update(void);
    void clear(void);

private:
    class gnomonViewMatplotlibPrivate *dd;
};

Q_DECLARE_METATYPE(gnomonViewMatplotlib *);
//
// gnomonViewMatplotlib.h ends here
