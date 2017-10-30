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

#pragma once

#include <QtCore>

#include <tissueCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

class dtkImage;
class tissueCellGraph;

class TISSUECORE_EXPORT tissueAbstractCellGraphFromImage : public QRunnable
{

    //Inputs
public:
    virtual void setUseMargins(bool use_margins) = 0;

    virtual void setImage(dtkImage *image) = 0;

    virtual void setMinContactArea(float min_val) = 0;

    virtual void setBackgroundLabel(long label) = 0;

    virtual void setPredefinedProperties(const QList<QString>& names) = 0;

    // Outputs
public:
    virtual tissueCellGraph *computedGraph(void) const = 0;

public:
    virtual void run(void) = 0;

};

DTK_DECLARE_OBJECT(tissueAbstractCellGraphFromImage *)

DTK_DECLARE_PLUGIN(tissueAbstractCellGraphFromImage, TISSUECORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(tissueAbstractCellGraphFromImage, TISSUECORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(tissueAbstractCellGraphFromImage, TISSUECORE_EXPORT)


namespace tissueCore {
    DTK_DECLARE_CONCEPT(tissueAbstractCellGraphFromImage, TISSUECORE_EXPORT, graphFromImage);
}

//
// tissueAbstractCellGraphFromImage.h ends here
