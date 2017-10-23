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

#include "tissueCoreSettings.h"

tissueCoreSettings::tissueCoreSettings(void) : QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "tissue-core")
{
    qDebug() << Q_FUNC_INFO << "Calling tissueCoreSettings";
    this->beginGroup("tissue-core");

    if(!this->allKeys().contains("plugins"))
        this->setValue("plugins", QString());

    this->sync();
    this->endGroup();

    this->beginGroup("omero");

    if(!this->allKeys().contains("server"))
        this->setValue("omero", QString());

    if(!this->allKeys().contains("user"))
        this->setValue("user", QString());

    if(!this->allKeys().contains("passwd"))
        this->setValue("passwd", QString());

    if(!this->allKeys().contains("port"))
        this->setValue("port", QString());

    this->sync();
    this->endGroup();
}

tissueCoreSettings::~tissueCoreSettings(void)
{

}

//
// tissueCoreSettings.cpp ends here
