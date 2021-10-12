#pragma once

#include <gnomonVisualizationExport.h>

#include <dtkCoreParameter>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterColorMap : public dtkCoreParameterBase<gnomonCoreParameterColorMap>
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
