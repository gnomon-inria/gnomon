#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePreprocess : public gnomonAlgorithmWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePreprocess(QObject *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

};

//
// gnomonWorkspacePreprocess.h ends here
