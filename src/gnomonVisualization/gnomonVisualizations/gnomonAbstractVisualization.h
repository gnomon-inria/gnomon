#pragma once

#include <QtCore>

#include <dtkCore/dtkCoreParameters>
#include <gnomonVisualizationExport.h>

#include <gnomonVisualization/gnomonView/gnomonAbstractView.h>

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualization : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractVisualization();
    ~gnomonAbstractVisualization(void);

public:
    virtual void setView(gnomonAbstractView *view);
    virtual gnomonAbstractView* view(void);

public:
    virtual const QString pluginName(void) = 0;
    virtual const QString name(void) = 0;

public:
    virtual void setParameter(const QString&, const QVariant&) = 0;
    virtual void setParameters(const dtkCoreParameters&) = 0;

public:
    virtual dtkCoreParameters parameters(void) const = 0;
    virtual QMap<QString, QString> parameterGroups(void) { return QMap<QString, QString>(); };

public:
    virtual QImage imageRendering(void) = 0;

signals:
    void parametersChanged(void);

public slots:
    virtual void update(void) = 0;
    virtual void render(void) = 0;
    virtual void clear(void) = 0;

public slots:
    virtual void setVisible(bool visible) = 0;

protected:
    class gnomonAbstractVisualizationPrivate *d;
};

//
// gnomonAbstractVisualization.h ends here
