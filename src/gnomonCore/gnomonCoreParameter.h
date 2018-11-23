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

#pragma once

#include <gnomonCoreExport>

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameter
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCoreParameter : public QObject
{
    Q_OBJECT

public:
             gnomonCoreParameter(const QString& doc);
    virtual ~gnomonCoreParameter(void) = default;

    QString doc(void) const;

    virtual QString type(void) const = 0;

protected:
    QString m_doc;
};

// QDebug& operator<<(QDebug& stream, const gnomonCoreParameter& parameter)
// {
//     stream << parameter.type();

//     return stream;
// }

// QDebug& operator<<(QDebug& stream,       gnomonCoreParameter *parameter)
// {
//     stream << parameter->type();

//     return stream;
// }

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterNumeric
// ///////////////////////////////////////////////////////////////////

template <typename T, typename Enable = std::enable_if_t<std::is_arithmetic<T>::value>>
class GNOMONCORE_EXPORT gnomonCoreParameterNumeric : public gnomonCoreParameter
{
public:
     gnomonCoreParameterNumeric(T val, const QString& doc = QString()) :
         gnomonCoreParameter(doc),
         m_value(val) {}

     gnomonCoreParameterNumeric(T val, T min, T max, const QString& doc = QString()) :
         gnomonCoreParameter(doc),
         m_value(val),
         m_min(min),
         m_max(max) {}

    gnomonCoreParameterNumeric(T val, T min, T max, int accuracy, const QString& doc = QString()) :
         gnomonCoreParameter(doc),
         m_value(val),
         m_min(min),
         m_max(max),
         m_accuracy(accuracy) {}

    ~gnomonCoreParameterNumeric(void) = default;

    T value(void) const { return m_value; }
    T min(void) const { return m_min; }
    T max(void) const { return m_max; }
    int accuracy(void) { return m_accuracy; }

    void setValue(T val) { m_value = val; }
    void setMinimumValue(T min) { m_min = min; }
    void setMaximumValue(T max) { m_max = max; }
    void setAccuracy(int accuracy) { m_accuracy = accuracy; }

    QString type(void) const
    {
        return QMetaType::typeName(qMetaTypeId<T>());
    }

private:
    T m_value = T(0);
    int m_accuracy = 2;
    T m_min = std::numeric_limits<T>::min();
    T m_max = std::numeric_limits<T>::max();
};

using gnomonCoreParameterShort  = gnomonCoreParameterNumeric<short>;
using gnomonCoreParameterUShort = gnomonCoreParameterNumeric<unsigned short>;
using gnomonCoreParameterInt    = gnomonCoreParameterNumeric<int>;
using gnomonCoreParameterUInt   = gnomonCoreParameterNumeric<unsigned int>;
using gnomonCoreParameterLong   = gnomonCoreParameterNumeric<long>;
using gnomonCoreParameterULong  = gnomonCoreParameterNumeric<unsigned long>;
using gnomonCoreParameterDouble = gnomonCoreParameterNumeric<double>;

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterBool
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCoreParameterBool : public gnomonCoreParameter
{
public:
     gnomonCoreParameterBool(bool val, const QString& doc = QString());
    ~gnomonCoreParameterBool(void) = default;

    bool value(void) const;

    void setValue(bool);

    QString type(void) const
    {
        return "bool";
    }

private:
    bool m_value = false;
};

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterString
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCoreParameterString : public gnomonCoreParameter
{
public:
     gnomonCoreParameterString(const QString& s, const QString& doc = QString());
    ~gnomonCoreParameterString(void) = default;

    QString value(void) const;

    void setValue(const QString&);

    QString type(void) const
    {
        return "string";
    }

private:
    QString m_s;
};

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterStringList
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCoreParameterStringList : public gnomonCoreParameter
{
public:
     gnomonCoreParameterStringList(const QString& value, const QStringList& values, const QString& doc = QString());
     gnomonCoreParameterStringList(const QStringList& values, const QString& doc = QString());
    ~gnomonCoreParameterStringList(void) = default;

    int size(void) const;
    int currentIndex(void) const;
    QString currentValue(void) const;
    QStringList values(void) const;

    void setCurrentIndex(int);
    void addValue(const QString&);
    void removeValue(const QString&);

    QString type(void) const
    {
        return "stringlist";
    }

private:
    int m_current_index = 0;
    QStringList m_values;
};

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterVariant
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCoreParameterVariant : public gnomonCoreParameter
{
public:
     gnomonCoreParameterVariant(const QVariant& s, const QString& doc = QString());
    ~gnomonCoreParameterVariant(void) = default;

    QVariant value(void) const;

    void setValue(const QVariant&);

    QString type(void) const
    {
        return "variant";
    }

private:
    QVariant m_v;
};

//
// gnomonCoreParameter.h ends here
