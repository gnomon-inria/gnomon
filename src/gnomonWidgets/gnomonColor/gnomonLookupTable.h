#pragma once

#include <gnomonWidgetsExport>

#include <dtkCore/dtkCoreParameterSimple.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include <QtCore>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonLookupTableColormap
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonLookupTable
{

public:
    gnomonLookupTable(void) = default;
    gnomonLookupTable(const QMap<double, QColor>& c, const QList<double> r={0,1}, bool v=true);
    gnomonLookupTable(const QString& clut, const QList<double> r={0,1}, bool v=true);
    gnomonLookupTable(const gnomonLookupTable&);

    ~gnomonLookupTable(void) = default;

    const QString& colorMapName(void) const;
    const QMap<double, QColor>& colorMap(void) const;
    const QList<double>& valueRange(void) const;
    bool visibility(void) const;

    //gnomonCoreParameterColorMap& operator = (const QVariant&);
    gnomonLookupTable& operator = (const gnomonLookupTable&);
    bool operator != (const gnomonLookupTable&);

    void setName(const QString&);
    void setColorMap(const QMap<double, QColor>&);
    void setColorMap(const QString&);
    void setValueRange(const QList<double>&);
    void setVisibility(bool);

private:
	QString name;
    QMap<double, QColor> colormap;
    QList<double> value_range;
    bool visible;
};

Q_DECLARE_METATYPE(gnomonLookupTable)
Q_DECLARE_METATYPE(gnomonLookupTable *)

using gnomonCoreParameterLookupTable = dtkCoreParameterSimple<gnomonLookupTable>;
DTK_DECLARE_PARAMETER(gnomonCoreParameterLookupTable)

//
// gnomonLookupTable.h ends here