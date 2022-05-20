#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>

#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
class gnomonAbstractMatplotlibVisualization;

class GNOMONVISUALIZATION_EXPORT gnomonViewMatplotlib  : public QObject
{
    Q_OBJECT

public:
     gnomonViewMatplotlib(QObject *parent = nullptr);
    ~gnomonViewMatplotlib(void);

public:
    Q_PROPERTY(int figureNumber READ figureNumber WRITE setFigureNumber NOTIFY figureNumberChanged);
    Q_PROPERTY(QStringList formNames READ formNames NOTIFY formsChanged);
    Q_PROPERTY(QStringList acceptedForms READ acceptedForms);
    Q_PROPERTY(bool inputView READ inputView WRITE setInputView);

public:
    void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>);
    std::shared_ptr<gnomonAbstractDynamicForm> form (const QString&);
    void clearForm(const QString&);

public:
    void setAdaptedForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> , gnomonAbstractMatplotlibVisualization * = nullptr);

public slots:
    void setAcceptForm(const QString&, bool);

public:
    void setIsModifiedForm(const QString&);

public slots:
    void setFigureNumber(int num);
    void setInputView(bool);

public:
    int figureNumber(void);
    QStringList formNames(void);
    QStringList acceptedForms(void);
    bool inputView(void);

signals:
    void formsChanged(void);

public:
    void updateVisualizations(void);

public:
    Q_INVOKABLE QString formVisuName(const QString& name);
    Q_INVOKABLE QStringList formVisualizations(const QString& name);
    Q_INVOKABLE void setFormVisuName(const QString& name, const QString& visu_name);
    Q_INVOKABLE QJSValue formVisuParameters(const QString& name);

signals:
    void formAdded(const QString&);
    void formRemoved(const QString&);
    void figureNumberChanged(int);

public slots:
    void render(void);
    void update(void);
    void clear(void);
    void transmit(void);

signals:
    void exportedForm(std::shared_ptr<gnomonAbstractDynamicForm> );

private:
    class gnomonViewMatplotlibPrivate *d;
};

Q_DECLARE_METATYPE(gnomonViewMatplotlib *);
//
// gnomonViewMatplotlib.h ends here
