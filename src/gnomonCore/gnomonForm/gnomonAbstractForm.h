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

#include <gnomonCoreExport>

#include <dtkCore>

class gnomonCellComplex;
class gnomonCellGraph;
class gnomonCellImage;
class gnomonDataDict;
class gnomonDataFrame;
class gnomonImage;
class gnomonLString;
class gnomonMesh;
class gnomonPointCloud;
class gnomonSphereForm;
class gnomonTree;


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractForm
{
public:
             gnomonAbstractForm(void) = default;
    virtual ~gnomonAbstractForm(void) = default;

//public:
//    virtual void accept(class gnomonFormVisitor *) {};
//
//protected:
//    void record(void);

public:
    virtual QString name(void) const = 0;
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;
    virtual QJsonObject serialize(void) = 0;
    virtual void deserialize(QJsonObject&) = 0;


public:
    gnomonCellComplex *asCellComplex();
    gnomonCellGraph *asCellGraph();
    gnomonCellImage *asCellImage();
    gnomonDataDict *asDataDict();
    gnomonDataFrame *asDataFrame();
    gnomonImage *asImage();
    gnomonLString *asLString();
    gnomonMesh *asMesh();
    gnomonPointCloud *asPointCloud();
    gnomonSphereForm *asSphere();
    gnomonTree *asTree();
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractForm *)
DTK_DECLARE_PLUGIN        (gnomonAbstractForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractForm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractForm, GNOMONCORE_EXPORT, abstractForm);
}

//
// gnomonAbstractForm.h ends here
