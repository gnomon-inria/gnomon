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

#include <gnomonWidgetsExport.h>

#include <dtkCoreParameter>

#include "gnomonLookupTable.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColormap
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonCoreParameterColorMap : public dtkCoreParameterBase<gnomonCoreParameterColorMap>
{
public:
     gnomonCoreParameterColorMap(void) = default;
     gnomonCoreParameterColorMap(const QMap<double, QColor>& c, const QString& doc = QString());
     gnomonCoreParameterColorMap(const QString& clut, const QString& doc = QString());
     gnomonCoreParameterColorMap(const gnomonCoreParameterColorMap&);
    ~gnomonCoreParameterColorMap(void) = default;

    QMap<double, QColor> value(void) const;
    QString name(void) const;

    void setValue(const QMap<double, QColor>&);
    void setValue(const QString&);
    void setValue(const QVariant&) override;

    void setName(const QString&);

    gnomonCoreParameterColorMap& operator = (const QVariant&);
    gnomonCoreParameterColorMap& operator = (const gnomonCoreParameterColorMap&);

    QVariantHash toVariantHash(void) const override;

    using dtkCoreParameter::castToTop;
    using dtkCoreParameter::documentation;
    using dtkCoreParameter::setDocumentation;
    using dtkCoreParameter::label;
    using dtkCoreParameter::setLabel;

private:
    using dtkCoreParameter::m_label;
    using dtkCoreParameter::m_doc;

    QMap<double, QColor> m_c;
    QString m_n;
};

Q_DECLARE_METATYPE(gnomonCoreParameterColorMap);
Q_DECLARE_METATYPE(gnomonCoreParameterColorMap *);

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterLookupTable
// ///////////////////////////////////////////////////////////////////
/*
class GNOMONWIDGETS_EXPORT gnomonCoreParameterLookupTable : public dtkCoreParameterBase<gnomonCoreParameterLookupTable>
{
public:
    gnomonCoreParameterLookupTable(void) = default;
    gnomonCoreParameterLookupTable(const gnomonLookupTable& lut, const QString& doc = QString());
    gnomonCoreParameterLookupTable(const gnomonCoreParameterLookupTable&);
    ~gnomonCoreParameterLookupTable(void) = default;

    gnomonLookupTable value(void) const;

    void setValue(const gnomonLookupTable&);

    gnomonCoreParameterLookupTable& operator = (const QVariant&);
    gnomonCoreParameterLookupTable& operator = (const gnomonCoreParameterLookupTable&);

    QVariantHash toVariantHash(void) const override;

private:
    using dtkCoreParameter::m_label;
    using dtkCoreParameter::m_doc;
    gnomonLookupTable m_l;
};

Q_DECLARE_METATYPE(gnomonCoreParameterLookupTable);
Q_DECLARE_METATYPE(gnomonCoreParameterLookupTable *);
*/
//
// dtkCoreParameter.h ends here
