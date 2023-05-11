#include "gnomonAbstractWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormWriter>


QStringList gnomonAbstractWriterCommand::extensions(void)
{
    auto * action_writer = dynamic_cast<gnomonAbstractFormWriter *>(this->action);
    if (action_writer)
        return action_writer->extensions();
    else
        return {""};
}
