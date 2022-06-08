#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtGui>

// ///////////////////////////////////////////////////////////////////
// gnomonLookupTableColormap
// ///////////////////////////////////////////////////////////////////

using gnomonColorMap = QMap<double, QColor>;

class GNOMONVISUALIZATION_EXPORT gnomonLookupTable : public QObject
{
    Q_OBJECT

public:
    gnomonLookupTable(void) = default;
    gnomonLookupTable(const gnomonColorMap& c, const QList<double> r={0,1}, bool v=true);
    gnomonLookupTable(const QString& clut, const QList<double> r={0,1}, bool v=true);
    gnomonLookupTable(const gnomonLookupTable&);

    ~gnomonLookupTable(void) = default;

    const QString& colorMapName(void) const;
    const gnomonColorMap& colorMap(void) const;
    double valueMin(void) const;
    QBindable<double> bindableValueMin(void);
    double valueMax(void) const;
    QBindable<double> bindableValueMax(void);
    //const QList<double>& valueRange(void) const;
    bool visibility(void) const;
    QBindable<bool> bindableVisibility(void);

    //gnomonCoreParameterColorMap& operator = (const QVariant&);
    gnomonLookupTable& operator = (const gnomonLookupTable&);
    bool operator != (const gnomonLookupTable&);

    void setName(const QString&);
    void setColorMap(const gnomonColorMap&);
    void setColorMap(const QString&);
    void setValueMin(double);
    void setValueMax(double);
    //void setValueRange(const QList<double>&);
    void setVisibility(bool);

private:
	QString name;
    gnomonColorMap colormap;
    //QList<double> value_range;
    Q_OBJECT_BINDABLE_PROPERTY(gnomonLookupTable, double, min);
    Q_OBJECT_BINDABLE_PROPERTY(gnomonLookupTable, double, max);
    //bool visible;
    Q_OBJECT_BINDABLE_PROPERTY(gnomonLookupTable, bool, visible);
};

Q_DECLARE_METATYPE(gnomonLookupTable)
Q_DECLARE_METATYPE(gnomonLookupTable *)


//
// gnomonLookupTable.h ends here
