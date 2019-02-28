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

#include <gnomonVisualizationExport.h>
#include <gnomonCore/gnomonCoreParameter>

#include "gnomonLookupTable.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColormap
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterColorMap : public gnomonCoreParameter
{
public:
     gnomonCoreParameterColorMap(const QMap<double, QColor>& c, const QString& doc = QString());
     gnomonCoreParameterColorMap(const QString& clut, const QString& doc = QString());
    ~gnomonCoreParameterColorMap(void) = default;

    QMap<double, QColor> value(void) const;
    QString name(void) const;

    void setValue(const QMap<double, QColor>&);
    void setValue(const QString&);

    void copy(gnomonCoreParameter *other);

private:
    QMap<double, QColor> m_c;
    QString m_n;
};

Q_DECLARE_METATYPE(gnomonCoreParameterColorMap *);

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColormap
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterLookupTable : public gnomonCoreParameter
{
public:
     gnomonCoreParameterLookupTable(const gnomonLookupTable& lut, const QString& doc = QString());
    ~gnomonCoreParameterLookupTable(void) = default;

    gnomonLookupTable value(void) const;

    void setValue(const gnomonLookupTable&);

    void copy(gnomonCoreParameter *other);

private:
    gnomonLookupTable m_l;
};

Q_DECLARE_METATYPE(gnomonCoreParameterLookupTable *);

//
// gnomonCoreParameter.h ends here