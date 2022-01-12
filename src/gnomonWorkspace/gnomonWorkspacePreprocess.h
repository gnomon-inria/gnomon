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

public:
    void setInputs() final;

public:
    Q_PROPERTY(QStringList availablePlugins READ availablePlugins);
    static QStringList availablePlugins();
};

//
// gnomonWorkspacePreprocess.h ends here
