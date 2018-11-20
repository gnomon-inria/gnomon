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

struct guiParameter
{
    virtual QWidget *connect(QWidget* parent, void (func*)(const QString&, const QVariant&)) const = 0;
};

class guiSpinBoxParameterPrivate;

class guiSpinBoxParameter : public guiParameter
{
    public:
        guiSpinBoxParameter(int, int, const QString&);

    public:
        virtual QWidget *connect(QWidget *parent, void (func*)(const QString&, const QVariant&)) const;

    private:
        guiSpinBoxParameterPrivate *d;
};

/*class guiDoubleSpinBoxParameterPrivate;

struct guiDoubleSpinBoxParameter : public guiParameter
{
    public:
        virtual QWidget *connect(QWidget* parent) const;

    private:
        guiDoubleSpinBoxParameterPrivate *d;
};

class guiCheckBoxParameterPrivate;

struct guiCheckBoxParameter : public guiParameter
{
    public:
        virtual QWidget *connect(QWidget* parent, void (func*)(const QString&, const QVariant&)) const;

    private:
        guiCheckBoxParameter *d;
};

class guiLineEditParameterPrivate;

struct guiLineEditParameter : public guiParameter
{
    public:
        virtual QWidget *connect(QWidget* parent, void (func*)(const QString&, const QVariant&)) const;

    private:
        guiLineEditParameterPrivate *d;
};*/

//
// gnomonFinder.h ends here
