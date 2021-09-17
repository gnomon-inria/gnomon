// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonWorkspaceExport>

#include <dtkWidgets>

#include <gnomonCore>
#include <gnomonWidgets>

class GNOMONWORKSPACE_EXPORT gnomonPythonAlgorithmPluginEditor : public gnomonPythonEditor
{
    Q_OBJECT

public:
     gnomonPythonAlgorithmPluginEditor(QWidget *parent = Q_NULLPTR);
    ~gnomonPythonAlgorithmPluginEditor(void);

public:
    dtkWidgetsMenu *newFormMenu(bool input = true);
    dtkWidgetsMenu *newParameterMenu(void);

public:
    const QMap<QString, gnomonFormDescription>& inputForms(void) const;
    const QMap<QString, gnomonFormDescription>& outputForms(void) const;
    const QMap<QString, gnomonParameterDescription>& parameters(void) const;

public slots:
    void updateDataPlugins(const QString& form_type);
    void addInputForm(gnomonFormDescription desc, bool update_code=true);
    void addOutputForm(gnomonFormDescription desc, bool update_code=true);
    void addParameter(gnomonParameterDescription desc, bool update_code=true);

    void updateMenus(void);
    void updateCode(void);

    void parseCode(void);

public:
    class gnomonPythonAlgorithmPluginEditorPrivate *dd;

};


//
// gnomonPythonAlgorithmPluginEditor.h ends here