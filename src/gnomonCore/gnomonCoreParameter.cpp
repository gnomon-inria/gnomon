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

gnomonCoreParameter::gnomonCoreParameter(const QString& doc) : QObject(), m_doc(doc)
{
}

QString gnomonCoreParameter::doc(void) const
{
    return m_doc;
}

void gnomonCoreParameter::setValue(const QVariant& v)
{
    return;
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
    if(m_value != val) {
        m_value = val;
        emit valueChanged();
    }
}

void gnomonCoreParameterBool::setValue(const QVariant& v)
{
    if(m_value != v.toBool()) {
        m_value = v.toBool();
        emit valueChanged();
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterString
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterString::gnomonCoreParameterString(const QString& value, const QString& doc) : gnomonCoreParameter(doc)
{
    m_values.append(value);
    this->setCurrentIndex(m_values.indexOf(value));
}

gnomonCoreParameterString::gnomonCoreParameterString(const QString& value, const QStringList& values, const QString& doc) : gnomonCoreParameter(doc), m_values(values)
{
    Q_ASSERT(m_values.contains(value));
    this->setCurrentIndex(m_values.indexOf(value));
}

gnomonCoreParameterString::gnomonCoreParameterString(const QStringList& values, const QString& doc) : gnomonCoreParameter(doc), m_values(values)
{
}

int gnomonCoreParameterString::size(void) const
{
    return m_values.size();
}

int gnomonCoreParameterString::currentIndex(void) const
{
    return m_current_index;
}

// QString gnomonCoreParameterString::currentValue(void) const
QString gnomonCoreParameterString::value(void) const
{
    if (!m_values.empty()) {
        return m_values.at(m_current_index);
    }
    return QString();
}

QStringList gnomonCoreParameterString::values(void) const
{
    return m_values;
}

void gnomonCoreParameterString::setCurrentIndex(int current_index)
{
    if(m_current_index != current_index) {
        m_current_index = current_index;
        emit valueChanged();
    }
}

void gnomonCoreParameterString::addValue(const QString& val)
{
    if (!m_values.contains(val)) {
        m_values.append(val);
    };
}

void gnomonCoreParameterString::removeValue(const QString& val)
{
    m_values.removeAll(val);
    if (m_values.empty()) {
        m_current_index = 0;
    } else if (m_current_index == m_values.size()) {
        --m_current_index;
    }
}

void gnomonCoreParameterString::setValues(const QStringList& val)
{
    QString c_val = m_values.at(m_current_index);

    m_values.clear();
    for (const auto& v : val) {
        if (!m_values.contains(v)) {
            m_values.append(v);
        }
    }

    int index = m_values.indexOf(c_val);
    if(index >= 0) {
        m_current_index = index;
        emit valueChanged();
    }
}

void gnomonCoreParameterString::setValue(const QString& val)
{
    int index = m_values.indexOf(val);
    if(index >= 0) {
        if(m_current_index != index) { 
            m_current_index = index; 
            emit valueChanged();
        }
    }
}

void gnomonCoreParameterString::setValue(const QVariant& v)
{
    QString val = v.toString();
    int index = m_values.indexOf(val);
    if(index >= 0){ 
        if(m_current_index != index) { 
            m_current_index = index; 
            emit valueChanged();
        }
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterStringList
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterStringList::gnomonCoreParameterStringList(const QStringList& value, const QString& doc) : gnomonCoreParameter(doc), m_value(value), m_values(value)
{
}

gnomonCoreParameterStringList::gnomonCoreParameterStringList(const QStringList& value, const QStringList& values, const QString& doc) : gnomonCoreParameter(doc), m_values(values)
{
    for (const auto& v : value) {
        if (m_values.contains(v)) {
            m_value.append(v);
        }
    }
}

int gnomonCoreParameterStringList::size(void) const
{
    return m_values.size();
}


QStringList gnomonCoreParameterStringList::value(void) const
{
    return m_value;
}

QStringList gnomonCoreParameterStringList::values(void) const
{
    return m_values;
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
    m_value.removeAll(val);
}

void gnomonCoreParameterStringList::setValues(const QStringList& val)
{
    m_values.clear();
    for (const auto& v : val) {
        if (!m_values.contains(v)) {
            m_values.append(v);
        }
    }

    for (const auto& v : m_value) {
        if (!m_values.contains(v)) {
            m_value.removeAll(v);
        }
    }
}

void gnomonCoreParameterStringList::setValue(const QStringList& value)
{
    m_value.clear();
    for (const auto& v : value) {
        if (m_values.contains(v)) {
            m_value.append(v);
        }
    }
}

void gnomonCoreParameterStringList::setValue(const QVariant& v)
{
    QStringList val = v.toStringList();
    this->setValue(val);
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
    if(m_v != v) {
        m_v = v;
        emit valueChanged();
    }
}

//
// gnomonCoreParameter.cpp ends here
