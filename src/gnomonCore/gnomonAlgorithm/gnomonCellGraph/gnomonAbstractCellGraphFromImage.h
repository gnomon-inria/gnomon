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

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

class dtkImage;
class gnomonCellGraph;

class GNOMONCORE_EXPORT gnomonAbstractCellGraphFromImage : public QRunnable
{

    //Inputs
public:
    virtual void setUseMargins(bool use_margins) = 0;

    virtual void setImage(dtkImage *image) = 0;

    virtual void setMinContactArea(float min_val) = 0;

    virtual void setBackgroundLabel(long label) = 0;

    virtual void setPredefinedProperties(const QStringList& names) = 0;

    // Outputs
public:
    virtual gnomonCellGraph *computedGraph(void) const = 0;

public:
    virtual void run(void) = 0;
    virtual QString documentation(void) = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellGraphFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellGraphFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellGraphFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellGraphFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellGraphFromImage, GNOMONCORE_EXPORT, cellGraphFromImage);
}

//
// gnomonAbstractCellGraphFromImage.h ends here
