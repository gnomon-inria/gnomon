#pragma once

#include <QtCore>
#include <QtQml>

class xLoggerConsoleItem;

class xLoggerConsoleController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<xLoggerConsoleItem> items READ items NOTIFY itemsChanged);
    Q_PROPERTY(int infoCount READ infoCount NOTIFY infoCountChanged);
    Q_PROPERTY(int debugCount READ debugCount NOTIFY debugCountChanged);
    Q_PROPERTY(int warningCount READ warningCount NOTIFY warningCountChanged);
    Q_PROPERTY(int errorCount READ errorCount NOTIFY errorCountChanged);

public:
    static xLoggerConsoleController *instance(void);

signals:
    void    infoCountChanged(void);
    void   debugCountChanged(void);
    void   errorCountChanged(void);
    void warningCountChanged(void);

signals:
    void itemsChanged(void);

public:
    int   infoCount(void);
    int  debugCount(void);
    int  errorCount(void);
    int warningCount(void);

    void addInfoMsg(const QString&);
    void addDebugMsg(const QString&);
    void addWarningMsg(const QString&);
    void addErrorMsg(const QString&);

    QQmlListProperty<xLoggerConsoleItem> items(void);

public:
#if QT_VERSION >= 0x060000
    static qsizetype itemCount(QQmlListProperty<xLoggerConsoleItem> *);
#else
    static int itemCount(QQmlListProperty<xLoggerConsoleItem> *);
#endif

public:
#if QT_VERSION >= 0x060000
    static xLoggerConsoleItem *itemAt(QQmlListProperty<xLoggerConsoleItem> *, qsizetype);
#else
    static xLoggerConsoleItem *itemAt(QQmlListProperty<xLoggerConsoleItem> *, int);
#endif

public slots:
    void clear(void);

private:
     xLoggerConsoleController(void);
    ~xLoggerConsoleController(void);

private:
    static xLoggerConsoleController* s_instance;

private:
    class xLoggerConsoleControllerPrivate *d;
};

//
// xLoggerConsoleController.hpp ends here
