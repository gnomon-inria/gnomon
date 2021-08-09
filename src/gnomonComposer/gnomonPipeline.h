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

#include <gnomonComposerExport.h>

#include <QtCore>

class gnomonPipelineNode;

class gnomonAbstractDynamicForm;
class gnomonAbstractAdapterCommand;
class gnomonAbstractAlgorithmCommand;
class gnomonAbstractConstructorCommand;
class gnomonAbstractReaderCommand;
class gnomonAbstractWriterCommand;

class gnomonPipelinePrivate;

class GNOMONCOMPOSER_EXPORT gnomonPipeline : public QObject
{
    Q_OBJECT

public:
    static gnomonPipeline *instance(void);

public slots:
    void addReader(gnomonAbstractReaderCommand *command);
    void addWriter(gnomonAbstractWriterCommand *command);
    void addAdapter(gnomonAbstractAdapterCommand *command);
    void addAdaptedForm(gnomonAbstractDynamicForm *form);
    void addAlgorithm(gnomonAbstractAlgorithmCommand *command);
    void addConstructor(gnomonAbstractConstructorCommand *command);
    void addForm(gnomonAbstractDynamicForm *form);
    void addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone);

signals:
    void nodeAdded(gnomonPipelineNode *);

public slots:
    void exportToToml(const QString& path);
    void exportToLuigiScript(const QString& path);
    void exportToJson(const QString& path);

public slots:
    void updateLayout(void);

protected:
     gnomonPipeline(void);
    ~gnomonPipeline(void);

private:
    gnomonPipelinePrivate *d;

private:
    static gnomonPipeline *s_instance;
};

//
// gnomonPipeline.h ends here
