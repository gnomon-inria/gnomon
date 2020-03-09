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

class dtkComposerSceneNodeComposite;

class gnomonAbstractDynamicForm;
class gnomonAbstractReaderCommand;
class gnomonAbstractAlgorithmCommand;
class gnomonCoreParameter;

class gnomonPipelinePrivate;

class GNOMONCOMPOSER_EXPORT gnomonPipeline : public QObject
{
    Q_OBJECT

public:
    static gnomonPipeline *instance(void);

public slots:
    void addReader(gnomonAbstractReaderCommand *command);
    void addAlgorithm(QMap<QString, gnomonAbstractDynamicForm *> input_forms, QMap<QString, gnomonAbstractDynamicForm *> output_forms, const QString& algorithm_class, const QString& algorithm, QMap<QString, gnomonCoreParameter *> parameters);
    void addForm(gnomonAbstractDynamicForm *form);
    void addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone);
signals:
    void nodeAdded(dtkComposerSceneNodeComposite *);

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
