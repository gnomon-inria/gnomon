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
#include <QtGui>
#include <QtWidgets>

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

public:
    virtual void setValue(const QVariant&);
    virtual void copy(gnomonCoreParameter *);

signals:
    void valueChanged();

protected:
    QString m_doc;
};

Q_DECLARE_METATYPE(gnomonCoreParameter *);

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterNumeric
// ///////////////////////////////////////////////////////////////////

template <typename T>
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

    void setValue(T val) { if(m_value != val) { m_value = val; emit valueChanged(); } }
    void setValue(const QVariant& v) { if(m_value != v.value<T>()) { m_value = v.value<T>(); emit valueChanged(); } }
    void setMinimumValue(T min) { m_min = min; }
    void setMaximumValue(T max) { m_max = max; }
    void setAccuracy(int accuracy) { m_accuracy = accuracy; }

    void copy(gnomonCoreParameter *other) {
        if (gnomonCoreParameterNumeric<T> *param = dynamic_cast<gnomonCoreParameterNumeric<T> *>(other)) {
            m_min = param->min();
            m_max = param->max();
            m_accuracy = param->accuracy();
            m_value = param->value();
            emit valueChanged();
        }
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

Q_DECLARE_METATYPE(gnomonCoreParameterShort *);
Q_DECLARE_METATYPE(gnomonCoreParameterUShort *);
Q_DECLARE_METATYPE(gnomonCoreParameterInt *);
Q_DECLARE_METATYPE(gnomonCoreParameterUInt *);
Q_DECLARE_METATYPE(gnomonCoreParameterLong *);
Q_DECLARE_METATYPE(gnomonCoreParameterULong *);
Q_DECLARE_METATYPE(gnomonCoreParameterDouble *);


// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterNumericRange
// ///////////////////////////////////////////////////////////////////

template <typename T>
class GNOMONCORE_EXPORT gnomonCoreParameterNumericRange : public gnomonCoreParameter
{
public:
     gnomonCoreParameterNumericRange(QList<T> val, const QString& doc = QString()) :
         gnomonCoreParameter(doc),
         m_value(val) {}

     gnomonCoreParameterNumericRange(QList<T> val, T min, T max, const QString& doc = QString()) :
         gnomonCoreParameter(doc),
         m_value(val),
         m_min(min),
         m_max(max) {}

     gnomonCoreParameterNumericRange(T valMin, T valMax, T min, T max, const QString& doc = QString()) :
         gnomonCoreParameter(doc),
         m_value(QList<T>({valMin,valMax})),
         m_min(min),
         m_max(max) {}

    gnomonCoreParameterNumericRange(T val[2], T min, T max, int accuracy, const QString& doc = QString()) :
         gnomonCoreParameter(doc),
         m_value(val),
         m_min(min),
         m_max(max),
         m_accuracy(accuracy) {}

    ~gnomonCoreParameterNumericRange(void) = default;

    QList<T> value(void) const { return m_value; }
    T min(void) const { return m_min; }
    T max(void) const { return m_max; }
    int accuracy(void) { return m_accuracy; }

    void setValue(QList<T> val) { if(m_value != val) { m_value = val; emit valueChanged(); } }
    void setValue(T valMin, T valMax) { if((m_value[0] != valMin)||(m_value[1] != valMax)) { m_value = QList<T>({valMin, valMax});  emit valueChanged(); } }
    void setValue(const QVariant& v) { if(m_value != v.value<QList<T> >()) { m_value = v.value<QList<T> >(); emit valueChanged(); } }
    void setMinimumValue(T min) { m_min = min; if(m_value[0]<min) m_value[0]=min; if(m_value[1]<min) m_value[1]=min; emit valueChanged(); }
    void setMaximumValue(T max) { m_max = max; if(m_value[0]>max) m_value[0]=max; if(m_value[0]>max) m_value[1]=max; emit valueChanged(); }
    void setAccuracy(int accuracy) { m_accuracy = accuracy; }

    void copy(gnomonCoreParameter *other) {
        if (gnomonCoreParameterNumericRange<T> *param = dynamic_cast<gnomonCoreParameterNumericRange<T> *>(other)) {
            m_min = param->min();
            m_max = param->max();
            m_accuracy = param->accuracy();
            m_value = param->value();
            emit valueChanged();
        }
    }

private:
    QList<T> m_value = {T(0),T(0)};
    int m_accuracy = 2;
    T m_min = std::numeric_limits<T>::min();
    T m_max = std::numeric_limits<T>::max();
};

using gnomonCoreParameterShortRange  = gnomonCoreParameterNumericRange<short>;
using gnomonCoreParameterUShortRange = gnomonCoreParameterNumericRange<unsigned short>;
using gnomonCoreParameterIntRange    = gnomonCoreParameterNumericRange<int>;
using gnomonCoreParameterUIntRange   = gnomonCoreParameterNumericRange<unsigned int>;
using gnomonCoreParameterLongRange   = gnomonCoreParameterNumericRange<long>;
using gnomonCoreParameterULongRange  = gnomonCoreParameterNumericRange<unsigned long>;
using gnomonCoreParameterDoubleRange = gnomonCoreParameterNumericRange<double>;

Q_DECLARE_METATYPE(gnomonCoreParameterShortRange *);
Q_DECLARE_METATYPE(gnomonCoreParameterUShortRange *);
Q_DECLARE_METATYPE(gnomonCoreParameterIntRange *);
Q_DECLARE_METATYPE(gnomonCoreParameterUIntRange *);
Q_DECLARE_METATYPE(gnomonCoreParameterLongRange *);
Q_DECLARE_METATYPE(gnomonCoreParameterULongRange *);
Q_DECLARE_METATYPE(gnomonCoreParameterDoubleRange *);

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
    void setValue(const QVariant&);

    void copy(gnomonCoreParameter *other);

private:
    bool m_value = false;
};

Q_DECLARE_METATYPE(gnomonCoreParameterBool *);

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterText
// ///////////////////////////////////////////////////////////////////


class GNOMONCORE_EXPORT gnomonCoreParameterText : public gnomonCoreParameter
{
public:
     gnomonCoreParameterText(const QString& val, const QString& doc = QString());
    ~gnomonCoreParameterText(void) = default;
    
    QString value(void) const;
    
    void setValue(QString&);
    void setValue(const QVariant&);
    
    void copy(gnomonCoreParameter *other);
    
private:
    QString m_value = "";
};

Q_DECLARE_METATYPE(gnomonCoreParameterText *);

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterString
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCoreParameterString : public gnomonCoreParameter
{
public:
     gnomonCoreParameterString(const QString& value, const QString& doc = QString());
     gnomonCoreParameterString(const QString& value, const QStringList& values, const QString& doc = QString());
     gnomonCoreParameterString(const QStringList& values, const QString& doc = QString());
    ~gnomonCoreParameterString(void) = default;

    int size(void) const;
    int currentIndex(void) const;
    // QString currentValue(void) const;
    QString value(void) const;
    QStringList values(void) const;

    void setCurrentIndex(int);
    void addValue(const QString&);
    void removeValue(const QString&);

    void setValues(const QStringList&);
    void setValue(const QString&);
    void setValue(const QVariant&);

    void copy(gnomonCoreParameter *other);

private:
    int m_current_index = 0;
    QStringList m_values;
};

Q_DECLARE_METATYPE(gnomonCoreParameterString *);


// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterString
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCoreParameterStringList : public gnomonCoreParameter
{
public:
     gnomonCoreParameterStringList(const QStringList& value, const QString& doc = QString());
     gnomonCoreParameterStringList(const QStringList& value, const QStringList& values, const QString& doc = QString());
    ~gnomonCoreParameterStringList(void) = default;

    int size(void) const;
    QStringList value(void) const;
    QStringList values(void) const;

    void addValue(const QString&);
    void removeValue(const QString&);

    void setValues(const QStringList&);
    void setValue(const QStringList&);
    void setValue(const QVariant&);

    void copy(gnomonCoreParameter *other);

private:
    QStringList m_value;
    QStringList m_values;
};

Q_DECLARE_METATYPE(gnomonCoreParameterStringList *);

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

    void copy(gnomonCoreParameter *other);

private:
    QVariant m_v;
};

Q_DECLARE_METATYPE(gnomonCoreParameterVariant *);

//
// gnomonCoreParameter.h ends here
