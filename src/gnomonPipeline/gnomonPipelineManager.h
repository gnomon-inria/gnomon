#pragma once

#include <gnomonPipelineExport.h>

#include <QtCore>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>

class gnomonPipeline;
class gnomonPipelineNode;

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
    void addAdapter(gnomonAbstractAdapterCommand *command); //todo DEPRECATED !!
    void addAdaptedForm(std::shared_ptr<gnomonAbstractDynamicForm> form); //todo delete ?
    void addMorphoForm(std::shared_ptr<gnomonAbstractDynamicForm> form, int id, double voxelsize, int time_start, int time_end);
    void addAlgorithm(gnomonAbstractCommand *command);
    void addTask(const QString &task, QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> inputs,
                 QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> outputs);
    void addConstructor(gnomonAbstractConstructorCommand *command); //todo delete ?
    void addForm(std::shared_ptr<gnomonAbstractDynamicForm> form);
    void addClonedForm(std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractDynamicForm> clone); // TODO check?

public:
    void setFormIndex(std::shared_ptr<gnomonAbstractDynamicForm> form, int index=-1);

private:
    class gnomonPipelineManagerPrivate *d;

private:
    static gnomonPipelineManager *s_instance;
};

//
// gnomonPipeline.h ends here
