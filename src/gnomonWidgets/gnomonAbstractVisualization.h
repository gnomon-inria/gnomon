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

#include <dtkCore>

class gnomonViewForm;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonAbstractVisualization : public QObject
{
    Q_OBJECT

public:
     gnomonAbstractVisualization();
    ~gnomonAbstractVisualization(void);

public:
    void setView(gnomonViewForm *view);
    void setParameter(const QString&, const QVariant&);
    void setParameters(const QMap<QString, gnomonCoreParameter *>&);

public:
    gnomonViewForm* view(void);
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

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

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualization, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualization, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualization, GNOMONWIDGETS_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonWidgets layer
// /////////////////////////////////////////////////////////////////

namespace gnomonWidgets {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualization, GNOMONWIDGETS_EXPORT, visualization);
}

//
// gnomonAbstractVisualization.h ends here
