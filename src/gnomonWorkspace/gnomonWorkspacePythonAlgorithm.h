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
#include "gnomonAbstractWorkspace.h"

#include <gnomonVisualization/gnomonView/gnomonVtkViewList>

#include <QJSValue>

class gnomonVtkView;
class gnomonPythonAlgorithmPluginCode;

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePythonAlgorithm : public gnomonAbstractWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to develop a new Python plugin class directly within \
the application, and to integrate it to the current pipeline.\n\
\n\
Through the Python editor, it is possible to write a plugin class that uses \
any library installed in the current environment. However the data structures \
used by the plugin must correspond to an existing form data plugin. There are \
buttons in the right menu that allow to simply configure the input and output \
forms of the plugin, and their associated data plugins.\n\
\n\
Once written, or loaded from disk, the plugin can be executed by swiping the \
right menu towards the right, making appear a standard workspace display.\n\
\n")

public:
     gnomonWorkspacePythonAlgorithm(QObject *parent = nullptr);
    ~gnomonWorkspacePythonAlgorithm(void);

public:
    Q_PROPERTY(QString algorithm READ algorithm NOTIFY algorithmLoaded)

    Q_PROPERTY(bool editMode READ editMode WRITE setEditMode NOTIFY editModeChanged)

    Q_PROPERTY(gnomonPythonAlgorithmPluginCode* code READ code CONSTANT);

    Q_PROPERTY(gnomonVtkViewList* sources READ sources CONSTANT);
    Q_PROPERTY(gnomonVtkViewList* targets READ targets CONSTANT);
    Q_PROPERTY(gnomonVtkView* source READ source CONSTANT); //for ease of use
    Q_PROPERTY(gnomonVtkView* target READ target CONSTANT); //for ease of use

    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)

signals:
    void editModeChanged(void);
    void algorithmLoaded(void);
    void parametersChanged(void);
    void requestOpenFile(const QString& path);

public slots:
    void read(const QString& file_url);
    void save(const QString& file_url) const;

public:
    Q_INVOKABLE QUrl defaultReadPath();

public slots:
    void loadAlgorithm(void);

public:
    QString algorithm(void) const;

public slots:
    void run(void);
    virtual void setInputs(void);
    virtual void viewOutputs(void);

    void export_outputs(void) override;

public:
    static bool isEmpty(void);

public:
    bool editMode(void);
    void setEditMode(bool edit);

public:
    gnomonPythonAlgorithmPluginCode *code(void) const;

    gnomonVtkViewList *sources(void) const;
    gnomonVtkViewList *targets(void) const;

    gnomonVtkView *source(void) const { return (*this->sources())[0]; };
    gnomonVtkView *target(void) const { return (*this->targets())[0]; };

    QJSValue parameters(void);

    QJsonObject serialize() override;

    void deserialize(const QJsonObject &state) override;

public slots:
    void saveState(void);
    void restoreState(void);
    bool backup(void) const;
    void restore(void);

private:
    class gnomonWorkspacePythonAlgorithmPrivate *d;
};

//
// gnomonWorkspacePythonAlgorithm.h ends here
