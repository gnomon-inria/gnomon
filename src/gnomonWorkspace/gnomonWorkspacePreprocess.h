#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePreprocess : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to apply preprocessing steps on the intensity signal of \
an Image form, producing a new output Image.\n\
\n\
Using the input Image from the left view, a processed Image will be computed \
and displayed in the right view. In some cases, a BinaryImage form can be used \
as a mask for some plugins, if there is one in the input view.\n\
\n")

public:
     gnomonWorkspacePreprocess(QObject *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

};
//
// gnomonWorkspacePreprocess.h ends here
