
#include <dtkCore>
#include <dtkImagingCore>
#include "gnomonDataManager.h"

class gnomonDataManagerPrivate
{

};

gnomonDataManager *gnomonDataManager::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonDataManager;

    return s_instance;
}

dtkImage *gnomonDataManager::get(const QString& name)
{
    if(dtkObjectManager::instance()->keys().contains(name))
        return dtkObjectManager::instance()->value(name).value<dtkImage *>();

    qWarning() << "object " << name << " is not in the object Manager";
    return nullptr;
}

gnomonDataManager *gnomonDataManager::s_instance = Q_NULLPTR;

gnomonDataManager::gnomonDataManager(void)
{
    d = new gnomonDataManagerPrivate;
}

gnomonDataManager::~gnomonDataManager(void)
{
    delete d;
}
