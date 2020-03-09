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

class gnomonCompositionPrivate;

class GNOMONCOMPOSER_EXPORT gnomonComposition : public QObject
{
    Q_OBJECT

public:
    static gnomonComposition *instance(void);

public slots:
    void addReader(gnomonAbstractReaderCommand *command);
    void addAlgorithm(QMap<QString, gnomonAbstractDynamicForm *> input_forms, QMap<QString, gnomonAbstractDynamicForm *> output_forms, const QString& algorithm_class, const QString& algorithm, QMap<QString, gnomonCoreParameter *> parameters);
    void addForm(gnomonAbstractDynamicForm *form);
    void addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone);
signals:
    void nodeAdded(dtkComposerSceneNodeComposite *);

protected:
     gnomonComposition(void);
    ~gnomonComposition(void);

private:
    gnomonCompositionPrivate *d;

private:
    static gnomonComposition *s_instance;
};

//
// gnomonComposition.h ends here
