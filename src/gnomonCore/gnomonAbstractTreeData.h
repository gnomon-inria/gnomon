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

class dtkImage;

#include <QtCore>

#include <dtkCore>

#include <gnomonCoreExport>

class gnomonTree;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractTreeData
{
public:
             gnomonAbstractTreeData(void) = default;
    virtual ~gnomonAbstractTreeData(void) = default;

    virtual gnomonAbstractTreeData* clone(void) const = 0;

public:
    virtual QList<long> vertexIds(void) const = 0;
    virtual long vertexCount(void) const = 0;

    virtual long rootId(void) const = 0;

public:
    virtual bool hasChildren(long vertexId) const = 0;
    virtual QList<long> childrenIds(long vertexId) const = 0;
    virtual long childrenCount(long vertexId) const = 0;

    virtual bool hasParent(long vertexId) const = 0;
    virtual long parentId(long vertexId) const = 0;

    virtual QStringList vertexPropertyNames(void) const = 0;
    virtual bool hasVertexProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> vertexProperty(const QString& propertyName) const = 0;

    virtual bool isIsomorphicTo(gnomonTree * other) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractTreeData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTreeData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractTreeData, GNOMONCORE_EXPORT, treeData);
}

// gnomonAbstractTreeData.h ends here
