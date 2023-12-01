#pragma once

#include <QtCore>
#include <QObject>
#include <QFileSystemModel>

class gnomonFileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    Q_PROPERTY(QString rootDir READ rootDir WRITE setRootDir NOTIFY rootDirChanged)

public:
    gnomonFileSystemModel(QObject *parent = nullptr);
    ~gnomonFileSystemModel(void);

public:
    QString rootDir(void);
    void setRootDir(const QString& dir);

signals:
    void rootDirChanged(void);

public:
    Q_INVOKABLE QString filePath(const QModelIndex &index) const;

private:
    QString path;
};