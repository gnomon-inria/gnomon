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

#include <gnomonConfig.h>

#include "gnomonVisualization.h"
#include "gnomonCoreSettings.h"

#include <dtkLog>

#include "gnomonVisualizations/gnomonCellComplex/gnomonCellComplexVtkVisualization.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonCellImageVtkVisualizationVolume.h"
#include "gnomonVisualizations/gnomonImage/gnomonImageVtkVisualizationChannelBlending.h"
#include "gnomonVisualizations/gnomonMesh/gnomonMeshVtkVisualization.h"

namespace gnomonVisualization {

DTK_DEFINE_LAYER_MANAGER;

void activateObjectManager(void)
{
    manager().setObjectManager(dtkCoreObjectManager::instance());
}

void initialize(const QString& path)
{
    QString realpath = path;
    QStringList pathslist;

    if(path.isEmpty()) {
        gnomonCoreSettings settings;
        settings.beginGroup("gnomon-core");
        realpath = settings.value("plugins").toString();
        settings.endGroup();

        if (realpath.isEmpty()) {
            realpath = QDir(GNOMON_INSTALL_PREFIX).filePath("plugins/gnomonVisualization");
            dtkDebug() << "no plugin path configured for gnomonVisualization, using default" << realpath;
        }

        pathslist = realpath.split(":");

    } else {
        pathslist = realpath.split(":");
    }

    for(const QString& v_path : pathslist) {
        manager().initialize(v_path);
    }
    gnomonVisualization::cellComplexVtkVisualization::pluginFactory().record("gnomonCellComplexVtkVisualization", gnomonCellComplexVtkVisualizationCreator, "Cell Complex Visualization", "");
    // gnomonVisualization::cellImageVtkVisualization::pluginFactory().record("gnomonCellImageVtkVisualizationMarchingCubes", gnomonCellImageVtkVisualizationMarchingCubesCreator);
    gnomonVisualization::cellImageVtkVisualization::pluginFactory().record("gnomonCellImageVtkVisualizationVolume", gnomonCellImageVtkVisualizationVolumeCreator, "Cell Image Volume", "");
    gnomonVisualization::imageVtkVisualization::pluginFactory().record("gnomonImageVtkVisualizationChannelBlending", gnomonImageVtkVisualizationChannelBlendingCreator, "Channel Blending", "");
    gnomonVisualization::meshVtkVisualization::pluginFactory().record("gnomonMeshVtkVisualization", gnomonMeshVtkVisualizationCreator, "Mesh Visualization", "");
}

void uninitialize(void)
{
    manager().uninitialize();
}

void setVerboseLoading(bool b)
{
    manager().setVerboseLoading(b);
}

void setAutoLoading(bool auto_load)
{
    manager().setAutoLoading(auto_load);
}

};

//
// gnomonVisualization.cpp ends here
