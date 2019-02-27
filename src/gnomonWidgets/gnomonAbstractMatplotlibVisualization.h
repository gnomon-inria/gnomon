// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <QtCore>

#include <gnomonWidgetsExport.h>

class gnomonViewMatplotlib;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonAbstractMatplotlibVisualization : public QObject
{
    Q_OBJECT

public:
     gnomonAbstractMatplotlibVisualization();
    ~gnomonAbstractMatplotlibVisualization(void);

public:
    void setView(gnomonViewMatplotlib *view);
    virtual void setParameter(const QString&, const QVariant&) = 0;
    virtual void setParameters(const QMap<QString, gnomonCoreParameter *>&) = 0;

public:
    gnomonViewMatplotlib* view(void);
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const = 0;

public:
    virtual QImage imageRendering(void) = 0;

signals:
    void parametersChanged(void);

public slots:
    virtual void update(void) = 0;
    virtual void render(void) = 0;

protected:
    class gnomonAbstractMatplotlibVisualizationPrivate *d;
};

//
// gnomonAbstractMatplotlibVisualization.h ends here
