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

#include "tissueOmero.h"
#include <tissueCore>

class tissueOmeroPrivate
{
public:
    QString omero_server;
};

tissueOmero::tissueOmero(void)
{
    this->d = new tissueOmeroPrivate;
    tissueCoreSettings settings;
    settings.beginGroup("omero");
    d->omero_server = settings.value("server").toString();
    settings.endGroup();

    qDebug() << Q_FUNC_INFO << d->omero_server;
}

tissueOmero::~tissueOmero(void)
{
    delete d;
}

//
// tissueOmero.cpp ends here
