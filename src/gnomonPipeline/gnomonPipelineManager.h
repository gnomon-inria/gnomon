#pragma once

#include <gnomonPipelineExport.h>

#include <QtCore>

class gnomonPipeline;
class gnomonPipelineNode;

class gnomonAbstractDynamicForm;
class gnomonAbstractAdapterCommand;
class gnomonAbstractCommand;
class gnomonAbstractConstructorCommand;
class gnomonAbstractReaderCommand;
class gnomonAbstractWriterCommand;

class GNOMONPIPELINE_EXPORT gnomonPipelineManager : public QObject
{
    Q_OBJECT

public:
    static gnomonPipelineManager *instance(void);

protected:
     gnomonPipelineManager(QObject *parent = nullptr);
    ~gnomonPipelineManager(void);

public:
    Q_PROPERTY(gnomonPipeline *pipeline READ pipeline CONSTANT);

public:
    gnomonPipeline *pipeline(void);

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

private:
    class gnomonPipelineManagerPrivate *d;

private:
    static gnomonPipelineManager *s_instance;
};

//
// gnomonPipeline.h ends here
