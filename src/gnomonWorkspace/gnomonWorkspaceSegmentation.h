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
    Q_PROPERTY(QJSValue collection READ collection NOTIFY collectionChanged)

signals:
    void collectionChanged(void);

public slots:
    void run(void);

public:
    QString algoName(void) const;
    void setAlgoName(const QString &);

public:
    gnomonViewForm *source(void) const;
    gnomonViewForm *target(void) const;
    QJSValue collection(void);

private:
    class gnomonWorkspaceSegmentationPrivate *d;
};

//
// gnomonWorkspaceSegmentation.h ends here
