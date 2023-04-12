#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonVtkView;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceSegmentation : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to segment an Image form passed as input into a \
CellImage form, where cells are represented as image regions carrying \
identical labels.\n\
\n\
The plugins performing cell segmentation usually consist in two phases: a \
cell dectection and an image segmentation. In some cases, plugins will accept \
a PointCloud input providing the result of the cell detection. In the \
resulting CellImage output, there will then be one cell for each point of\
the input PointCloud form.\n\
\n")
public:
     explicit gnomonWorkspaceSegmentation(QObject *parent = nullptr);
    ~gnomonWorkspaceSegmentation() override;

};

//
// gnomonWorkspaceSegmentation.h ends here
