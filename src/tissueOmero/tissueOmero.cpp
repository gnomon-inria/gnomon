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
    int omero_port;
    QString omero_user;
    QString omero_passwd;
public:
    omero::client_ptr client;
    omero::api::ServiceFactoryPrx sf;


};

tissueOmero::tissueOmero(void)
{
    this->d = new tissueOmeroPrivate;
    tissueCoreSettings settings;
    settings.beginGroup("omero");
    d->omero_server = settings.value("server").toString();
    d->omero_port = settings.value("port").toInt();
    d->omero_user = settings.value("user").toString();
    d->omero_passwd = settings.value("passwd").toString();
    settings.endGroup();

    qWarning() << Q_FUNC_INFO << d->omero_server;

    d->client = new omero::client(qPrintable(d->omero_server), d->omero_port);
    d->sf = d->client->createSession(qPrintable(d->omero_user), qPrintable(d->omero_passwd));
    d->sf->closeOnDestroy();

    qWarning() << "sessionID: " << QString::fromStdString(d->client->getSessionId()) <<  "Metadata:" << d->client->getSession()->getMetadataService();
}

tissueOmero::~tissueOmero(void)
{
  if(d->client){
    d->client->closeSession();
  }
  delete d;
}

void tissueOmero::browseDB(void)
{

}

void tissueOmero::readData(void)
{

}

void tissueOmero::writeData(void)
{

}

//
// tissueOmero.cpp ends here
