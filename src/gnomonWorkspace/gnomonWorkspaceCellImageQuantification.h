#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <gnomonVisualization/gnomonView/gnomonViewMatplotlib.h>

#include <QtCore>
#include <QtQml>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageQuantification : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to compute properties over the cells of a CellImage form \
and to display them in a 2D interactive figure.\n\
\n\
Depending on the chosen plugin, the properties are computed either only on \
the CellImage itself, or using an optional Image a input. In any case, cell \
properties are added to the input CellImage, and a DataFrame representing \
the data asociated with each cell is returned as an output.\n\
\n")

public:
     explicit gnomonWorkspaceCellImageQuantification(QObject *parent = nullptr);
    ~gnomonWorkspaceCellImageQuantification(void) override;

public:
    Q_PROPERTY(gnomonViewMatplotlib* targetMpl READ targetMpl CONSTANT);

public slots:
    void setInputs(void) override;
    void viewOutputs(void) override;

public:
    gnomonViewMatplotlib *targetMpl(void) const { return this->m_target_mpl;};

private: 
    gnomonViewMatplotlib *m_target_mpl = nullptr;
};

//
// gnomonWorkspaceCellImageQuantification.h ends here
