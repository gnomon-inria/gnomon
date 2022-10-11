// gnomonCoreParameterGraphical.h
//

#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtGui>

#include <dtkCore/dtkCoreParameter>


class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterGraphical : public dtkCoreParameterBase<gnomonCoreParameterGraphical>
{
public:

    using self_type = gnomonCoreParameterGraphical;
    using base_type = dtkCoreParameterBase<self_type>;

    gnomonCoreParameterGraphical(void);
    gnomonCoreParameterGraphical(const QString& label);
    gnomonCoreParameterGraphical(const dtkCoreParameter *);

    ~gnomonCoreParameterGraphical(void);

public:
    void setName(const QString&);
    QString name(void) const;

    void setValue(const QString& id);
    void setValue(const QVariant&) override;


public:
    QVariantHash toVariantHash(void) const override;

    dtkCoreParameterObject *object(void) override;

    using dtkCoreParameter::documentation;
    using dtkCoreParameter::setDocumentation;
    using dtkCoreParameter::label;
    using dtkCoreParameter::setLabel;

private:
    using dtkCoreParameter::m_label;

    QString m_n = QStringLiteral("no name");

private:
    class gnomonCoreParameterGraphicalObject *m_object = nullptr;

};

GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream&, const gnomonCoreParameterGraphical&);
GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream&, gnomonCoreParameterGraphical&);
GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug, gnomonCoreParameterGraphical);

DTK_DECLARE_PARAMETER(gnomonCoreParameterGraphical);

//
// gnomonCoreParameterGraphical.h ends here
