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

#include "gnomonComposition.h"

#include <gnomonCore>

#include <dtkCore>
#include <dtkComposer>


// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeReader
// /////////////////////////////////////////////////////////////////

class GNOMONCOMPOSER_EXPORT gnomonComposerNodeReader : public dtkComposerSceneNodeComposite
{
public:
     gnomonComposerNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path);
    ~gnomonComposerNodeReader(void);

public:
    QString algorithm_class;
    QString algorithm;
    QString path;

    dtkComposerScenePort *output_port;

};

gnomonComposerNodeReader::gnomonComposerNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path) : dtkComposerSceneNodeComposite()
{
    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    this->output_port = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
    this->addOutputPort(output_port);
    this->layout();
}

gnomonComposerNodeReader::~gnomonComposerNodeReader(void)
{

}

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeAlgorithm
// /////////////////////////////////////////////////////////////////

class GNOMONCOMPOSER_EXPORT gnomonComposerNodeAlgorithm : public dtkComposerSceneNodeComposite
{
public:
     gnomonComposerNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QList<QString> inputs, QList<QString> outputs);
    ~gnomonComposerNodeAlgorithm(void);

public:
    QString algorithm_class;
    QString algorithm;

    QMap<QString, dtkComposerScenePort *> input_ports;
    QMap<QString, dtkComposerScenePort *> output_ports;
};

gnomonComposerNodeAlgorithm::gnomonComposerNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QList<QString> inputs,  QList<QString> outputs) : dtkComposerSceneNodeComposite()
{
    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;

    this->setTitle(this->algorithm_class);

    for (const auto& input : inputs) {
        this->input_ports[input] = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
        this->addInputPort(this->input_ports[input]);
    }
    for (const auto& output : outputs) {
        this->output_ports[output] = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
        this->addOutputPort(this->output_ports[output]);
    }
    this->layout();
}

gnomonComposerNodeAlgorithm::~gnomonComposerNodeAlgorithm(void)
{

}


// /////////////////////////////////////////////////////////////////
// gnomonCompositionPrivate
// /////////////////////////////////////////////////////////////////

class gnomonCompositionPrivate
{
public:
    QMap<gnomonAbstractDynamicForm *, gnomonComposerNodeReader *> reader_nodes;

    QMap<gnomonAbstractDynamicForm *, gnomonComposerNodeAlgorithm *> algorithm_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> algorithm_output;

    QMap<gnomonComposerNodeAlgorithm *, QMap<QString, gnomonAbstractDynamicForm *> > node_input_forms;

    QMap<gnomonAbstractDynamicForm *, gnomonAbstractDynamicForm *> form_clones;
};



// /////////////////////////////////////////////////////////////////
// gnomonComposition
// /////////////////////////////////////////////////////////////////

gnomonComposition *gnomonComposition::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonComposition;

    return s_instance;
}


gnomonComposition::gnomonComposition(void)
{
    d = new gnomonCompositionPrivate;
}

gnomonComposition::~gnomonComposition(void)
{
    delete d;
}

void gnomonComposition::addReader(gnomonAbstractDynamicForm *form, const QString& algorithm_class, const QString& algorithm, const QString& path)
{
    qDebug() << Q_FUNC_INFO << algorithm_class << "[" << algorithm << "] : "<<path;
    d->reader_nodes[form] = new gnomonComposerNodeReader(algorithm_class,algorithm,path);
}

void gnomonComposition::addAlgorithm(QMap<QString, gnomonAbstractDynamicForm *> input_forms, QMap<QString, gnomonAbstractDynamicForm *> output_forms, const QString& algorithm_class, const QString& algorithm, QMap<QString, gnomonCoreParameter *> parameters)
{
    qDebug() << Q_FUNC_INFO << algorithm_class << "[" << algorithm << "]";
    qDebug() << Q_FUNC_INFO << input_forms.keys() << "->" << output_forms.keys();
    gnomonComposerNodeAlgorithm *node = new gnomonComposerNodeAlgorithm(algorithm_class,algorithm,input_forms.keys(),output_forms.keys());

    d->node_input_forms[node] = input_forms;

    for (const auto& output : output_forms.keys()) {
        d->algorithm_nodes[output_forms[output]] = node;
        d->algorithm_output[output_forms[output]] = output;
    }
}

void gnomonComposition::addForm(gnomonAbstractDynamicForm *form)
{
    if (d->reader_nodes.contains(form)) {
        emit nodeAdded(d->reader_nodes[form]);
    } else if (d->algorithm_nodes.contains(form)) {
        gnomonComposerNodeAlgorithm *node = d->algorithm_nodes[form];
        QMap<QString, gnomonAbstractDynamicForm *> input_forms = d->node_input_forms[node];
        for (const auto& input : input_forms.keys()) {
            gnomonAbstractDynamicForm *input_form = input_forms[input];
            if (d->form_clones.contains(input_form)) {
                input_form = d->form_clones[input_form];
            }

            dtkComposerSceneEdge *edge = nullptr;
            if (d->reader_nodes.contains(input_form)) {
                edge = new dtkComposerSceneEdge();
                edge->setSource(d->reader_nodes[input_form]->output_port);
            } else if (d->algorithm_nodes.contains(input_form)) {
                edge = new dtkComposerSceneEdge();
                edge->setSource(d->algorithm_nodes[input_form]->output_ports[d->algorithm_output[input_form]]);
            }
            if (edge) {
                edge->setDestination(node->input_ports[input]);
                edge->link(true);
                node->addEdge(edge);
            }
        }
        emit nodeAdded(node);
    }
}

void gnomonComposition::addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone)
{
    qDebug()<<Q_FUNC_INFO<<clone<<"->"<<form;
    d->form_clones[clone] = form;
}

gnomonComposition *gnomonComposition::s_instance = nullptr;

//
// gnomonComposition.cpp ends here