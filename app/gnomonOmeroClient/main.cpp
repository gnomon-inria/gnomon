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


#include <gnomonCore>
#include <gnomonCoreSettings>
#include <gnomonOmero>

int main(int argc, char *argv[])
{
    QList<gnomonOmeroProjectPtr> allProject;
    QList<gnomonOmeroDatasetPtr> allDataset;
    int long id;

    dtkCoreApplication *application = dtkCoreApplication::create(argc, argv);
    application->setApplicationName("GnomonOmeroClient");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.1.0");

    gnomonOmero omero;
    allProject = omero.projects();
    qWarning() << "OMERO: Found projects" << allProject.size();

    gnomonOmeroProjectPtr  omeroDataset = allProject.first();
    allDataset = omeroDataset->datasets();
    qWarning() << "OMERO: Found Dataset" << allDataset.size();


    return 0;
}

//
// main.cpp ends here
