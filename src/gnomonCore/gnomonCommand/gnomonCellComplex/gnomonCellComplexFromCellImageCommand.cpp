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

#include "gnomonCellComplexFromCellImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellComplexFromCellImageCommandPrivate
{
public:
    gnomonCellImageSeries* input = nullptr;
};

gnomonCellComplexFromCellImageCommand::gnomonCellComplexFromCellImageCommand(const QString& key) : d(new gnomonCellComplexFromCellImageCommandPrivate)
{
    loadPluginGroup("cellComplexFromCellImage");

    this->action = gnomonCore::cellComplexFromCellImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellComplexFromCellImageCommand::~gnomonCellComplexFromCellImageCommand(void)
{
    delete d;
}

void gnomonCellComplexFromCellImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonCellComplexFromCellImageCommand::undo(void)
{
    ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(nullptr);
}

void gnomonCellComplexFromCellImageCommand::setInput(gnomonCellImageSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(d->input);
}

void gnomonCellComplexFromCellImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellComplexFromCellImageCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellComplexFromCellImageCommand::input(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellComplexFromCellImage *) this->action)->input();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        return nullptr;
    } else {
        return cellImage;
    }
}

gnomonCellComplexSeries *gnomonCellComplexFromCellImageCommand::output(void)
{
    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexFromCellImage *) this->action)->output();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        return nullptr;
    } else {
        return cellComplex;
    }
}

//
// gnomonCellComplexFromCellImageCommand.cpp ends here
