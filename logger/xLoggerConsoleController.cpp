#include "xLoggerConsoleController.hpp"
#include "xLoggerConsoleItem.hpp"

class xLoggerConsoleControllerPrivate
{
public:
    int infoCount = 0;
    int debugCount = 0;
    int errorCount = 0;
    int warningCount = 0;

public:
    QList<xLoggerConsoleItem *> items;
};

xLoggerConsoleController *xLoggerConsoleController::instance(void)
{
    if(!s_instance)
        s_instance = new xLoggerConsoleController;

    return s_instance;
}

int xLoggerConsoleController::infoCount(void)
{
    return d->infoCount;
}

int xLoggerConsoleController::debugCount(void)
{
    return d->debugCount;
}

int xLoggerConsoleController::errorCount(void)
{
    return d->errorCount;
}

int xLoggerConsoleController::warningCount(void)
{
    return d->warningCount;
}

void xLoggerConsoleController::clear(void)
{
    d->items.clear();

    d->infoCount = 0;
    d->debugCount = 0;
    d->warningCount = 0;
    d->errorCount = 0;

    emit itemsChanged();

    emit infoCountChanged();
    emit debugCountChanged();
    emit errorCountChanged();
    emit warningCountChanged();
}

void xLoggerConsoleController::addInfoMsg(const QString& msg)
{
    d->items.append(new xLoggerConsoleItem(QString(msg).remove('\n'), xLoggerConsoleItem::Info));

    emit infoCountChanged();
    emit itemsChanged();
}

void xLoggerConsoleController::addDebugMsg(const QString& msg)
{
    d->items.append(new xLoggerConsoleItem(QString(msg).remove('\n'), xLoggerConsoleItem::Debug));

    emit debugCountChanged();
    emit itemsChanged();
}

void xLoggerConsoleController::addWarningMsg(const QString& msg)
{
    d->items.append(new xLoggerConsoleItem(QString(msg).remove('\n'), xLoggerConsoleItem::Warning));

    d->warningCount++;

    emit warningCountChanged();
    emit itemsChanged();
}

void xLoggerConsoleController::addErrorMsg(const QString& msg)
{
    d->items.append(new xLoggerConsoleItem(QString(msg).remove('\n'), xLoggerConsoleItem::Error));

    d->errorCount++;

    emit errorCountChanged();
    emit itemsChanged();
}

QQmlListProperty<xLoggerConsoleItem> xLoggerConsoleController::items(void)
{
    return QQmlListProperty<xLoggerConsoleItem>(
        this, nullptr,
        &xLoggerConsoleController::itemCount,
        &xLoggerConsoleController::itemAt);
}

#if QT_VERSION >= 0x060000
qsizetype xLoggerConsoleController::itemCount(QQmlListProperty<xLoggerConsoleItem> *list)
#else
int xLoggerConsoleController::itemCount(QQmlListProperty<xLoggerConsoleItem> *list)
#endif
{
    auto parent = qobject_cast<xLoggerConsoleController *>(list->object);

    if (parent)
        return parent->d->items.count();

    return 0;
}

#if QT_VERSION >= 0x060000
xLoggerConsoleItem *xLoggerConsoleController::itemAt(QQmlListProperty<xLoggerConsoleItem> *list, qsizetype i)
#else
xLoggerConsoleItem *xLoggerConsoleController::itemAt(QQmlListProperty<xLoggerConsoleItem> *list, int i)
#endif
{
    auto parent = qobject_cast<xLoggerConsoleController *>(list->object);

    if (parent)
        return parent->d->items.at(i);

    return 0;
}

xLoggerConsoleController::xLoggerConsoleController(void)
{
    d = new xLoggerConsoleControllerPrivate;
}

xLoggerConsoleController::~xLoggerConsoleController()
{
    delete d;
}

xLoggerConsoleController* xLoggerConsoleController::s_instance = nullptr;


//
// xLoggerConsoleController.cpp ends here
