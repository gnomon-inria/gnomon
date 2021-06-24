#include "gnomonDataDriver.h"
#include "gnomonAbstractDataDriver.h"

gnomonDataDriver *gnomonDataDriver::s_instance = nullptr;

gnomonDataDriver *gnomonDataDriver::instance(const QString& name)
{
    if(s_instance && s_instance->name() != name)
        delete s_instance;

    if(!s_instance) {
        qDebug() << "keys : " << gnomonCore::dataDriver::pluginFactory().keys();
        if(!gnomonCore::dataDriver::pluginFactory().keys().contains(name))
            qDebug() << "WARNING no key" << name << "in dataDriver factory. did you initialize python?";

        s_instance = new gnomonDataDriver(name);
    }

    return s_instance;
}

const QString& gnomonDataDriver::name(void) {
    return d->name();
}

bool gnomonDataDriver::insert(const QString& doc)
{
    return d->insert(doc);
}

bool gnomonDataDriver::remove(const QString& key)
{
    return d->remove(key);
}

bool gnomonDataDriver::protect(const QString& key)
{
    return d->protect(key);
}

const QString& gnomonDataDriver::find(const QString& query)
{
    return d->find(query);
}

gnomonDataDriver::gnomonDataDriver(const QString& name)
{
    this->d = gnomonCore::dataDriver::pluginFactory().create(name);
}

gnomonDataDriver::~gnomonDataDriver(void)
{
    if(this->d)
        delete d;
}