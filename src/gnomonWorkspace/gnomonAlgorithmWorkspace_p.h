#pragma once

#include <QtCore>

class gnomonPipelineManager;
class gnomonAbstractAlgorithmCommand;

class gnomonVtkView;
class gnomonVtkViewList;
class gnomonVtkViewPool;
class gnomonMplView;


#define WORKSPACEINIT(name, group, thecommand)                          \
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

public:
    gnomonPipelineManager *pipeline_manager;

public:
    gnomonAbstractAlgorithmCommand *command = nullptr;
    QMetaObject::Connection connect_finished;
    QTimer timer = QTimer(this);

public:
    gnomonVtkViewList *sources = nullptr;
    gnomonVtkViewList *targets = nullptr;
    gnomonVtkViewPool *pool = nullptr;

    QList<QMetaObject::Connection> connect_target_view_exports;

    gnomonMplView *figure = nullptr;
    gnomonQmlView *text_view = nullptr;
    bool canBeDestroyed = true;
};
