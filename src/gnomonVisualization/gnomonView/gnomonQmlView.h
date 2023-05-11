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

    void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractVisualization> = nullptr) override;

public:
    Q_PROPERTY(QString displayText READ displayText WRITE setDisplayText NOTIFY displayTextChanged);
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged);

    void setDisplayText(const QString&);
    const QString& displayText(void);

    void setFontSize(int);
    int fontSize(void);

signals:
    void displayTextChanged(void);
    void fontSizeChanged(void);

private:
    class gnomonQmlViewPrivate *dd;

};

QString transformMatrixString(QVector<QVector<double> > transform_matrix);

// Q_DECLARE_METATYPE(gnomonQmlView *);
//
// gnomonQmlView.h ends here