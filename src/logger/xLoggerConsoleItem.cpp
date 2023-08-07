#include "xLoggerConsoleItem.hpp"

class xLoggerConsoleItemPrivate
{
public:
    QString toString(xLoggerConsoleItem::Type);

public:
    xLoggerConsoleItem::Type type;
    QString description;
    QString label;
    QString time;
};

xLoggerConsoleItem::xLoggerConsoleItem(const QString& message, Type type) : QObject()
{
    d = new xLoggerConsoleItemPrivate;

    d->type = type;
    d->description = message;
    d->time = QDateTime::currentDateTime().toString("HH:mm:ss");
    d->label = d->toString(type);
}

xLoggerConsoleItem::~xLoggerConsoleItem(void)
{
    delete d;
}

QString xLoggerConsoleItem::description(void)
{
    return d->description;
}

xLoggerConsoleItem::Type xLoggerConsoleItem::type(void)
{
    return d->type;
}

QString xLoggerConsoleItem::label(void)
{
    return d->label;
}

QString xLoggerConsoleItem::time(void)
{
    return d->time;
}

QString xLoggerConsoleItemPrivate::toString(xLoggerConsoleItem::Type type)
{
    switch (type) {
        case xLoggerConsoleItem::Info: return "Info";
        case xLoggerConsoleItem::Debug: return "Debug";
        case xLoggerConsoleItem::Warning: return "Warning";
        case xLoggerConsoleItem::Error: return "Error";
    }

    return QString("Error");
}
