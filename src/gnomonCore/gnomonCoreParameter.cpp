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

#include "gnomonCoreParameter.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameter
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameter::gnomonCoreParameter(const QString& doc) : m_doc(doc)
{
}

QString gnomonCoreParameter::doc(void) const
{
    return m_doc;
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterBool
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterBool::gnomonCoreParameterBool(bool val, const QString& doc) : gnomonCoreParameter(doc), m_value(val)
{
}

bool gnomonCoreParameterBool::value(void) const
{
    return m_value;
}

void gnomonCoreParameterBool::setValue(bool val)
{
    m_value = val;
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterString
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterString::gnomonCoreParameterString(const QString& s, const QString& doc) : gnomonCoreParameter(doc), m_s(s)
{
}

QString gnomonCoreParameterString::value(void) const
{
    return m_s;
}

void gnomonCoreParameterString::setValue(const QString& s)
{
    m_s = s;
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterStringList
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterStringList::gnomonCoreParameterStringList(const QStringList& values, const QString& doc) : gnomonCoreParameter(doc), m_values(values)
{
}

int gnomonCoreParameterStringList::size(void) const
{
    return m_values.size();
}

int gnomonCoreParameterStringList::currentIndex(void) const
{
    return m_current_index;
}

QString gnomonCoreParameterStringList::currentValue(void) const
{
    if (!m_values.empty()) {
        return m_values.at(m_current_index);
    }
    return QString();
}

QStringList gnomonCoreParameterStringList::values(void) const
{
    return m_values;
}

void gnomonCoreParameterStringList::setCurrentIndex(int current_index)
{
    m_current_index = current_index;
}

void gnomonCoreParameterStringList::addValue(const QString& val)
{
    if (!m_values.contains(val)) {
        m_values.append(val);
    };
}

void gnomonCoreParameterStringList::removeValue(const QString& val)
{
    m_values.removeAll(val);
    if (m_values.empty()) {
        m_current_index = 0;
    } else if (m_current_index == m_values.size()) {
        --m_current_index;
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterVariant
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterVariant::gnomonCoreParameterVariant(const QVariant& v, const QString& doc) : gnomonCoreParameter(doc), m_v(v)
{
}

QVariant gnomonCoreParameterVariant::value(void) const
{
    return m_v;
}

void gnomonCoreParameterVariant::setValue(const QVariant& v)
{
    m_v = v;
}

//
// gnomonCoreParameter.cpp ends here
