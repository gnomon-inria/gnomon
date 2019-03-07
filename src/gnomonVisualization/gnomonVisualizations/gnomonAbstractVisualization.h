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

#include <gnomonVisualizationExport.h>

class gnomonViewForm;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualization : public QObject
{
    Q_OBJECT

public:
     gnomonAbstractVisualization();
    ~gnomonAbstractVisualization(void);

public:
    void setView(gnomonViewForm *view);
    virtual void setParameter(const QString&, const QVariant&) = 0;
    virtual void setParameters(const QMap<QString, gnomonCoreParameter *>&) = 0;

public:
    gnomonViewForm* view(void);
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const = 0;

public:
    virtual QImage imageRendering(void) = 0;

signals:
    void parametersChanged(void);

public slots:
    virtual void update(void) = 0;
    virtual void render(void) = 0;

protected:
    class gnomonAbstractVisualizationPrivate *d;
};

//
// gnomonAbstractVisualization.h ends here
