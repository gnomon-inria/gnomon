#pragma once

#include <QtCore>

class xLoggerConsoleItem : public QObject
{
    Q_OBJECT

public:
    enum Type {
        Info,
        Debug,
        Warning,
        Error
    };

    Q_ENUM(Type)

    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(QString time READ time CONSTANT)
    Q_PROPERTY(QString label READ label CONSTANT)

public:
     xLoggerConsoleItem(const QString&, Type);
    ~xLoggerConsoleItem(void);

public:
    QString description(void);
    QString label(void);
    QString time(void);
    Type type(void);

private:
    class xLoggerConsoleItemPrivate *d;
};

//
// xLoggerConsoleItem.hpp ends here
