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
    Q_PROPERTY(bool inputView READ inputView WRITE setInputView);
    Q_PROPERTY(QString dataDict READ dataDict WRITE setDataDict NOTIFY dictChanged);



public:
    void setForm(const QString&, gnomonAbstractDynamicForm *);
    gnomonAbstractDynamicForm * form(const QString&) ;
    void clearForm(const QString&);

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
    void exportedForm(gnomonAbstractDynamicForm *);
    void formAdded(const QString&);
    void formsChanged(void);
    void dictChanged(void);

private:
    class gnomonViewDataPrivate *d;

};

// Q_DECLARE_METATYPE(gnomonViewData *);
//
// gnomonViewData.h ends here