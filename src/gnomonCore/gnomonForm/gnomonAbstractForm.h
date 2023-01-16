#pragma once

#include <gnomonCoreExport>
#include <dtkCore>

class gnomonBinaryImage;
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
    virtual ~gnomonAbstractForm(void) {
        qDebug() << Q_FUNC_INFO << "Form " << this << " is dying";
    };

    virtual gnomonAbstractForm* clone() = 0;
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
    gnomonBinaryImage *asBinaryImage();
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
//
// gnomonAbstractForm.h ends here
