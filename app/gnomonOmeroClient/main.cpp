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
    qWarning() << "Found " << allProject.size() << "project(s)" ;

        for (int i=0; i < allProject.size(); ++i) {
            QList<gnomonOmeroDatasetPtr> allDataset = allProject.at(i)->datasets();
            qWarning() << "project" << i
                       << ", id=" << allProject.at(i)->id()
                       << ": found " << allDataset.size() << "dataset(s)";
            qWarning() << "project name = " << allProject.at(i)->name();
            //qWarning() << " details Project " << allProject.at(i)->details();

            for(int j=0 ; j< allDataset.size(); ++j) {
                QList<gnomonOmeroImagePtr> allImages = allDataset.at(j)->images();
                qWarning() << "dataset" << j
                << ", id=" << allDataset.at(j)->id()
                << ": found " << allDataset.size() << "images(s)"
                << ", dataset Name " << allDataset.at(j)->name();

                for(int k=0 ; k< allImages.size(); ++k) {
                  qWarning() << "images" << k
                  << ", id=" << allImages.at(k)->id()
                  << ", image Name " << allImages.at(k)->name();
                  qWarning() << ", details " <<allImages.at(k)->details();


                }
            }
        }

    return 0;
}

//
// main.cpp ends here
