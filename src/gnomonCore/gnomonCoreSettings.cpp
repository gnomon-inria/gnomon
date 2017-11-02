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
}

gnomonCoreSettings::~gnomonCoreSettings(void)
{

}

//
// gnomonCoreSettings.cpp ends here
