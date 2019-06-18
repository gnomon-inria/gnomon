// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonWidgetsExport>

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
    ~gnomonLookupTable(void) = default;

    const QString& colorMapName(void) const;
    const QMap<double, QColor>& colorMap(void) const;
    const QList<double>& valueRange(void) const;
    bool visibility(void) const;

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

//
// gnomonLookupTable.h ends here