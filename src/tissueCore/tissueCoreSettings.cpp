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
    this->beginGroup("tissue-core");

    if(!this->allKeys().contains("plugins"))
        this->setValue("plugins", QString());

    this->sync();
    this->endGroup();
}

tissueCoreSettings::~tissueCoreSettings(void)
{

}

//
// tissueCoreSettings.cpp ends here
