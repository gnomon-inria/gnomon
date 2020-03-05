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
     gnomonComposerNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QList<QString> inputs);
    ~gnomonComposerNodeAlgorithm(void);

public:
    QString algorithm_class;
    QString algorithm;

    QMap<QString, dtkComposerScenePort *> input_ports;
};

gnomonComposerNodeAlgorithm::gnomonComposerNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QList<QString> inputs) : dtkComposerSceneNodeComposite()
{
    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;

    this->setTitle(this->algorithm_class);

    for (const auto& input : inputs) {
        this->input_ports[input] = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
        this->addInputPort(this->input_ports[input]);
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

    QMap<gnomonComposerNodeAlgorithm *, QMap<QString, gnomonAbstractDynamicForm *> > node_input_forms;
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
    gnomonComposerNodeAlgorithm *node = new gnomonComposerNodeAlgorithm(algorithm_class,algorithm,input_forms.keys());

    d->node_input_forms[node] = input_forms;

    for (const auto& output : output_forms.keys()) {
        d->algorithm_nodes[output_forms[output]] = node;
    }
}

void gnomonComposition::addForm(gnomonAbstractDynamicForm *form)
{
    qDebug()<<Q_FUNC_INFO<<form<<d->reader_nodes.keys();
    qDebug()<<Q_FUNC_INFO<<form<<d->algorithm_nodes.keys();
    if (d->reader_nodes.contains(form)) {
        emit nodeAdded(d->reader_nodes[form]);
    } else if (d->algorithm_nodes.contains(form)) {
        gnomonComposerNodeAlgorithm *node = d->algorithm_nodes[form];
        QMap<QString, gnomonAbstractDynamicForm *> input_forms = d->node_input_forms[node];
        qDebug()<<Q_FUNC_INFO<<input_forms;
        for (const auto& input : input_forms.keys()) {
            qDebug()<<Q_FUNC_INFO<<input<<input_forms[input];
            if (d->reader_nodes.contains(input_forms[input])) {
                dtkComposerSceneEdge *edge = new dtkComposerSceneEdge();
                edge->setSource(d->reader_nodes[input_forms[input]]->output_port);
                edge->setDestination(node->input_ports[input]);
                edge->link(true);
                node->addEdge(edge);
            }
        }
        emit nodeAdded(node);
    }
}

gnomonComposition *gnomonComposition::s_instance = nullptr;

//
// gnomonComposition.cpp ends here