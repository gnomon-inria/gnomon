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

#include "gnomonOmeroConfig.h"

#include "gnomonOmero.h"
#include "gnomonOmeroObject.h"

#include <gnomonCore>
#include <omero/client.h>
#include <omero/model/Project.h>
#include <omero/model/Dataset.h>
#include <omero/model/Image.h>
#include <omero/model/Pixels.h>
#include <omero/model/PixelsType.h>
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/api/IAdmin.h>
#include <omero/api/IMetadata.h>


class gnomonOmeroPrivate
{
public:
    QString omero_server;
    QString omero_port;
    QString omero_user;
    QString omero_passwd;


public:
    omero::client_ptr client;
    omero::api::ServiceFactoryPrx sf;
    omero::api::IAdminPrx admin;
    omero::api::IContainerPrx containerService;
    omero::api::IPixelsPrx pixelsService;
    omero::api::RawPixelsStorePrx rawPixelsStore;

public:
    QList<gnomonOmeroObject *> topDir;
    int long userID;

};

gnomonOmero::gnomonOmero(void)
{
    this->d = new gnomonOmeroPrivate;
    gnomonCoreSettings settings;
    settings.beginGroup("omero");
    d->omero_server = settings.value("server").toString();
    d->omero_port = settings.value("port").toString();
    d->omero_user = settings.value("user").toString();
    d->omero_passwd = settings.value("passwd").toString();
    settings.endGroup();

    // create omero client context (only way to change the SSL cipher)
    Ice::InitializationData data;
    data.properties = Ice::createProperties();
    data.properties->setProperty("omero.host", qPrintable(d->omero_server));
    data.properties->setProperty("omero.port", qPrintable(d->omero_port));
    data.properties->setProperty("omero.user", qPrintable(d->omero_user));
    data.properties->setProperty("omero.pass", qPrintable(d->omero_passwd));

#if OPENSSL_VERSION_MAJOR <= 1 && OPENSSL_VERSION_MINOR < 1
    data.properties->setProperty("IceSSL.Ciphers", "ADH");
#else
    data.properties->setProperty("IceSSL.Ciphers", "ADH:@SECLEVEL=0");
#endif

    omero::client_ptr initialize_client = new omero::client(data);
    omero::api::ServiceFactoryPrx session_new = initialize_client->createSession();

    d->client = initialize_client->createClient(false);
    d->sf = d->client->getSession();
    d->sf->closeOnDestroy();
    d->admin = d->sf->getAdminService();
    d->pixelsService = d->sf->getPixelsService();
    d->rawPixelsStore = d->sf->createRawPixelsStore();

    omero::api::IAdminPrx admin = d->sf->getAdminService();
    omero::sys::EventContextPtr context = admin->getEventContext();
    d->userID = context->userId;


    qWarning() << "sessionID: " << QString::fromStdString(d->client->getSessionId()) <<  "Metadata:" << d->client->getSession()->getMetadataService();

}

gnomonOmero::~gnomonOmero(void)
{
    if(d->client){
      d->client->closeSession();
    }
    d->topDir.clear();
    delete d;
}

int long gnomonOmero::userId(void)
{
  return d->userID;
}

void gnomonOmero::browseDB(void)
{
    omero::api::IAdminPrx admin = d->sf->getAdminService();
    qWarning() << "admin" << admin;
    omero::sys::EventContextPtr context = admin->getEventContext();
    qWarning() << "context" << context;

    int long userID = context->userId;
    qWarning() << "OMERO: UserID: " << userID ;

    omero::sys::ParametersIPtr params = new omero::sys::ParametersI();
  	params->leaves();
    params->addId(userID);

    d->containerService = d->sf->getContainerService();
    omero::sys::LongList list;
  	omero::api::IObjectList projectList = d->containerService->loadContainerHierarchy("Project", list, params);
    qWarning() << "OMERO: Found projects" << projectList.size() ;

    for(int i=0; i< projectList.size(); i++)
	  {
  		omero::model::ProjectPtr proj = omero::model::ProjectPtr::dynamicCast(projectList[i]);
  		std::string projectName = proj->getName()->getValue();
  		qWarning() << "OMERO: Project: " << projectName.data() ;

  		omero::model::ProjectLinkedDatasetSeq datasets = proj->linkedDatasetList();

  		for(int j=0; j< datasets.size(); j++)
  		{
  			omero::model::DatasetPtr ds = datasets[j];
  			std::string datasetName = ds->getName()->getValue();
  			qWarning() << "OMERO: Dataset: " << datasetName.data();

  			omero::model::DatasetLinkedImageSeq images = ds->linkedImageList();

  			for(int k=0; k< images.size(); k++)
  			{
  				omero::model::ImagePtr img = images[k];
  				std::string imgName = img->getName()->getValue();
  				int long imgid = img->getId()->getValue();
          qWarning() << "OMERO: Image: ID: " << imgid ;
			  }
		  }
    }
}

QList<gnomonOmeroProjectPtr> gnomonOmero::projects(void)
{
    QList<gnomonOmeroProjectPtr> listProject;

    omero::sys::ParametersIPtr params = new omero::sys::ParametersI();
    params->leaves();
    params->addId(d->userID);

    d->containerService = d->sf->getContainerService();
    omero::sys::LongList list;
    omero::api::IObjectList projectList = d->containerService->loadContainerHierarchy("Project", list, params);

    for(int i=0; i< projectList.size(); i++)
    {
      omero::model::ProjectPtr proj = omero::model::ProjectPtr::dynamicCast(projectList[i]);
      gnomonOmeroProjectPtr item = gnomonOmeroProjectPtr(new gnomonOmeroProject(proj));
      listProject << item;
    }

    return listProject;

}

QList<gnomonOmeroDatasetPtr> gnomonOmero::datasets(void)
{

}

QList<gnomonOmeroImagePtr> gnomonOmero::images(void)
{

}

//
// gnomonOmero.cpp ends here
