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

#include <functional>

#include <QString>
#include <QtWidgets>

class gnomonParameter: public QObject
{
    Q_OBJECT

public:
    virtual QWidget *connect(QWidget* parent) = 0;
};

class gnomonSpinBoxParameterPrivate;

class gnomonSpinBoxParameter : public gnomonParameter
{
    public:
        gnomonSpinBoxParameter(int, int, int, const QString&);

    public:
        QWidget *connect(QWidget *parent);

        int value() const;
        
    private:
        gnomonSpinBoxParameterPrivate *d;
};

class gnomonDoubleSpinBoxParameterPrivate;

class gnomonDoubleSpinBoxParameter : public gnomonParameter
{
    public:
        gnomonDoubleSpinBoxParameter(double, double, double, const QString&);

    public:
        QWidget *connect(QWidget *parent);

        double value() const;
        
    private:
        gnomonDoubleSpinBoxParameterPrivate *d;
};

class gnomonCheckBoxParameterPrivate;

class gnomonCheckBoxParameter : public gnomonParameter
{
    public:
        gnomonCheckBoxParameter(bool, const QString&);

    public:
        QWidget *connect(QWidget *parent);

        bool value() const;
        
    private:
        gnomonCheckBoxParameterPrivate *d;
};

class gnomonLineEditParameterPrivate;

class gnomonLineEditParameter : public gnomonParameter
{
    public:
        gnomonLineEditParameter(const QString&, const QString&);

    public:
        QWidget *connect(QWidget *parent);

        QString value() const;
        
    private:
        gnomonLineEditParameterPrivate *d;
};

/* class gnomonComboBoxParameterPrivate;

class gnomonComboBoxParameter : public gnomonParameter
{
    public:
        gnomonComboBoxParameter(const QString&, const QStringList&, const QString&);

    public:
        QWidget *connect(QWidget *parent);

        QString value() const;
        
    private:
        gnomonComboBoxParameterPrivate *d;
}; */

//
// gnomonParameter.h ends here
