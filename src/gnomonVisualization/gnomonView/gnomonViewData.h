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

    void setDataDict(QString);
    QString dataDict(void);

signals:
    void dictChanged(void);

private:
    class gnomonViewDataPrivate *dd;

};

// Q_DECLARE_METATYPE(gnomonViewData *);
//
// gnomonViewData.h ends here