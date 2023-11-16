#pragma once

#include <gnomonPipelineExport.h>

#include <utility>
#include <QtCore>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>

class gnomonPipeline;
class gnomonPipelineNode;
class gnomonPipelineNodeReader;

class gnomonAbstractAdapterCommand;
class gnomonAbstractCommand;
class gnomonAbstractConstructorCommand;
class gnomonAbstractEvolutionModelCommand;
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
    void addAdaptedForm(const QString& form_uuid); //todo delete ?
    void addMorphoForm(const QString& form_uuid, int id, double voxelsize, int time_start, int time_end);
    void addAlgorithm(gnomonAbstractCommand *command);
    void addEvolutionModel(gnomonAbstractEvolutionModelCommand *command);
    void addTask(const QString &task, QMap<QString, QString> inputs,
                 QMap<QString, QString> outputs);
    void addConstructor(gnomonAbstractConstructorCommand *command); //todo delete ?
    void addForm(const QString& form_uuid);
    void addClonedForm(const QString& form_uuid, QString clone); // TODO check?
    bool removeForm(const QString& form_uuid);
public:
    void setFormIndex(const QString& form_uuid, int index=-1);

public: 
    std::pair<QString, gnomonPipelineNodeReader *> cacheNode(const QString& form_uuid);
    void decacheNode(const QString& form_uuid, std::pair<QString, gnomonPipelineNodeReader *> name_and_node);
private:
    class gnomonPipelineManagerPrivate *d;

private:
    static gnomonPipelineManager *s_instance;
};

//
// gnomonPipeline.h ends here
