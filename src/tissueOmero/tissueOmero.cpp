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
#include <omero/api/IAdmin.h>
#include <omero/sys/ParametersI.h>
#include <omero/all.h>
#include <omero/api/ISession.h>
#include <omero/model/Session.h>


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
    omero::api::IContainerPrx containerService;

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
  int long userID = d->sf->getAdminService()->getEventContext()->userId;
  qWarning() << "OMERO: UserID: " << userID ;

  omero::sys::ParametersIPtr params = new omero::sys::ParametersI();

	//indicate to load leaves
	params->leaves();
  params->addId(omero::rtypes::rlong(userID));

  omero::api::IContainerPrx containerService = d->sf->getContainerService();
  omero::sys::LongList list;
	omero::api::IObjectList objlist = containerService->loadContainerHierarchy("Project", list, params);
  qWarning() << "OMERO: Found projects" << objlist.size() ;

  omero::api::IObjectList objlist2 = containerService->loadContainerHierarchy("Dataset", list, params);
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
