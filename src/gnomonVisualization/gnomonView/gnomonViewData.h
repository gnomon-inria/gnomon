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
    void setForm(const QString&, gnomonAbstractDynamicForm *);
    gnomonAbstractDynamicForm * form(const QString&) ;
    void clearForm(const QString&);

public slots:
    void transmit(void);

signals:
    void exportedForm(gnomonAbstractDynamicForm *);
    void formAdded(const QString&);
    void formsChanged(void);

private:
    class gnomonViewDataPrivate *d;

};

// Q_DECLARE_METATYPE(gnomonViewData *);
//
// gnomonViewData.h ends here