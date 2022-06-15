// gnomonCoreParameterLookupTableObject.h
//

#pragma once

#include <gnomonVisualizationExport>

#include <dtkCore/dtkCoreParameterObject>

#include "gnomonCoreParameterLookupTable.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterLookupTableObject declaration
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterLookupTableObject : public dtkCoreParameterObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap colorMap READ colorMap WRITE setColorMap NOTIFY colorMapChanged)
    Q_PROPERTY(QString colorMapName READ colorMapName WRITE setColorMapName NOTIFY colorMapNameChanged)
    Q_PROPERTY(double valueMin READ valueMin WRITE setValueMin NOTIFY valueMinChanged)
    Q_PROPERTY(double valueMax READ valueMax WRITE setValueMax NOTIFY valueMaxChanged)
    Q_PROPERTY(double from READ rangeMin WRITE setRangeMin NOTIFY rangeMinChanged)
    Q_PROPERTY(double to READ rangeMax WRITE setRangeMax NOTIFY rangeMaxChanged)
    Q_PROPERTY(bool visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
    Q_PROPERTY(QStringList availableCluts READ availableCluts CONSTANT)

public:
     gnomonCoreParameterLookupTableObject(gnomonCoreParameterLookupTable *);
    ~gnomonCoreParameterLookupTableObject(void);

    void setColorMap(const QVariantMap&);
    QVariantMap colorMap(void) const;

    void setColorMapName(const QString&);
    QString colorMapName(void) const;

    void setValueMin(double);
    double valueMin(void) const;

    void setValueMax(double);
    double valueMax(void) const;

    void setRangeMin(double);
    double rangeMin(void) const;

    void setRangeMax(double);
    double rangeMax(void) const;

    void setVisibility(bool);
    bool visibility(void) const;

public:
    QStringList availableCluts(void) const;

signals:
    void valueChanged(const gnomonLookupTable&);
    void colorMapChanged(const QVariantMap&);
    void colorMapNameChanged(const QString&);
    void valueMinChanged(double);
    void valueMaxChanged(double);
    void rangeMinChanged(double);
    void rangeMaxChanged(double);
    void visibilityChanged(bool);

public:
    gnomonCoreParameterLookupTable *parameter(void) override;

public:
    void notifyColorMap(const gnomonColorMap&);
    void notifyColorMapName(const QString&);
    void notifyValueMin(double);
    void notifyValueMax(double);
    void notifyRangeMin(double);
    void notifyRangeMax(double);
    void notifyVisibility(bool);

private:
    gnomonCoreParameterLookupTable *m_param = nullptr;
};

//
// gnomonCoreParameterLookupTableObject.h ends here
