#pragma once

#include <gnomonVisualizationExport>

#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

#include "gnomonAbstractView.h"

class gnomonAbstractDynamicForm;

class GNOMONVISUALIZATION_EXPORT gnomonQmlView : public gnomonAbstractView
{
    Q_OBJECT

public:
    gnomonQmlView(QObject *parent = nullptr);
    ~gnomonQmlView(void);

public:
    Q_PROPERTY(QString dataDict READ dataDict WRITE setDataDict NOTIFY dictChanged);

    void setDataDict(QString);
    QString dataDict(void);

signals:
    void dictChanged(void);

private:
    class gnomonQmlViewPrivate *dd;

};

QString transformMatrixString(QVector<QVector<double> > transform_matrix);

// Q_DECLARE_METATYPE(gnomonQmlView *);
//
// gnomonQmlView.h ends here