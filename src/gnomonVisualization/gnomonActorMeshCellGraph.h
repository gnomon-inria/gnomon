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

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

#include "gnomonActor.h"
#include "gnomonActorMesh.h"

#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

class gnomonCellGraph;

class GNOMONVISUALIZATION_EXPORT gnomonActorMeshCellGraph : public gnomonActorMesh
{
public:
    vtkTypeMacro(gnomonActorMeshCellGraph, vtkAssembly);

    static gnomonActorMeshCellGraph *New(void);

public:
    gnomonCellGraph *cellGraph(void);
    const QMap<double, QColor>& colormap(void) const;

public:
	void setCellGraph(gnomonCellGraph *cellgraph);

public:
    void setColorMap(const QMap<double, QColor>& colormap);
    void setVertexProperty(const QString& propertyName);
    void setVertexPropertyRange(const QList<double>& range);
    void setVertexSize(double radius);
    void setEdgeOpacity(double opacity);
    void setEdgeLinewidth(double linewidth);
    void setSlice(const QString& dim, const QList<double>& slice);
    void setFilterProperty(const QString& propertyName);
    void setFilterPropertyRange(const QList<double>& range);

public slots:
    void update(void) override;

public:
    double rangeMin(void) const;
    double rangeMax(void) const;

    double filterRangeMin(void) const;
    double filterRangeMax(void) const;

protected:
     gnomonActorMeshCellGraph(void);
    ~gnomonActorMeshCellGraph(void);

private:
    gnomonActorMeshCellGraph(const gnomonActorMeshCellGraph&);
    void operator = (const gnomonActorMeshCellGraph&);

private:
	class gnomonActorMeshCellGraphPrivate *dd;
};

//
// gnomonActorMeshCellGraph.h ends here
