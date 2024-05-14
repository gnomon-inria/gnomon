#include "gnomonCoreParameterNurbs.h"
#include "gnomonCoreParameterNurbsObject.h"
#include <QtXml>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterNurbs
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterNurbs::gnomonCoreParameterNurbs(void) : dtkCoreParameterBase<gnomonCoreParameterNurbs>()
{
    m_object = new gnomonCoreParameterNurbsObject(this);
}

gnomonCoreParameterNurbs::gnomonCoreParameterNurbs(const QString& label,const ctrls_type control_points, int dimension, NURBS_TYPE nurbs_type, const QString& doc) : dtkCoreParameterBase<gnomonCoreParameterNurbs>()
{
    m_label = label;
    m_doc = doc;
    m_ctrl_points = control_points;
    m_dimension = dimension;
    m_nurbs_type = nurbs_type;
    if(nurbs_type == NURBS_TYPE::FUNCTION)
        m_is_function = true;

    //set dimension before creating the object
    m_object = new gnomonCoreParameterNurbsObject(this);
}

gnomonCoreParameterNurbs::gnomonCoreParameterNurbs(const dtkCoreParameter *p):  dtkCoreParameterBase<gnomonCoreParameterNurbs>()
{
    if(!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        m_object = new gnomonCoreParameterNurbsObject(this);
        return;
    }
    *this = p->variant();
    m_object = new gnomonCoreParameterNurbsObject(this);
}

gnomonCoreParameterNurbs::gnomonCoreParameterNurbs(const QVariant& v) : dtkCoreParameterBase<gnomonCoreParameterNurbs>()
{
    if(v.canConvert<gnomonCoreParameterNurbs>()) {
        auto o(v.value<gnomonCoreParameterNurbs>());
        *this = o;
    } else if(v.canConvert<QVariantList>()) {
        this->setValue(v);
    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::fromType<gnomonCoreParameterNurbs>().name()
                  << ". Nothing is done.";
    }
    m_object = new gnomonCoreParameterNurbsObject(this);
}


gnomonCoreParameterNurbs::gnomonCoreParameterNurbs(const gnomonCoreParameterNurbs& o) : dtkCoreParameterBase<gnomonCoreParameterNurbs>(o), m_ctrl_points(o.m_ctrl_points)
{

    m_dimension = o.m_dimension;
    m_degree = o.m_degree;
    m_delta = o.m_delta;
    m_is_function = o.m_is_function;
    m_nurbs_type = o.m_nurbs_type;
    m_object = new gnomonCoreParameterNurbsObject(this);
}

gnomonCoreParameterNurbs::~gnomonCoreParameterNurbs(void)
{
    delete m_object;
}

gnomonCoreParameterNurbs& gnomonCoreParameterNurbs::operator = (const ctrls_type& ctrl_points)
{
    this->m_ctrl_points = ctrl_points;
    return *this;
}

gnomonCoreParameterNurbs& gnomonCoreParameterNurbs::operator = (const QVariant& v)
{
    if(v.canConvert<gnomonCoreParameterNurbs>()) {
        *this = v.value<gnomonCoreParameterNurbs>();
    } else if(v.canConvert<QVariantList>()) {
        this->setValue(v);
    } else if(v.canConvert<QVariantHash>()) {
        auto hash = v.toHash();
        m_label = hash["label"].toString();
        m_doc = hash["doc"].toString();
        m_dimension = hash["dimension"].toInt();
        m_degree = hash["degree"].toInt();
        m_delta = hash["delta"].toDouble();
        m_is_function = hash["is_function"].toBool();

        this->setValue(hash["ctrl_points"]);
    }

   return *this;
}

gnomonCoreParameterNurbs& gnomonCoreParameterNurbs::operator = (const gnomonCoreParameterNurbs& o)
{
    if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_ctrl_points = o.m_ctrl_points;
        m_dimension = o.m_dimension;
        m_degree = o.m_degree;
        m_delta = o.m_delta;
        m_is_function = o.m_is_function;
        m_nurbs_type = o.m_nurbs_type;
    }

    return *this;
}

void gnomonCoreParameterNurbs::setControlPoints(const ctrls_type& ctrl_points)
{
    m_ctrl_points = ctrl_points;
    if(m_degree >= m_ctrl_points.size() ) {
        dtkWarn() << "degree " << m_degree << " is too big setting it to " << m_ctrl_points.size() - 1;
        this->setDegree(m_ctrl_points.size() -1);
    }
    m_object->notifyControlPointsChanged();
}

gnomonCoreParameterNurbs::ctrls_type gnomonCoreParameterNurbs::controlPoints(void) const
{
    return m_ctrl_points;
}

int gnomonCoreParameterNurbs::degree(void) const
{
    return m_degree;
}

bool gnomonCoreParameterNurbs::is_function(void) const
{
    return m_is_function;
}

int gnomonCoreParameterNurbs::type(void) const
{
    return m_nurbs_type;
}

void gnomonCoreParameterNurbs::setDegree(int degree)
{
    if(m_ctrl_points.size() <= degree) {
        dtkWarn() << "degree " << degree << " is not compatible with number of control points " << m_ctrl_points.size();
        return;
    }
    m_degree = degree;
}

double gnomonCoreParameterNurbs::delta(void) const
{
    return m_delta;
}

void gnomonCoreParameterNurbs::setDelta(double delta)
{
    m_delta = delta;
}

int gnomonCoreParameterNurbs::dimension(void) const
{
    return m_dimension;
}

void gnomonCoreParameterNurbs::setDimension(int dimension)
{
    m_dimension = dimension;
}

void gnomonCoreParameterNurbs::setIs_function(bool is_function)
{
    m_is_function = is_function;
}

void gnomonCoreParameterNurbs::setValue(const QVariant &v)
{
    if(v.canConvert<QVariantList>()) {
        m_ctrl_points.clear();
        auto list_ctrl_points = v.toList();
        for(auto point : list_ctrl_points) {
            auto l_point = point.toList();
            std::array<double, 3> vv = {0., 0., 1.};
            for(int i=0; i < l_point.count(); ++i) {
                vv[i] = l_point[i].toDouble();
            }
            m_ctrl_points.append(vv);

        }
    } else {
        dtkWarn() << Q_FUNC_INFO << "cannot convert variant to list"
        << v.metaType().name() << v;
    }
}

QVariantHash gnomonCoreParameterNurbs::toVariantHash(void) const
{
    QVariantHash hash = dtkCoreParameterBase<gnomonCoreParameterNurbs>::toVariantHash();
    QList<QVariant> list_ctrl_points;
    for(auto point: m_ctrl_points) {
        QList<QVariant> l_point;
        l_point << QVariant::fromValue(point[0])
                << QVariant::fromValue(point[1])
                << QVariant::fromValue(point[2]);
        list_ctrl_points << l_point;
    }
    hash.insert("ctrl_points", list_ctrl_points);
    hash.insert("dimension", m_dimension);
    hash.insert("degree", m_degree);
    hash.insert("delta", m_delta);
    hash.insert("is_function", m_is_function);

    return hash;
}

dtkCoreParameterObject *gnomonCoreParameterNurbs::object(void)
{
    return m_object;
}


GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream& s, const gnomonCoreParameterNurbs& p)
{
    s << p.label();
    s << p.documentation();
    s << "dimension" << p.dimension();
    s << "degree" << p.degree();
    s << "delta" << p.delta();
    s << "is_function" << p.is_function();
    s << "control points: {";
    for(auto point: p.controlPoints()) {
        s << "(" << point[0] << " , " << point[1] << " , " << point[2] << ") ,";
    }
    s << "}";
    return s;
}

GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream& s, gnomonCoreParameterNurbs& p)
{
    QString label; s >> label;
    QString doc;  s >> doc;
    int dimension; s >> dimension;
    int degree; s >> degree;
    double delta; s >> delta;
    bool is_function; s >> is_function;

    qDebug() << Q_FUNC_INFO << " Not DONE TODO";
    p = gnomonCoreParameterNurbs(label);
    //p = gnomonCoreParameterNurbs(label, .....  );
    return s;
}

GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug dbg, gnomonCoreParameterNurbs p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label " << p.label() << ", "
                  << "dimension " << p.dimension() << ", "
                  << "degree " << p.degree() << ", "
                  << "delta " << p.delta() << ", "
                  << "is_function " << p.is_function() << ", "
                  << "control_points: { ";

    for(auto point: p.controlPoints()) {
        dbg.nospace() << "(" << point[0] << " , " << point[1] << " , " << point[2] << "), ";
    }
    dbg.nospace() << "}, "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}


DTK_DEFINE_PARAMETER(gnomonCoreParameterNurbs, g_nurbs);

//
// gnomonCoreParameterNurbs.cpp ends here
