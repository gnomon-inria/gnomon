#pragma once

#include "gnomonCoreExport.h"

#include <QtCore>
#include <dtkCorePlugin>

class GNOMONCORE_EXPORT gnomonMeshAttribute
{
public:
 enum KindType : unsigned char {
        Scalar = 1,
        Vector = 3,
        Tensor = 9,
        Uncknown = 99
    };
 enum SupportType : unsigned char {
     Point = 1,
     Cell = 2
 };

public:
gnomonMeshAttribute() : m_name(""), m_kind(KindType::Scalar), m_support(SupportType::Point) {;}
    gnomonMeshAttribute(const QString& name, const KindType& kind, const SupportType& support,const std::vector<double>& data) : m_name(name), m_kind(kind), m_support(support), m_data(data) {;}
    ~gnomonMeshAttribute() {;}

public:
    gnomonMeshAttribute(const gnomonMeshAttribute& other) {
        m_name = other.m_name;
        m_kind = other.m_kind;
        m_support = other.m_support;
        m_data = other.m_data;
    }

public:
    QString m_name;
    KindType m_kind; // 1 scalar, 3 vector, 9 tensor
    SupportType m_support; // 1 = point, 2 = cell, other = custom
    std::vector<double> m_data;
};

// QDebug
inline QDebug operator<<(QDebug dbg, const gnomonMeshAttribute& data) {
    dbg.nospace() << data.m_name << "kind:" << data.m_kind << "support:" << data.m_support << "data:" << data.m_data;
    return dbg.space();
}
inline QDebug operator<<(QDebug dbg, gnomonMeshAttribute *data) {
    dbg.nospace() << data->m_name << "kind:" << data->m_kind << "support:" << data->m_support << "data:" << data->m_data;;
    return dbg.space();
}

inline QDataStream& operator<<(QDataStream& s, const gnomonMeshAttribute& data) {
    s << data.m_name;
    s << data.m_kind;
    s << data.m_support ;
    s << data.m_data;
    return s;
}

inline QDataStream& operator>>(QDataStream& s, gnomonMeshAttribute& data)       {
    s >> data.m_name;
    s >> data.m_kind;
    s >> data.m_support ;
    s >> data.m_data;
    return s;
}


DTK_DECLARE_OBJECT(gnomonMeshAttribute*)
