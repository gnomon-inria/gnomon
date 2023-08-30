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
    Q_PROPERTY(gnomonColorMap colorMap READ colorMap WRITE setColorMap NOTIFY colorMapChanged)
    Q_PROPERTY(QVariantMap value READ value WRITE setValue NOTIFY valueChanged);
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(QStringList availableCluts READ availableCluts CONSTANT)

public:
     gnomonCoreParameterColorMapObject(gnomonCoreParameterColorMap *);
    ~gnomonCoreParameterColorMapObject(void);

    void setColorMap(const gnomonColorMap&);
    gnomonColorMap colorMap(void) const;

    void setIdentifier(const QString&);
    QString identifier(void) const;

    void setValue(const QVariantMap&);
    QVariantMap value(void) const;

    void setName(const QString&);
    QString name(void) const;

public:
    Q_INVOKABLE QVariantMap getColorMap(const QString &clut);

public:
    QStringList availableCluts(void) const;

signals:
    void colorMapChanged(const gnomonColorMap&);
    void identifierChanged(const QString&);
    void valueChanged(const QVariantMap&);
    void nameChanged(const QString&);

public:
    gnomonCoreParameterColorMap *parameter(void) override;

public:
    void notifyColorMap(const gnomonColorMap&);
    void notifyName(const QString&);

private:
    gnomonCoreParameterColorMap *m_param = nullptr;
};

//
// gnomonCoreParameterColorObject.h ends here
