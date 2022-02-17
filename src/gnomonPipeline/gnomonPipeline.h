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

#include <gnomonPipelineExport.h>

#include <QtCore>

class gnomonPipelineNode;

class gnomonAbstractDynamicForm;
class gnomonAbstractAdapterCommand;
class gnomonAbstractCommand;
class gnomonAbstractConstructorCommand;
class gnomonAbstractReaderCommand;
class gnomonAbstractWriterCommand;

class GNOMONPIPELINE_EXPORT gnomonPipeline : public QObject
{
    Q_OBJECT

public:
    static gnomonPipeline *instance(void);

protected:
     gnomonPipeline(void);
    ~gnomonPipeline(void);

public:
    Q_PROPERTY(QStringList nodeNames READ nodeNames);

public:
    const QStringList& nodeNames(void);

    Q_INVOKABLE gnomonPipelineNode *node(const QString& node_name);
    Q_INVOKABLE void exportToJson(const QString& url);
    Q_INVOKABLE void setPipeplineInfoForJsonExport(const QString&, const QString&, const QString&, const QString& );


public slots:
    void addReader(gnomonAbstractReaderCommand *command);
    void addWriter(gnomonAbstractWriterCommand *command);
    void addAdapter(gnomonAbstractAdapterCommand *command); //todo delete ?
    void addAdaptedForm(gnomonAbstractDynamicForm *form); //todo delete ?
    void addAlgorithm(gnomonAbstractCommand *command);
    void addConstructor(gnomonAbstractConstructorCommand *command); //todo delete ?
    void addForm(gnomonAbstractDynamicForm *form); //todo delete ?
    void addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone);

public:
    void setFormIndex(gnomonAbstractDynamicForm *form, int index=-1);

signals:
    void nodeAdded(gnomonPipelineNode *);

public slots:
    void exportToToml(const QString& path);
    void exportToLuigiScript(const QString& path);

public slots:
    void updateLayout(void);

private:
    class gnomonPipelinePrivate *d;

private:
    static gnomonPipeline *s_instance;
};

//
// gnomonPipeline.h ends here
