#pragma once

#include <gnomonVisualizationExport>
#include "gnomonCore/gnomonCorePlugin.h"

#include <QtCore>
#include <QtGui>

#include <dtkCore/dtkCoreParameter>

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterNurbs : public dtkCoreParameterBase<gnomonCoreParameterNurbs>
{
public:

    using self_type = gnomonCoreParameterNurbs;
    using base_type = dtkCoreParameterBase<self_type>;
    using ctrls_type = QList<std::array<double, 3>>;

    gnomonCoreParameterNurbs(void);
    gnomonCoreParameterNurbs(const QString& label,const ctrls_type control_points, int dimension, const QString& doc = QString());
    gnomonCoreParameterNurbs(const dtkCoreParameter *);
    gnomonCoreParameterNurbs(const QVariant&);
    gnomonCoreParameterNurbs(const gnomonCoreParameterNurbs&);
    ~gnomonCoreParameterNurbs(void);


    gnomonCoreParameterNurbs& operator = (const ctrls_type&);
    gnomonCoreParameterNurbs& operator = (const QVariant&);
    gnomonCoreParameterNurbs& operator = (const gnomonCoreParameterNurbs&);

public:
    void setControlPoints(const ctrls_type& ctrl_points);
    ctrls_type controlPoints(void) const;

    int degree(void) const;
    int dimension(void) const;
    double delta(void) const;
    void setDegree(int);
    void setDelta(double);
    void setDimension(int);
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
    using dtkCoreParameter::m_doc;

    int m_degree = 2;
    int m_dimension = 2;
    double m_delta = 0.01;
    ctrls_type m_ctrl_points;

    QString m_n = QStringLiteral("no name");

private:
    class gnomonCoreParameterNurbsObject *m_object = nullptr;

};

GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream&, const gnomonCoreParameterNurbs&);
GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream&, gnomonCoreParameterNurbs&);
GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug, gnomonCoreParameterNurbs);

DTK_DECLARE_PARAMETER(gnomonCoreParameterNurbs);