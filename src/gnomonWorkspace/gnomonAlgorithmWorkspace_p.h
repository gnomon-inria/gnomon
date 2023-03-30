#pragma once

#include <QtCore>

class gnomonPipelineManager;
class gnomonAbstractCommand;

class gnomonViewForm;
class gnomonViewFormList;
class gnomonViewFormPool;

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

    void updatePool(void);

public:
    QString workspace;
    QStringList keys;
    QVariantList algorithmsData;
    QString algorithm;
    int currentIndex = 0;
    QJsonObject savedState;

public:
    gnomonPipelineManager *pipeline_manager;

public:
    gnomonAbstractCommand *command = nullptr;
    QMetaObject::Connection connect_finished;
    QTimer timer = QTimer(this);

public:
    gnomonViewFormList *sources = nullptr;
    gnomonViewFormList *targets = nullptr;
    gnomonViewFormPool *pool = nullptr;
};
