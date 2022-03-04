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
    Q_PROPERTY(QString file_path READ file_path);
    Q_PROPERTY(QStringList scheduled_algo READ scheduled_algo);

public:
    const QString& name(void);
    const QString& description(void);

public slots:
    void clear(void);

public:
    void setName(const QString& name);
    void setDescription(const QString& desc);

public:
    const QStringList& nodeNames(void);
    const QString& file_path(void);
    const QStringList& scheduled_algo(void);
    Q_INVOKABLE gnomonPipelineNode *node(const QString& node_name);
    Q_INVOKABLE QVariantList indices(const QString&);

public slots:
    void addNode(gnomonPipelineNode *node);

signals:
    void nameChanged(void);
    void descriptionChanged(void);
    void nodeAdded(gnomonPipelineNode *);
    void pluginChanged(void);

public:
    Q_INVOKABLE QList<gnomonPipelineNode *> scheduledNodes(void);

public slots:
    Q_INVOKABLE void exportToJson(const QString& url);
    Q_INVOKABLE void readFromJson(const QString& url);
    void exportToToml(const QString& path);
    void exportToLuigiScript(const QString& path);

public slots:
    void updateLayout(void);

private:
    class gnomonPipelinePrivate *d;
};

//
// gnomonPipeline.h ends here
