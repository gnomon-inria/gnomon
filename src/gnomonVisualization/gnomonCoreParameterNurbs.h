#pragma once

#include <gnomonVisualizationExport>
#include "gnomonCore/gnomonCorePlugin.h"

#include <QtCore>
#include <QtGui>

#include <dtkCore/dtkCoreParameter>

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterNurbs : public dtkCoreParameterBase<gnomonCoreParameterNurbs>
{
public:
    enum NURBS_TYPE {
        CURVE,
        FUNCTION,
        SURFACE
    };
public:

    using self_type = gnomonCoreParameterNurbs;
    using base_type = dtkCoreParameterBase<self_type>;
    using ctrls_type = QList<std::array<double, 3>>;

    gnomonCoreParameterNurbs(void);
    gnomonCoreParameterNurbs(const QString& label,const ctrls_type control_points, int dimension, gnomonCoreParameterNurbs::NURBS_TYPE nurbs_type, const QList<double>& ctrl_points_size, const QString& doc = QString());
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
    QList<double> cpsize(void) const;

    int degree(void) const;
    int dimension(void) const;
    double delta(void) const;
    bool is_function(void) const;
    NURBS_TYPE type(void) const;

    void setDegree(int);
    void setDelta(double);
    void setDimension(int);
    void setIs_function(bool);
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
    bool m_is_function = false;
    NURBS_TYPE m_nurbs_type = NURBS_TYPE::CURVE;
    QList<double> m_ctrl_points_size;

    QString m_n = QStringLiteral("no name");

private:
    class gnomonCoreParameterNurbsObject *m_object = nullptr;

};

GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream&, const gnomonCoreParameterNurbs&);
GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream&, gnomonCoreParameterNurbs&);
GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug, gnomonCoreParameterNurbs);

DTK_DECLARE_PARAMETER(gnomonCoreParameterNurbs);
