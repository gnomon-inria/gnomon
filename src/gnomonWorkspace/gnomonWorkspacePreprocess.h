#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePreprocess : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "Preprocess Workspace description from cxx")

public:
     gnomonWorkspacePreprocess(QObject *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

public:
    void setInputs() final;

};
//
// gnomonWorkspacePreprocess.h ends here
