#pragma once

#include <QtCore>

class gnomonPipeline;
class gnomonAbstractCommand;

class gnomonViewForm;
class gnomonViewFormList;

// /////////////////////////////////////////////////////////////////////////////
// gnomonAlgorithmWorkspacePrivate
// /////////////////////////////////////////////////////////////////////////////

class gnomonAlgorithmWorkspacePrivate : public QObject
{
    Q_OBJECT

signals:
    void algorithmChanged(void);

public:
     gnomonAlgorithmWorkspacePrivate(void);
    ~gnomonAlgorithmWorkspacePrivate(void);

public:
    bool setAlgorithm(const QString& algorithm);
    void registerPipeline(void);
    void updateViewFormTypes(void);

public:
    QString workspace;
    QStringList keys;
    QString algorithm;

public:
    gnomonPipeline *pipeline;

public:
    gnomonAbstractCommand *command = nullptr;

public:
    gnomonViewFormList *sources = nullptr;
    gnomonViewFormList *targets = nullptr;
};
