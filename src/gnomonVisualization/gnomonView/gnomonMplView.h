#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtQml>

#include "gnomonAbstractView.h"

class gnomonAbstractDynamicForm;
class gnomonAbstractVisualization;
class gnomonAbstractMplVisualization;

class GNOMONVISUALIZATION_EXPORT gnomonMplView  : public gnomonAbstractView
{
    Q_OBJECT

public:
     gnomonMplView(QObject *parent = nullptr);
    virtual ~gnomonMplView(void);

public:
    Q_PROPERTY(int figureNumber READ figureNumber WRITE setFigureNumber NOTIFY figureNumberChanged);

public:
    void setAdaptedForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractMplVisualization> visualization = nullptr);

public slots:
    void setFigureNumber(int num);

public:
    int figureNumber(void);

signals:
    void figureNumberChanged(int);

public slots:
    void render(void) override;
    void clear(void) override;
    void saveScreenshot(const QString& filename) override;

private:
    class gnomonMplViewPrivate *dd;
};

Q_DECLARE_METATYPE(gnomonMplView *);
//
// gnomonMplView.h ends here
