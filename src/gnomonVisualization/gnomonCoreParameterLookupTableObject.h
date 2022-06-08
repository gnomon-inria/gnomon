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
    Q_PROPERTY(double valueRangeMin READ valueRangeMin WRITE setValueRangeMin NOTIFY valueRangeMinChanged BINDABLE bindableValueRangeMin)
    Q_PROPERTY(double valueRangeMax READ valueRangeMax WRITE setValueRangeMax NOTIFY valueRangeMaxChanged BINDABLE bindableValueRangeMax)
    Q_PROPERTY(bool visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged BINDABLE bindableVisibility)
    Q_PROPERTY(QStringList availableCluts READ availableCluts CONSTANT)

public:
     gnomonCoreParameterLookupTableObject(gnomonCoreParameterLookupTable *);
    ~gnomonCoreParameterLookupTableObject(void);

    void setColorMap(const QVariantMap&);
    QVariantMap colorMap(void) const;

    void setColorMapName(const QString&);
    QString colorMapName(void) const;

    void setValueRangeMin(double);
    double valueRangeMin(void) const;
    QBindable<double> bindableValueRangeMin(void);

    void setValueRangeMax(double);
    double valueRangeMax(void) const;
    QBindable<double> bindableValueRangeMax(void);

    void setVisibility(bool);
    bool visibility(void) const;
    QBindable<bool> bindableVisibility(void);

public:
    QStringList availableCluts(void) const;

signals:
    void valueChanged(const gnomonLookupTable&);
    void colorMapChanged(const QVariantMap&);
    void colorMapNameChanged(const QString&);
    void valueRangeMinChanged(double);
    void valueRangeMaxChanged(double);
    void visibilityChanged(bool);

public:
    gnomonCoreParameterLookupTable *parameter(void) override;

public:
    void notifyColorMap(const gnomonColorMap&);
    void notifyColorMapName(const QString&);
    void notifyValueRangeMin(double);
    void notifyValueRangeMax(double);
    void notifyVisibility(bool);

private:
    gnomonCoreParameterLookupTable *m_param = nullptr;
};

//
// gnomonCoreParameterLookupTableObject.h ends here
