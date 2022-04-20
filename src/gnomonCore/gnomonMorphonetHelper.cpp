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

void gnomonMorphonetHelper::morphoPlot(void) {

    QProcess pp;
    pp.setProgram("start_morphonet.sh");
    pp.start();



/*
import morphonet
        mc = morphonet.Plot(start_browser=True, clear_temp=False,only_compute_mesh=False)
        segment="/home/trcabel/Dev/naviscope/test_data/p58_imgSeg_t000.inr.gz"

        mc.set_dataset(begin=0,end=0,background=1,segment=segment, factor=5,memory = 20)
        mc.curate()
*/

}