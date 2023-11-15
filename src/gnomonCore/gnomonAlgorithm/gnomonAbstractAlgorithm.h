#pragma once

#include <gnomonCoreExport>

#include <QtCore>

#include <dtkCore/dtkCorePlugin>
#include <dtkCore/dtkCoreParameters>

#include "gnomonCore/gnomonCorePlugin.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
class gnomonAbstractCommand;

class GNOMONCORE_EXPORT gnomonAbstractAlgorithm
{
public:
             gnomonAbstractAlgorithm(void) = default;
    virtual ~gnomonAbstractAlgorithm(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;
    virtual dtkCoreParameters parameters(void) const = 0;
    virtual QMap<QString, QString> parameterGroups(void) { return QMap<QString, QString>(); };
    virtual void connectParameter(const QString& parameter_name);
    virtual void refreshParameters(void);
    virtual void onParameterChanged(const QString& parameter_name = "");

public:
    virtual int run(void) = 0;
    virtual void run_async(gnomonAbstractCommand *c) {};
    virtual QString documentation(void) = 0;
    virtual QString version(void) { return "X.X.X"; }
    virtual QString name(void) {return "";};

    virtual void pause(void) {};
    virtual void resume(void) {};
    virtual void stop(void) {};
    virtual int progress(void) {return -1;}

    virtual void setLogServerAddress(QString addr) {};

    virtual void clearInputs(void) {}; // set in decorators for python (see gnomonPlugin.py)
    virtual void clearOutputs(void) {};

    bool is_async = false;
    void (* callback)(gnomonAbstractCommand *command) = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractAlgorithm *)
DTK_DECLARE_PLUGIN        (gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT, algorithm, gnomonCore)


//
// gnomonAbstractAlgorithm.h ends here
