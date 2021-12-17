
#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceBinarization : public gnomonAlgorithmWorkspace
{
Q_OBJECT

public:
    explicit gnomonWorkspaceBinarization(QObject *parent = nullptr);
    ~gnomonWorkspaceBinarization(void);

public:
    void setInputs() final;

};

