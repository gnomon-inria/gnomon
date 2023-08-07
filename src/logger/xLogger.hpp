#pragma once

#include <QtCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class xLoggerSink : public QObject
{
public:
     xLoggerSink(void) = default;
    ~xLoggerSink(void) = default;

public:
    virtual void critical(const QString&) = 0;
    virtual void    error(const QString&) = 0;
    virtual void     warn(const QString&) = 0;
    virtual void     info(const QString&) = 0;
    virtual void    debug(const QString&) = 0;
    virtual void    trace(const QString&) = 0;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

void x_logger_initialise(void);
void x_logger_append_console(void);
void x_logger_append(xLoggerSink *sink);
