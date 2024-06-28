#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>
#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"

class dtkCoreParameter;

class GNOMONCORE_EXPORT gnomonAbstractModel : public QObject
{
    Q_OBJECT

public:
    virtual ~gnomonAbstractModel(void) = default;

public:
    virtual void reset(void) = 0;
    virtual void step(double time, double dt) = 0;
    virtual void run(double timeMin, double timeMax, double dt) = 0;

signals:
    void modelMessage(QString);

public:
    virtual QString documentation(void) = 0;
    virtual QString version(void) { return "X.X.X"; }

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;
    virtual void addParameter(const QString& parameterName, const QString& parameterType) { };
    virtual void removeParameter(const QString& parameterName) { };
    virtual dtkCoreParameters parameters(void) const = 0;
    virtual QMap<QString, QString> parameterGroups(void) { return QMap<QString, QString>(); };

public:
    void setLogServerAddress(QString addr) {};
};

DTK_DECLARE_OBJECT        (gnomonAbstractModel *)
DTK_DECLARE_PLUGIN        (gnomonAbstractModel, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractModel, GNOMONCORE_EXPORT, abstractModel, gnomonCore)


//
// gnomonAbstractModel.h ends here
