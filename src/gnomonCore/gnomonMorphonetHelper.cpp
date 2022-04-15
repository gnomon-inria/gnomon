#include "gnomonMorphonetHelper.h"

gnomonMorphonetHelper *gnomonMorphonetHelper::s_instance = nullptr;
gnomonMorphonetHelperCreator *gnomonMorphonetHelper::s_creator = nullptr;

gnomonMorphonetHelper *gnomonMorphonetHelper::instance(void) 
{
    if(!gnomonMorphonetHelper::s_instance) {
        gnomonMorphonetHelper::s_instance = static_cast<gnomonMorphonetHelper *>(gnomonMorphonetHelper::s_creator->create());
    }

    return gnomonMorphonetHelper::s_instance;
}

void setMorphonetHelperCreator(gnomonMorphonetHelperCreator *creator) {
    gnomonMorphonetHelper::s_creator = creator;
}