#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceSegmentation : public gnomonAlgorithmWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceSegmentation(QObject *parent = nullptr);
    ~gnomonWorkspaceSegmentation(void);

public:
    void setInputs() final;

};

//
// gnomonWorkspaceSegmentation.h ends here
