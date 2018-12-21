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
#include <gnomonCore/gnomonCoreParameter>

#include "gnomonLookupTable.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColormap
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonCoreParameterColorMap : public gnomonCoreParameter
{
public:
     gnomonCoreParameterColorMap(const QMap<double, QColor>& c, const QString& doc = QString());
     gnomonCoreParameterColorMap(const QString& clut, const QString& doc = QString());
    ~gnomonCoreParameterColorMap(void) = default;

    QMap<double, QColor> value(void) const;

    void setValue(const QMap<double, QColor>&);
    void setValue(const QString&);

private:
    QMap<double, QColor> m_c;
};

Q_DECLARE_METATYPE(gnomonCoreParameterColorMap *);

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColormap
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonCoreParameterLookupTable : public gnomonCoreParameter
{
public:
     gnomonCoreParameterLookupTable(gnomonLookupTable *lut, const QString& doc = QString());
    ~gnomonCoreParameterLookupTable(void) = default;

    gnomonLookupTable *value(void) const;

    void setValue(gnomonLookupTable *);

private:
    gnomonLookupTable * m_l;
};

Q_DECLARE_METATYPE(gnomonCoreParameterLookupTable *);

//
// gnomonCoreParameter.h ends here