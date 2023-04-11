#pragma once

#include <QtCore>

#include <dtkCore/dtkCoreParameters>
#include <gnomonVisualizationExport.h>
#include <dtkImagingCore>

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
    virtual void connectParameter(const QString& parameter_name);
    virtual void refreshParameters(void);
    virtual void onParameterChanged(const QString& parameter_name = "");

public:
    virtual dtkCoreParameters parameters(void) const = 0;
    virtual QMap<QString, QString> parameterGroups(void) { return QMap<QString, QString>(); };

public:
    QVariantMap visuParameters(void);
    void setVisuParameters(QVariantMap parameters);

public:
    virtual QImage imageRendering(void) = 0;

signals:
    void parametersChanged(void);

public slots:
    // recompute the form visualization display and if necessary add it to its view
    virtual void update(void) = 0;
    // refresh the view where the visualization is displayed without any recomputing
    virtual void render(void) = 0;
    // remove the form visualization from its view without deleting the computed display
    virtual void clear(void) = 0;
    // add the form visualization to its view, if possible without any recomputing
    virtual void fill(void) = 0;

    virtual inline void clearConnections(void) {  };

public slots:
    virtual void setVisible(bool visible) = 0;

protected:
    class gnomonAbstractVisualizationPrivate *d;
};

//
// gnomonAbstractVisualization.h ends here
