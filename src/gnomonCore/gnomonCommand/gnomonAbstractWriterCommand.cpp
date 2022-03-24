#include "gnomonAbstractWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormWriter>


QStringList gnomonAbstractWriterCommand::extensions(void)
{
    return dynamic_cast<gnomonAbstractFormWriter *>(this->action)->extensions();
}
