#pragma once

#include <QtCore>

class gnomonPipelineManager;
class gnomonAbstractCommand;

class gnomonVtkView;
class gnomonVtkViewList;
class gnomonVtkViewPool;


#define WORKSPACEINIT(name, group, thecommand)                          \
    loadPluginGroup(#group);                                           \
    d->workspace = #name;                                              \
    d->command = new thecommand;                                        \
    d->keys = gnomonCore::group::pluginFactory().keys();                \
    d->algorithmsData = gnomonCore::group::pluginFactory().dataList();  \
    d->algorithm = d->command->algorithmName();                         \
                                                                        \
    emit algorithmsLoaded();

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
    gnomonVtkViewList *sources = nullptr;
    gnomonVtkViewList *targets = nullptr;
    gnomonVtkViewPool *pool = nullptr;
};
