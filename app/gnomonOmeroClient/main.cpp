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
    qWarning() << "Found " << allProject.size() << "project(s)";

    if ( allProject.size() >= 1 ) {
        for (int i=0; i < allProject.size(); ++i) {
            gnomonOmeroProjectPtr  omeroDataset = allProject.at(i);
            allDataset = omeroDataset->datasets();
            qWarning() << "project" << i << ", id=" << allProject.at(i)->id()  << ": found " << allDataset.size() << "dataset(s)";

            for(int j=0 ; j< allDataset.size(); ++j) {
                // gnomonOmeroImagePtr omeroImages = allDataset.at(j);
                // qWarning() << "dataset" << j << ": found " << omeroImages.size() << "image(s)";
            }
        }
    }


    return 0;
}

//
// main.cpp ends here
