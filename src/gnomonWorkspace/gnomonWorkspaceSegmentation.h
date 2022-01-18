#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceSegmentation : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "Segmentation Workspace description from cxx")

public:
     explicit gnomonWorkspaceSegmentation(QObject *parent = nullptr);
    ~gnomonWorkspaceSegmentation() override;

public:
    void setInputs() final;

};

//
// gnomonWorkspaceSegmentation.h ends here
