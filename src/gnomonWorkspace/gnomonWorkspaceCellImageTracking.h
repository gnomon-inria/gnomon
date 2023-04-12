#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include "gnomonVisualization/gnomonView/gnomonQmlView.h"


class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageTracking :  public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace is used to compute the lineage of cells from an intensity image series \
and a segmented image series (or cell image series).\n\
\n\
It outputs the lineage tree and a cell image series where each cell knows its parents. \
\n")

public:
     gnomonWorkspaceCellImageTracking(QObject *parent = nullptr);
    ~gnomonWorkspaceCellImageTracking(void);

public:
    Q_PROPERTY(gnomonQmlView* sourceDict READ sourceDict CONSTANT);

    gnomonQmlView *sourceDict(void) const;

// TODO: have a more generic way for workspaces to signal warnings/errors to the GUI
signals:
    void notEnoughCells(int);
    void notInitTrans(void);

public slots:
    virtual void setInputs(void) override;
    virtual void viewOutputs(void) override;

protected:
    class gnomonWorkspaceCellImageTrackingPrivate *dd = nullptr;
};

//
// gnomonWorkspaceCellImageTracking.h ends here
