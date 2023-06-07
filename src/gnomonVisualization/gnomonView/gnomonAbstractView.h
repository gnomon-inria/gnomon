#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>
#include <QtGui>

class gnomonAbstractDynamicForm;
class gnomonDynamicFormMetadata;
class gnomonAbstractVisualization;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractView : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractView(QObject *parent = nullptr);
    ~gnomonAbstractView(void);

public:
    Q_PROPERTY(QStringList formNames READ formNames NOTIFY formsChanged);
    Q_PROPERTY(QStringList formNamesAndId READ formNamesAndId NOTIFY formsChanged);
    Q_PROPERTY(QStringList acceptedForms READ acceptedForms);
    Q_PROPERTY(bool inputView READ inputView WRITE setInputView NOTIFY inputViewChanged);

    Q_PROPERTY(bool empty READ empty NOTIFY formsChanged);

public:
    virtual void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractVisualization> = nullptr);
    virtual std::shared_ptr<gnomonAbstractDynamicForm>  form(const QString&);
    virtual void removeForm(const QString& form_type);

public slots:
    virtual inline void render(void) {} // refresh the display of the view
    virtual void update(void); // update the visualizations of the displayed forms
    virtual void clear(void); // remove all displayed forms and their visualizations

public:
    QStringList nodePortNames(void);
    void setNodePortNames(const QStringList&);

public slots:
    virtual void drop(int);
    virtual void transmit(void);
    virtual void restoreState(void);

public slots:
    virtual inline void saveScreenshot(const QString& filename) {};
    virtual inline QImage toImage(void) { return QImage(); };

public slots:
    virtual void setAcceptForm(const QString&, bool);
    virtual void setInputView(bool);

public:
    virtual QStringList formNames(void);
    virtual QStringList formNamesAndId(void);
    virtual QStringList acceptedForms(void);
    virtual bool inputView(void);

    bool contains(const QString&);
    virtual bool empty(void);

public:
    Q_INVOKABLE QString formVisuName(const QString& form_type);
    Q_INVOKABLE void setFormVisuName(const QString& form_type, const QString& visu_name);
    Q_INVOKABLE QVariantList formVisualizations(const QString& form_type);

    Q_INVOKABLE gnomonDynamicFormMetadata* formMetadata(const QString& form_type);

    Q_INVOKABLE QJSValue formVisuParameters(const QString& form_type);
    Q_INVOKABLE QVariant formVisuParameter(const QString& form_type, const QString& parameter_name);
    Q_INVOKABLE void setFormVisuParameter(const QString& form_type, const QString& parameter_name, const QVariant& value);

    Q_INVOKABLE void setFormVisible(const QString& form_type, bool visible);

public:
    Q_INVOKABLE void notifyFormSelected(int index, QString form_type);
    Q_INVOKABLE int lastFormIndexSelected();
    Q_INVOKABLE QString lastFromTypeSelected();
    Q_INVOKABLE QString lastVisuSelected(QString form_type);

signals:
    void exportedForm(std::shared_ptr<gnomonAbstractDynamicForm> );
    void inputViewChanged(bool);
    void formAdded(const QString&);
    void formRemoved(const QString&);
    void formsChanged(void);

    void formVisuParametersChanged(void);
    void formVisualizationChanged(const QString&);

    void badFormDropped(const QString& form_type, const QString& acceptedForms);

public:
    class gnomonAbstractViewPrivate *d;
};

// Q_DECLARE_METATYPE(gnomonAbstractView *);
//
// gnomonAbstractView.h ends here