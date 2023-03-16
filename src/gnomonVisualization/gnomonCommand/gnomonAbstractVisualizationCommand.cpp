#include "gnomonAbstractVisualizationCommand.h"

gnomonAbstractView *gnomonAbstractVisualizationCommand::view(void)
{
    return this->_view;
}

void gnomonAbstractVisualizationCommand::setView(gnomonAbstractView *view)
{
    this->_view = view;
}

std::shared_ptr<gnomonAbstractVisualization> gnomonAbstractVisualizationCommand::visualization(void) {
    return this->visu;
}