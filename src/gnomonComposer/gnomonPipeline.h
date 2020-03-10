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
class gnomonAbstractAlgorithmCommand;
class gnomonAbstractReaderCommand;
class gnomonAbstractWriterCommand;
class gnomonCoreParameter;

class gnomonPipelinePrivate;

class GNOMONCOMPOSER_EXPORT gnomonPipeline : public QObject
{
    Q_OBJECT

public:
    static gnomonPipeline *instance(void);

public slots:
    void addReader(gnomonAbstractReaderCommand *command);
    void addWriter(gnomonAbstractWriterCommand *command);
    void addAlgorithm(gnomonAbstractAlgorithmCommand *command);
    void addForm(gnomonAbstractDynamicForm *form);
    void addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone);
signals:
    void nodeAdded(gnomonPipelineNode *);

public slots:
    void exportToToml(const QString& path);

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
