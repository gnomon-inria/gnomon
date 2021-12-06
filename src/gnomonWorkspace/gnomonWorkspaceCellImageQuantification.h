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
class gnomonViewMatplotlib;


class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageQuantification : public QObject
{
    Q_OBJECT

public:
     gnomonWorkspaceCellImageQuantification(QObject *parent = nullptr);
    ~gnomonWorkspaceCellImageQuantification(void);

public:
    Q_PROPERTY(QString algoName READ algoName WRITE setAlgoName);
    Q_PROPERTY(QStringList algorithms READ algorithms NOTIFY algorithmsLoaded)
    Q_PROPERTY(gnomonViewForm* source READ source CONSTANT);
    Q_PROPERTY(gnomonViewMatplotlib* target READ target CONSTANT);
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
    gnomonViewMatplotlib *target(void) const;
    QJSValue parameters(void);

private:
    class gnomonWorkspaceCellImageQuantificationPrivate *d;
};

//
// gnomonWorkspaceCellImageQuantification.h ends here
