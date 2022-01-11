// gnomonCoreParameterColor.h
//

#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtGui>

#include <dtkCore/dtkCoreParameter>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorMap declaration
// ///////////////////////////////////////////////////////////////////

using gnomonColorMap = QMap<double, QColor>;

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterColorMap : public dtkCoreParameterBase<gnomonCoreParameterColorMap>
{
public:

    using self_type = gnomonCoreParameterColorMap;
    using base_type = dtkCoreParameterBase<self_type>;

     gnomonCoreParameterColorMap(void);
     gnomonCoreParameterColorMap(const dtkCoreParameter *);
     gnomonCoreParameterColorMap(const QString& label, const gnomonColorMap& color_map, const QString& doc = QString());
     gnomonCoreParameterColorMap(const QString& label, const QString& color_map_id, const QString& doc = QString());
     gnomonCoreParameterColorMap(const QVariant&);
     gnomonCoreParameterColorMap(const gnomonCoreParameterColorMap&);
    ~gnomonCoreParameterColorMap(void);

    gnomonCoreParameterColorMap& operator = (const dtkCoreParameter *);
    gnomonCoreParameterColorMap& operator = (const QVariant&);
    gnomonCoreParameterColorMap& operator = (const gnomonCoreParameterColorMap&);

    void setValue(const gnomonColorMap&);
    void setValue(const QString&);
    void setValue(const QVariant&) override;

    void setName(const QString&);

    gnomonColorMap value(void) const;
    QString name(void) const;

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

    gnomonColorMap m_c;
    QString m_n = QStringLiteral("no name");

private:
    class gnomonCoreParameterColorMapObject *m_object = nullptr;
};

DTK_DECLARE_PARAMETER(gnomonCoreParameterColorMap);

//
// gnomonCoreParameterColor.h ends here
