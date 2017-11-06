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

public:
    QList<gnomonOmeroObject *> topDir;
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

    omero::api::IContainerPrx containerService = d->sf->getContainerService();
    omero::sys::LongList list;
  	omero::api::IObjectList projectList = containerService->loadContainerHierarchy("Project", list, params);
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

void gnomonOmero::readData(void)
{

}

void gnomonOmero::writeData(void)
{

}

//
// gnomonOmero.cpp ends here
