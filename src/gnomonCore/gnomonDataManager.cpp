// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <dtkCore>
#include <dtkImagingCore>

#include "gnomonDataManager.h"

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataManagerPrivate
{

};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataManager *gnomonDataManager::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonDataManager;

    return s_instance;
}

dtkImage *gnomonDataManager::get(const QString& name)
{
    if(dtkCoreObjectManager::instance()->keys().contains(name))
        return dtkCoreObjectManager::instance()->value(name).value<dtkImage *>();

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

//
// gnomonDataManager.cpp ends here
