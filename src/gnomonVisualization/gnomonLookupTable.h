#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtGui>

// ///////////////////////////////////////////////////////////////////
// gnomonLookupTableColormap
// ///////////////////////////////////////////////////////////////////

using gnomonColorMap = QMap<double, QColor>;

class GNOMONVISUALIZATION_EXPORT gnomonLookupTable
{

public:
    gnomonLookupTable(void) = default;
    gnomonLookupTable(const gnomonColorMap& c, const QList<double> a={0,1}, const QList<double> r={0,1}, bool v=true);
    gnomonLookupTable(const QString& clut, const QList<double> a={0,1}, const QList<double> r={0,1}, bool v=true);
    gnomonLookupTable(const gnomonLookupTable&);

    ~gnomonLookupTable(void) = default;

    const QString& colorMapName(void) const;
    const gnomonColorMap& colorMap(void) const;
    double valueMin(void) const;
    double rangeMin(void) const;
    double valueMax(void) const;
    double rangeMax(void) const;
    bool visibility(void) const;

    //gnomonCoreParameterColorMap& operator = (const QVariant&);
    gnomonLookupTable& operator = (const gnomonLookupTable&);
    bool operator != (const gnomonLookupTable&);

    void setName(const QString&);
    void setColorMap(const gnomonColorMap&);
    void setColorMap(const QString&);
    void setValueMin(double);
    void setRangeMin(double);
    void setValueMax(double);
    void setRangeMax(double);
    void setVisibility(bool);

private:
	QString name;
    gnomonColorMap colormap;
    double min;
    double r_min;
    double max;
    double r_max;
    bool visible;
};

Q_DECLARE_METATYPE(gnomonLookupTable)
Q_DECLARE_METATYPE(gnomonLookupTable *)


//
// gnomonLookupTable.h ends here
