#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

class GNOMONCORE_EXPORT gnomonCoreSettings : public QSettings
{
public:
     gnomonCoreSettings(void);
    ~gnomonCoreSettings(void);
};

//
// gnomonCoreSettings.h ends here
