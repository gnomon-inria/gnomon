// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonWorkspaceExport>

#include <QtCore>
#include <QtQml>

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePreprocess : public QObject
{
    Q_OBJECT

public:
     gnomonWorkspacePreprocess(QObject *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

public:
    Q_PROPERTY(QString algoName READ algoName WRITE setAlgoName);
    Q_PROPERTY(QStringList algorithms READ algorithms NOTIFY algorithmsLoaded)
    Q_PROPERTY(gnomonViewForm* source READ source);
    Q_PROPERTY(gnomonViewForm* target READ target);
    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)

signals:
    void algorithmsLoaded(void);
    void parametersChanged(void);

public slots:
    void run(void);

public:
    QString algoName(void) const;
    QStringList algorithms(void) const;
    Q_INVOKABLE void setAlgoName(const QString &);

public:
    gnomonViewForm *source(void) const;
    gnomonViewForm *target(void) const;
    QJSValue parameters(void);

private:
    class gnomonWorkspacePreprocessPrivate *d;
};

//
// gnomonWorkspacePreprocess.h ends here
