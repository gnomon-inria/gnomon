#pragma once

#include <gnomonCoreExport>

#include <QtCore>

#include <dtkCore/dtkCorePlugin>
#include <dtkCore/dtkCoreParameters>

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
    virtual void refreshParameters(void) {};

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
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractAlgorithm, GNOMONCORE_EXPORT, algorithm);
}

//
// gnomonAbstractAlgorithm.h ends here
