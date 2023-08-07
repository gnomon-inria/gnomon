#include "xLogger.hpp"
#include "xLoggerConsoleController.hpp"
#include "xLoggerConsoleItem.hpp"

#define SPDLOG_PREVENT_CHILD_FD

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

void xLoggerQmlMessageHandler(QtMsgType, const QMessageLogContext &, const QString &);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class xLoggerConsoleSink : public xLoggerSink
{
public:
    void critical(const QString& message) override { xLoggerConsoleController::instance()->addErrorMsg(message);   }
    void    error(const QString& message) override { xLoggerConsoleController::instance()->addErrorMsg(message);   }
    void     warn(const QString& message) override { xLoggerConsoleController::instance()->addWarningMsg(message); }
    void     info(const QString& message) override { xLoggerConsoleController::instance()->addInfoMsg(message);    }
    void    debug(const QString& message) override { xLoggerConsoleController::instance()->addDebugMsg(message);   }
    void    trace(const QString& message) override { xLoggerConsoleController::instance()->addDebugMsg(message);   }
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class xLoggerPrivate
{
public:
    std::shared_ptr<spdlog::logger> logger;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class xLogger : public QObject
{
    Q_OBJECT

public:
    enum Level {
             Off = 6,
        Critical = 5,
           Error = 4,
            Warn = 3,
            Info = 2,
           Debug = 1,
           Trace = 0
    };

public:
    static xLogger *instance(void);

public slots:
    void critical(const QString&);
    void    error(const QString&);
    void     warn(const QString&);
    void     info(const QString&);
    void    debug(const QString&);
    void    trace(const QString&);

public:
    Level getLevel(void);
     void setLevel(Level);

private:
     xLogger(void);
    ~xLogger(void);

private:
    xLoggerPrivate *d;

private:
    static xLogger *s_instance;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

spdlog::level::level_enum toLevel(xLogger::Level level)
{
    switch(level) {
    case xLogger::Off:
        return spdlog::level::off;
    case xLogger::Critical:
        return spdlog::level::critical;
    case xLogger::Error:
        return spdlog::level::err;
    case xLogger::Warn:
        return spdlog::level::warn;
    case xLogger::Info:
        return spdlog::level::info;
    case xLogger::Debug:
        return spdlog::level::debug;
    case xLogger::Trace:
    default:
        return spdlog::level::trace;
    }
}

xLogger::Level fromLevel(spdlog::level::level_enum level)
{
    switch(level) {
    case spdlog::level::off:
        return xLogger::Off;
    case spdlog::level::critical:
        return xLogger::Critical;
    case spdlog::level::err:
        return xLogger::Error;
    case spdlog::level::warn:
        return xLogger::Warn;
    case spdlog::level::info:
        return xLogger::Info;
    case spdlog::level::debug:
        return xLogger::Debug;
    case spdlog::level::trace:
    default:
        return xLogger::Trace;
    }
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

xLogger *xLogger::instance(void)
{
    if(!s_instance)
        s_instance = new xLogger;

    return s_instance;
}

void xLogger::critical(const QString& log)
{
    d->logger->critical(log.toStdString());
}

void xLogger::error(const QString& log)
{
    d->logger->error(log.toStdString());
}

void xLogger::warn(const QString& log)
{
    d->logger->warn(log.toStdString());
}

void xLogger::info(const QString& log)
{
    d->logger->info(log.toStdString());
}

void xLogger::debug(const QString& log)
{
    d->logger->debug(log.toStdString());
}

void xLogger::trace(const QString& log)
{
    d->logger->trace(log.toStdString());
}

xLogger::Level xLogger::getLevel(void)
{
    return fromLevel(d->logger->level());
}

void xLogger::setLevel(xLogger::Level level)
{
    d->logger->set_level(toLevel(level));
}

xLogger::xLogger(void) : QObject()
{
    d = new xLoggerPrivate;

    d->logger = spdlog::stdout_color_mt("x");
    d->logger->set_level(spdlog::level::trace);

    qInstallMessageHandler(xLoggerQmlMessageHandler);
}

xLogger::~xLogger(void)
{
    delete d;
}

xLogger *xLogger::s_instance = nullptr;

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

QList <xLoggerSink *> sinks;

void x_logger_initialise(void)
{
    Q_UNUSED(xLogger::instance());

    qmlRegisterAnonymousType<xLoggerConsoleItem>("xLogger", 1);
    qmlRegisterSingletonInstance("xLogger", 1, 0, "Controller", xLoggerConsoleController::instance());
}

void x_logger_append_console(void)
{
    x_logger_append(new xLoggerConsoleSink);
}

void x_logger_append(xLoggerSink *sink)
{
    sinks << sink;
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

void xLoggerQmlMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    if (message.endsWith("Both point size and pixel size set. Using pixel size."))
        return;

    if (message.contains("QNSWindow"))
        return;

    QString log = QString("%1 (%2:%3, %4)")
        .arg(message)
        .arg(context.file)
        .arg(context.line)
        .arg(context.function);

    switch (type) {
    case    QtDebugMsg: xLogger::instance()->debug(log);    break;
    case     QtInfoMsg: xLogger::instance()->info(log);     break;
    case  QtWarningMsg: xLogger::instance()->warn(log);     break;
    case QtCriticalMsg: xLogger::instance()->critical(log); break;
    case    QtFatalMsg: xLogger::instance()->critical(log); break;
    }

    foreach(xLoggerSink *sink, ::sinks)
    switch (type) {
    case    QtDebugMsg: sink->debug(log);    break;
    case     QtInfoMsg: sink->info(log);     break;
    case  QtWarningMsg: sink->warn(log);     break;
    case QtCriticalMsg: sink->critical(log); break;
    case    QtFatalMsg: sink->critical(log); break;
    }
}

// /////////////////////////////////////////////////////////////////////////////

#include "xLogger.moc"

//
// xLogger.cpp ends here
