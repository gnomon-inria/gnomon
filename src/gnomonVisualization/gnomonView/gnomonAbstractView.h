#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>
#include <QtGui>

class gnomonAbstractDynamicForm;
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

public:
    virtual void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractVisualization> = nullptr);
    virtual std::shared_ptr<gnomonAbstractDynamicForm>  form(const QString&);
    virtual void removeForm(const QString& name);

    virtual inline void render(void) {}
    virtual inline void update(void) {}
    virtual void clear(void);

public:
    QStringList nodePortNames(void);
    void setNodePortNames(const QStringList&);

public slots:
    virtual void drop(int);
    virtual void transmit(void);

public slots:
    virtual void setAcceptForm(const QString&, bool);
    virtual void setInputView(bool);

public:
    virtual QStringList formNames(void);
    virtual QStringList formNamesAndId(void);
    virtual QStringList acceptedForms(void);
    virtual bool inputView(void);

    virtual bool empty(void);


public:
    Q_INVOKABLE QString formVisuName(const QString& name);
    Q_INVOKABLE QVariantList formVisualizations(const QString& name);

signals:
    void exportedForm(std::shared_ptr<gnomonAbstractDynamicForm> );
    void inputViewChanged(bool);
    void formAdded(const QString&);
    void formRemoved(const QString&);
    void formsChanged(void);

    void badFormDropped(QString badFormName, QString acceptedForms);

public:
    class gnomonAbstractViewPrivate *d;
};

// Q_DECLARE_METATYPE(gnomonAbstractView *);
//
// gnomonAbstractView.h ends here