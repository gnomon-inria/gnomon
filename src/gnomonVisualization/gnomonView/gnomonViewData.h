#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>

#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

class gnomonAbstractDynamicForm;

class GNOMONVISUALIZATION_EXPORT gnomonViewData : public QObject
{
    Q_OBJECT

public:
    gnomonViewData(QObject *parent = nullptr);
    ~gnomonViewData(void);

public:
    Q_PROPERTY(QStringList formNames READ formNames NOTIFY formsChanged);
    Q_PROPERTY(QStringList acceptedForms READ acceptedForms);
    Q_PROPERTY(bool inputView READ inputView WRITE setInputView NOTIFY inputViewChanged);
    Q_PROPERTY(QString dataDict READ dataDict WRITE setDataDict NOTIFY dictChanged);

public:
    void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> );
    std::shared_ptr<gnomonAbstractDynamicForm>  form(const QString&) ;
    void clearForm(const QString&);

public slots:
    void drop(int);

public slots:
    void setAcceptForm(const QString&, bool);
    void setInputView(bool);
    void setDataDict(QString);

public:
    QStringList formNames(void);
    QStringList acceptedForms(void);
    bool inputView(void);
    QString dataDict(void);


public slots:
    void transmit(void);

signals:
    void exportedForm(std::shared_ptr<gnomonAbstractDynamicForm> );
    void inputViewChanged(bool);
    void formAdded(const QString&);
    void formsChanged(void);
    void dictChanged(void);

private:
    class gnomonViewDataPrivate *d;

};

// Q_DECLARE_METATYPE(gnomonViewData *);
//
// gnomonViewData.h ends here