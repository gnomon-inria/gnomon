#pragma once

#include <qquickimageprovider.h>
#include <QtCore>
#include <QQuickItemGrabResult>
#include <memory>

class gnomonImageProvider : public QQuickImageProvider
{
public:
    gnomonImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};

class workspaceImageProvider : public QQuickImageProvider
{
    Q_OBJECT

public:
    workspaceImageProvider(QObject *parent = nullptr);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

protected:
    QMap<int, QImage> m_thumbnails;

public slots:
    void makeScreenshot(const QString &id);
};

void gnomon_rinit();
void gnomonInitLogServer();

class gnomonPipeline;

class gnomonSessionLoader: public QObject
{
    Q_OBJECT

public:
    gnomonSessionLoader() = default;
    ~gnomonSessionLoader() = default;

public:
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged);
    Q_INVOKABLE bool load(const QString& url, QObject *window);

    double progress();
    void setProgress(double);

    bool runNodes(QStringList scheduled_nodes, QObject *window, std::shared_ptr<gnomonPipeline> pipeline, double progress_increment);

signals:
    void progressChanged();
    void finished();
    void failed(QString);

private:
    double m_progress = 0.; // from 0 to 1
    QString file_path;
    bool res = true;
    QMetaObject::Connection callbackConnection;

};

class gnomonQMLUtils: public QObject
{
  Q_OBJECT

protected:
  gnomonQMLUtils() = default;

public:
  ~gnomonQMLUtils() = default;

  Q_PROPERTY(QString dataPath READ dataPath WRITE setDataPath NOTIFY dataPathChanged)


  QString dataPath();
  void setDataPath(QString);

  static gnomonQMLUtils *instance();

  Q_INVOKABLE bool isValidPath(QString path);
  Q_INVOKABLE QString initDataPath(QString defaultPath);

signals:
  void dataPathChanged();

private:
  static gnomonQMLUtils *s_instance;

};
