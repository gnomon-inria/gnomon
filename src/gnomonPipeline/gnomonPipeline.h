#pragma once

#include <gnomonPipelineExport.h>

#include <QtCore>

class gnomonPipelineNode;

class GNOMONPIPELINE_EXPORT gnomonPipeline : public QObject
{
    Q_OBJECT

public:
     gnomonPipeline(QObject *parent = nullptr);
    ~gnomonPipeline(void);

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged);
    Q_PROPERTY(QStringList nodeNames READ nodeNames);

public:
    const QString& name(void);
    const QString& description(void);

public slots:
    void clear(void);

public:
    void setName(const QString& name);
    void setDescription(const QString& desc);

public:
    QStringList nodeNames(void);
    Q_INVOKABLE gnomonPipelineNode *node(const QString& node_name);

public slots:
    void addNode(gnomonPipelineNode *node);
    void removeNode(gnomonPipelineNode *node);

signals:
    void nameChanged(void);
    void descriptionChanged(void);
    void nodeAdded(gnomonPipelineNode *);
    void pluginChanged(void);
    void nodeRemoved(gnomonPipelineNode *);

public:
    QList<QStringList> scheduleGroups(void);
    Q_INVOKABLE QStringList scheduledNodeNames(bool recompute_form_indices=false);

public slots:
    Q_INVOKABLE void exportToJson(const QString& url);
    Q_INVOKABLE bool readFromJson(const QString& url, bool check_plugins=true);
    void exportToToml(const QString& path);
    void exportToLuigiScript(const QString& path);

public slots:
    void updateLayout(void);

private:
    class gnomonPipelinePrivate *d;
};

//
// gnomonPipeline.h ends here
