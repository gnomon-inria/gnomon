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

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceSegmentation : public QObject
{
    Q_OBJECT

public:
     gnomonWorkspaceSegmentation(QObject *parent = nullptr);
    ~gnomonWorkspaceSegmentation(void);

public:
    Q_PROPERTY(QString algoName READ algoName WRITE setAlgoName);
    Q_PROPERTY(gnomonViewForm* source READ source);
    Q_PROPERTY(gnomonViewForm* target READ target);
    //TODO: include crossParameterCollection ?
//    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)

public slots:
    void run(void);

public:
    QString algoName(void) const;
    void setAlgoName(const QString &);

public:
    gnomonViewForm *source(void) const;
    gnomonViewForm *target(void) const;

private:
    class gnomonWorkspaceSegmentationPrivate *d;
};

//
// gnomonWorkspaceSegmentation.h ends here
