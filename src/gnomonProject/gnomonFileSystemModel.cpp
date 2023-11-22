#include "gnomonFileSystemModel.h"

gnomonFileSystemModel::gnomonFileSystemModel(QObject *parent) : QFileSystemModel(parent)
{
    this->path = "";
}

gnomonFileSystemModel::~gnomonFileSystemModel(void)
{
}

QString gnomonFileSystemModel::rootDir()
{
    return this->path;
}

void gnomonFileSystemModel::setRootDir(const QString& dir) 
{
    this->path = dir;
    this->setRootPath(this->path);
    emit rootDirChanged();
}
