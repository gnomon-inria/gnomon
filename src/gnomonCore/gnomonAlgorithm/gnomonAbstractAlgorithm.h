#pragma once

#include <gnomonCoreExport>

#include <QtCore>

#include <dtkCore/dtkCorePlugin>
#include <dtkCore/dtkCoreParameters>
#include <dtkImagingCore>

#include "gnomonCore/gnomonCorePlugin.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
class gnomonAbstractCommand;

class GNOMONCORE_EXPORT gnomonAbstractAlgorithm : public QRunnable
{
public:
             gnomonAbstractAlgorithm(void) = default;
    virtual ~gnomonAbstractAlgorithm(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;
    virtual dtkCoreParameters parameters(void) const = 0;
    virtual QMap<QString, QString> parameterGroups(void) { return QMap<QString, QString>(); };
    virtual void updateParameters(void) = 0;
    virtual inline void connectParameter(dtkCoreParameter *parameter) {
        parameter->connect([this] (QVariant v) {
            qDebug()<<Q_FUNC_INFO;
            this->updateParameters();
            this->refreshParameters();
        });
    };
    virtual void refreshParameters(void) {
        qDebug()<<Q_FUNC_INFO<<"Not implemented";
    };
public:
    virtual void run(void) = 0;
    virtual void run_async(gnomonAbstractCommand *c) {};
    virtual QString documentation(void) = 0;
    virtual QString version(void) { return "X.X.X"; }
    virtual QString name(void) {return "";};

    virtual void clearInputs(void) {};
    virtual void clearOutputs(void) {};

    bool is_async = false;
    void (* callback)(gnomonAbstractCommand *command) = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractAlgorithm *)
DTK_DECLARE_PLUGIN        (gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT, algorithm);
}

//
// gnomonAbstractAlgorithm.h ends here
