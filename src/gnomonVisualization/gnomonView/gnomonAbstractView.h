#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>

class gnomonAbstractDynamicForm;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractView : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractView(QObject *parent = nullptr);
    ~gnomonAbstractView(void);

public:
    Q_PROPERTY(QStringList formNames READ formNames NOTIFY formsChanged);
    Q_PROPERTY(QStringList acceptedForms READ acceptedForms);
    Q_PROPERTY(bool inputView READ inputView WRITE setInputView NOTIFY inputViewChanged);

public:
    virtual void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> ) = 0;
    virtual std::shared_ptr<gnomonAbstractDynamicForm>  form(const QString&)  = 0;
    virtual void clearForm(const QString&) = 0;

public slots:
    virtual void drop(int) = 0;

public slots:
    virtual void setAcceptForm(const QString&, bool) = 0;
    virtual void setInputView(bool) = 0;

public:
    virtual QStringList formNames(void) = 0;
    virtual QStringList acceptedForms(void) = 0;
    virtual bool inputView(void) = 0;

public slots:
    virtual void transmit(void) = 0;

signals:
    void exportedForm(std::shared_ptr<gnomonAbstractDynamicForm> );
    void inputViewChanged(bool);
    void formAdded(const QString&);
    void formsChanged(void);

};

// Q_DECLARE_METATYPE(gnomonAbstractView *);
//
// gnomonAbstractView.h ends here