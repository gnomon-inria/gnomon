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
class gnomonViewFormList;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceRegistration : public QObject
{
    Q_OBJECT

public:
     gnomonWorkspaceRegistration(QObject *parent = nullptr);
    ~gnomonWorkspaceRegistration(void);

public:
    Q_PROPERTY(QString algoName READ algoName WRITE setAlgoName);
    Q_PROPERTY(QStringList algorithms READ algorithms NOTIFY algorithmsLoaded)
    Q_PROPERTY(gnomonViewFormList* sources READ sources CONSTANT);
    Q_PROPERTY(gnomonViewForm* target READ target CONSTANT);
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
    gnomonViewFormList* sources(void) const;
    gnomonViewForm *target(void) const;
    QJSValue parameters(void);

private:
    class gnomonWorkspaceRegistrationPrivate *d;
};

//
// gnomonWorkspaceRegistration.h ends here
