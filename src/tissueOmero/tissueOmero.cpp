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
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/api/IAdmin.h>

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
    omero::api::IAdminPrx admin = d->sf->getAdminService();
    qDebug() << "admin" << admin;
    omero::sys::EventContextPtr context = admin->getEventContext();
    qDebug() << "context" << context;

    int long userID = context->userId;
  qWarning() << "OMERO: UserID: " << userID ;

  omero::sys::ParametersIPtr params = new omero::sys::ParametersI();

	params->leaves();
  params->addId(userID);

  omero::api::IContainerPrx containerService = d->sf->getContainerService();
  omero::sys::LongList list;
	omero::api::IObjectList objlist = d->sf->getContainerService()->loadContainerHierarchy("Project", list, NULL);
  qWarning() << "OMERO: Found projects" << objlist.size() ;

  omero::api::IObjectList objlist2 = d->sf->getContainerService()->loadContainerHierarchy("Dataset", list, NULL);
  qWarning() << "OMERO: Found dataset" << objlist2.size() ;

}

void tissueOmero::readData(void)
{

}

void tissueOmero::writeData(void)
{

}

//
// tissueOmero.cpp ends here
