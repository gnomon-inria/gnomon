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


#include <tissueCore>
#include <tissueCoreSettings>
#include <tissueOmero>

int main(int argc, char *argv[])
{
    dtkCoreApplication *application = dtkCoreApplication::create(argc, argv);
    application->setApplicationName("TissueOmeroClient");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.1.0");

    tissueOmero omero;
    omero.browseDB();

    return 0;
}

//
// main.cpp ends here
