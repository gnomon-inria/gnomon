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
    dtkCoreApplication *application = dtkCoreApplication::create(argc, argv);
    application->setApplicationName("GnomonOmeroClient");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.1.0");

    gnomonOmero omero;

    qDebug() << "Session ID:" << omero.sessionId();

    QList<gnomonOmeroProjectPtr> allProject = omero.projects();
    qWarning() << "Found " << allProject.size() << "project(s)";

    if ( allProject.size() >= 1 ) {
        for (int i=0; i < allProject.size(); ++i) {
            QList<gnomonOmeroDatasetPtr> allDataset = allProject.at(i)->datasets();
            qWarning() << "project" << i << ", id=" << allProject.at(i)->id()  << ": found " << allDataset.size() << "dataset(s)";

            for(int j=0 ; j< allDataset.size(); ++j) {
                QList<gnomonOmeroImagePtr> allImages = allDataset.at(i)->images();
                qWarning() << "dataset" << j << ": found " << allImages.size() << "image(s)";
            }
        }
    }


    return 0;
}

//
// main.cpp ends here
