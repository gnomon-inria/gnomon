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
#include <omero/client.h>

class tissueOmeroPrivate
{
public:
    QString omero_server;
    omero::client *client;

};

tissueOmero::tissueOmero(void)
{
    this->d = new tissueOmeroPrivate;
    tissueCoreSettings settings;
    settings.beginGroup("omero");
    d->omero_server = settings.value("server").toString();
    settings.endGroup();

    qDebug() << Q_FUNC_INFO << d->omero_server;

    client = new omero::client(qPrintable(d->omero_server));
    //client->createSession("root", "ome");
}

tissueOmero::~tissueOmero(void)
{
    delete d;
}

//
// tissueOmero.cpp ends here
