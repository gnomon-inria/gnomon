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

#include <QtCore>
#include <QtQml>

#include <gnomonVisualization/gnomonView/gnomonViewFormList>

class gnomonViewForm;
class gnomonPythonAlgorithmPluginCode;

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePythonAlgorithm : public QObject
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

    Q_PROPERTY(gnomonViewFormList* sources READ sources CONSTANT);
    Q_PROPERTY(gnomonViewFormList* targets READ targets CONSTANT);
    Q_PROPERTY(gnomonViewForm* source READ source CONSTANT); //for ease of use
    Q_PROPERTY(gnomonViewForm* target READ target CONSTANT); //for ease of use

    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)

signals:
    void editModeChanged(void);
    void algorithmLoaded(void);
    void parametersChanged(void);

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

public:
    static bool isEmpty(void);

public:
    bool editMode(void);
    void setEditMode(bool edit);

public:
    gnomonPythonAlgorithmPluginCode *code(void) const;

    gnomonViewFormList *sources(void) const;
    gnomonViewFormList *targets(void) const;

    gnomonViewForm *source(void) const { return (*this->sources())[0]; };
    gnomonViewForm *target(void) const { return (*this->targets())[0]; };

    QJSValue parameters(void);

private:
    class gnomonWorkspacePythonAlgorithmPrivate *d;
};

//
// gnomonWorkspacePythonAlgorithm.h ends here
