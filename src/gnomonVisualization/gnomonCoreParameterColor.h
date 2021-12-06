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

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterColorMap : public dtkCoreParameterBase<gnomonCoreParameterColorMap>
{
public:
    using self_type = gnomonCoreParameterColorMap;
    using base_type = dtkCoreParameterBase<self_type>;

     gnomonCoreParameterColorMap(void);
     gnomonCoreParameterColorMap(const dtkCoreParameter *);
     gnomonCoreParameterColorMap(const QMap<double, QColor>& color_map, const QString& doc = QString());
     gnomonCoreParameterColorMap(const QString& color_map_id, const QString& doc = QString());
     gnomonCoreParameterColorMap(const QVariant&);
     gnomonCoreParameterColorMap(const gnomonCoreParameterColorMap&);
    ~gnomonCoreParameterColorMap(void);

    gnomonCoreParameterColorMap& operator = (const dtkCoreParameter *);
    gnomonCoreParameterColorMap& operator = (const QVariant&);
    gnomonCoreParameterColorMap& operator = (const gnomonCoreParameterColorMap&);

    void setValue(const QMap<double, QColor>&);
    void setValue(const QString&);
    void setValue(const QVariant&) override;

    void setName(const QString&);

    QMap<double, QColor> value(void) const;
    QString name(void) const;

    QVariantHash toVariantHash(void) const override;

    dtkCoreParameterObject *object(void) override;

    using dtkCoreParameter::documentation;
    using dtkCoreParameter::setDocumentation;
    using dtkCoreParameter::label;
    using dtkCoreParameter::setLabel;

private:
    using dtkCoreParameter::m_label;
    using dtkCoreParameter::m_doc;

    QMap<double, QColor> m_c;
    QString m_n = QStringLiteral("no name");

private:
    class gnomonCoreParameterColorMapObject *m_object = nullptr;
};

DTK_DECLARE_PARAMETER(gnomonCoreParameterColorMap);

//
// gnomonCoreParameterColor.h ends here
