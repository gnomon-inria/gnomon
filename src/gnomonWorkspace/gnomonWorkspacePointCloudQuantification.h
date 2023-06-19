#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <gnomonVisualization/gnomonView/gnomonMplView.h>

#include <QtCore>

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePointCloudQuantification : public gnomonAlgorithmWorkspace
{
Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to compute properties over the points of a PointCloud form \
and to display them in a 2D interactive figure.\n\
\n\
Depending on the chosen plugin, the properties are computed either only on \
the PointCloud itself, or using an optional Image a input. In any case, point \
properties are added to the input PointCloud, and a DataFrame representing \
the data asociated with each point is returned as an output.\n\
\n")

public:
    explicit gnomonWorkspacePointCloudQuantification(QObject *parent = nullptr);
    ~gnomonWorkspacePointCloudQuantification(void) override;

public:
    Q_PROPERTY(gnomonMplView* targetMpl READ targetMpl CONSTANT);

public slots:
    void setInputs(void) override;
    void viewOutputs(void) override;

public:
    gnomonMplView *targetMpl(void) const { return this->m_target_mpl;};

private:
    gnomonMplView *m_target_mpl = nullptr;
};

//
// gnomonWorkspacePointCloudQuantification.h ends here
