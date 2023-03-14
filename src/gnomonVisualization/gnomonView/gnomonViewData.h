#pragma once

#include <gnomonVisualizationExport>

#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

#include "gnomonAbstractView.h"

class gnomonAbstractDynamicForm;

class GNOMONVISUALIZATION_EXPORT gnomonViewData : public gnomonAbstractView
{
    Q_OBJECT

public:
    gnomonViewData(QObject *parent = nullptr);
    ~gnomonViewData(void);

public:
    Q_PROPERTY(QString dataDict READ dataDict WRITE setDataDict NOTIFY dictChanged);

public:
    virtual void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> ) override;
    virtual std::shared_ptr<gnomonAbstractDynamicForm>  form(const QString&) override;
    virtual void clearForm(const QString&) override;

public slots:
    virtual void drop(int) override;

public slots:
    virtual void setAcceptForm(const QString&, bool) override;
    virtual void setInputView(bool) override;
    void setDataDict(QString);

public:
    virtual QStringList formNames(void) override;
    virtual QStringList acceptedForms(void) override;
    virtual bool inputView(void) override;
    QString dataDict(void);


public slots:
    virtual void transmit(void) override;

signals:
    void dictChanged(void);

private:
    class gnomonViewDataPrivate *d;

};

// Q_DECLARE_METATYPE(gnomonViewData *);
//
// gnomonViewData.h ends here