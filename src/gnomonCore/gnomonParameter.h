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

signals:
    void valueChanged(QVariant value);
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

//
// gnomonParameter.h ends here
