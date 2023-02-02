//

#pragma once

#include <gnomonVisualizationExport>
#include "gnomonCore/gnomonCorePlugin.h"

#include <QtCore>
#include <QtGui>

#include <dtkCore/dtkCoreParameter>

#include "gnomonLookupTable.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterLookupTable declaration
// ///////////////////////////////////////////////////////////////////


class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterLookupTable : public dtkCoreParameterBase<gnomonCoreParameterLookupTable>
{
public:

    using self_type = gnomonCoreParameterLookupTable;
    using base_type = dtkCoreParameterBase<self_type>;

     gnomonCoreParameterLookupTable(void);
     gnomonCoreParameterLookupTable(const dtkCoreParameter *);
     gnomonCoreParameterLookupTable(const QString& label, const gnomonLookupTable& color_map, const QString& doc = QString());
     gnomonCoreParameterLookupTable(const QVariant&);
     gnomonCoreParameterLookupTable(const gnomonCoreParameterLookupTable&);
    ~gnomonCoreParameterLookupTable(void);

    gnomonCoreParameterLookupTable& operator = (const dtkCoreParameter *);
    gnomonCoreParameterLookupTable& operator = (const QVariant&);
    gnomonCoreParameterLookupTable& operator = (const gnomonCoreParameterLookupTable&);

    void setColorMap(const gnomonColorMap&);
    void setColorMap(const QString&);

    void setValue(const QVariant&) override;
    void setValue(const gnomonLookupTable&);

    void setName(const QString&);

    void setValueMin(double);
    void setValueMax(double);
    void setRangeMin(double);
    void setRangeMax(double);
    void setVisibility(bool);

    gnomonLookupTable value(void) const;
    gnomonColorMap colorMap(void) const;
    QString name(void) const;

    double valueMin(void) const;
    double valueMax(void) const;
    double rangeMin(void) const;
    double rangeMax(void) const;
    bool visibility(void) const;

public:
    QStringList availableCluts(void);

public:
    QVariantHash toVariantHash(void) const override;

    dtkCoreParameterObject *object(void) override;

    using dtkCoreParameter::documentation;
    using dtkCoreParameter::setDocumentation;
    using dtkCoreParameter::label;
    using dtkCoreParameter::setLabel;

private:
    using dtkCoreParameter::m_label;
    using dtkCoreParameter::m_doc;

    gnomonLookupTable m_l;

private:
    class gnomonCoreParameterLookupTableObject *m_object = nullptr;
};

GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream&, const gnomonCoreParameterLookupTable&);
GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream&, gnomonCoreParameterLookupTable&);
GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug, gnomonCoreParameterLookupTable);

DTK_DECLARE_PARAMETER(gnomonCoreParameterLookupTable);

//
// gnomonCoreParameterLookupTable.h ends here
