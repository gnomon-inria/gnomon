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

#include "gnomonCoreSettings.h"

gnomonCoreSettings::gnomonCoreSettings(void) : QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon-core")
{
    this->beginGroup("gnomon-core");

    if(!this->allKeys().contains("plugins"))
        this->setValue("plugins", QString());

    this->sync();
    this->endGroup();

    this->beginGroup("omero");

    if(!this->allKeys().contains("server"))
        this->setValue("server", QString("myhost.company.com"));

    if(!this->allKeys().contains("user"))
        this->setValue("user", QString("nobody"));

    if(!this->allKeys().contains("passwd"))
        this->setValue("passwd", QString("insert your password here"));

    if(!this->allKeys().contains("port"))
        this->setValue("port", 4064);

    this->sync();
    this->endGroup();
}

gnomonCoreSettings::~gnomonCoreSettings(void)
{

}

//
// gnomonCoreSettings.cpp ends here
