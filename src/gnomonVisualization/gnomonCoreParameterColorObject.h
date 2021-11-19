// gnomonCoreParameterColorObject.h
//

#pragma once

#include <gnomonVisualizationExport>

#include <dtkCore/dtkCoreParameterObject>

#include "gnomonCoreParameterColor.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorMapObject declaration
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterColorMapObject : public dtkCoreParameterObject
{
    Q_OBJECT
    Q_PROPERTY(QMap<double, QColor> colorMap READ colorMap WRITE setColorMap NOTIFY colorMapChanged)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
     gnomonCoreParameterColorMapObject(gnomonCoreParameterColorMap *);
    ~gnomonCoreParameterColorMapObject(void);

    void setColorMap(const QMap<double, QColor>&);
    QMap<double, QColor> colorMap(void) const;

    void setIdentifier(const QString&);
    QString identifier(void) const;

    void setName(const QString&);
    QString name(void) const;

signals:
    void colorMapChanged(const QMap<double, QColor>&);
    void identifierChanged(const QString&);
    void nameChanged(const QString&);

public:
    gnomonCoreParameterColorMap *parameter(void) override;

public:
    void notifyColorMap(const QMap<double, QColor>&);
    void notifyName(const QString&);

private:
    gnomonCoreParameterColorMap *m_param = nullptr;
};

//
// gnomonCoreParameterColorObject.h ends here
