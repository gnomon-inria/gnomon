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

#include "gnomonMeshConstructorCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonMeshConstructorCommandPrivate
{
public:
    gnomonImageSeries* input = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshConstructorCommand::gnomonMeshConstructorCommand(const QString& key) : d(new gnomonMeshConstructorCommandPrivate)
{
    loadPluginGroup("meshConstructor");

    this->action = gnomonCore::meshConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshConstructorCommand::~gnomonMeshConstructorCommand(void)
{
    delete d;
}

void gnomonMeshConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonMeshConstructorCommand::undo(void)
{
    ((gnomonAbstractMeshFromImage *) this->action)->setInput(nullptr);
}

void gnomonMeshConstructorCommand::setInput(gnomonImageSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFromImage *) this->action)->setInput(d->input);
}

void gnomonMeshConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonMeshConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonMeshConstructorCommand::input(void)
{
    gnomonImageSeries *image = ((gnomonAbstractMeshFromImage *) this->action)->input();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        return nullptr;
    } else {
        return image;
    }
}

gnomonMeshSeries *gnomonMeshConstructorCommand::output(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshFromImage *) this->action)->output();
    if ((!mesh)||(mesh->times().size()==0)) {
        return nullptr;
    } else {
        return mesh;
    }
}

//
// gnomonMeshConstructorCommand.cpp ends here
