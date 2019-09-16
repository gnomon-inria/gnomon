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

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
// TODO: Image
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkImagingCore>

class gnomonCellComplexFromCellImageCommandPrivate
{
public:
    gnomonCellImageSeries* input = nullptr;
};

gnomonCellComplexFromCellImageCommand::gnomonCellComplexFromCellImageCommand(const QString& key) : d(new gnomonCellComplexFromCellImageCommandPrivate)
{
    // QString command = "import gnomonCellComplexFromCellImage";

    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    // Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

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
    return ((gnomonAbstractCellComplexFromCellImage *) this->action)->input();
}

gnomonCellComplexSeries *gnomonCellComplexFromCellImageCommand::output(void)
{
    return ((gnomonAbstractCellComplexFromCellImage *) this->action)->output();
}

// 
// gnomonCellComplexFromCellImageCommand.cpp ends here
