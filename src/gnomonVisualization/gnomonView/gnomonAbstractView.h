#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>
#include <QtGui>

class gnomonAbstractDynamicForm;

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
    virtual void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> );
    virtual std::shared_ptr<gnomonAbstractDynamicForm>  form(const QString&);
    virtual void removeForm(const QString& name);
    virtual void clear(void);

public slots:
    virtual void drop(int);

public slots:
    virtual void setAcceptForm(const QString&, bool);
    virtual void setInputView(bool);

public:
    virtual QStringList formNames(void);
    virtual QStringList formNamesAndId(void);
    virtual QStringList acceptedForms(void);
    virtual bool inputView(void);

    virtual bool empty(void);

public slots:
    virtual void transmit(void);

signals:
    void exportedForm(std::shared_ptr<gnomonAbstractDynamicForm> );
    void inputViewChanged(bool);
    void formAdded(const QString&);
    void formRemoved(const QString&);
    void formsChanged(void);

    void badFormDropped(QString badFormName, QString acceptedForms);

protected:
    class gnomonAbstractViewPrivate *d;
};

// Q_DECLARE_METATYPE(gnomonAbstractView *);
//
// gnomonAbstractView.h ends here