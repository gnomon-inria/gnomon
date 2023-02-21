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

#include "gnomonVisualizations/gnomonCellComplex/gnomonVisualizationCellComplex.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonVisualizationCellImageVolume.h"
#include "gnomonVisualizations/gnomonImage/gnomonVisualizationImageChannelBlending.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVisualizationMesh.h"

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
    gnomonVisualization::visualizationCellComplex::pluginFactory().record("gnomonVisualizationCellComplex", gnomonVisualizationCellComplexCreator, "Cell Complex Visualization", "");
    // gnomonVisualization::visualizationCellImage::pluginFactory().record("gnomonVisualizationCellImageMarchingCubes", gnomonVisualizationCellImageMarchingCubesCreator);
    gnomonVisualization::visualizationCellImage::pluginFactory().record("gnomonVisualizationCellImageVolume", gnomonVisualizationCellImageVolumeCreator, "Cell Image Volume", "");
    gnomonVisualization::visualizationImage::pluginFactory().record("gnomonVisualizationImageChannelBlending", gnomonVisualizationImageChannelBlendingCreator, "Channel Blending", "");
    gnomonVisualization::visualizationMesh::pluginFactory().record("gnomonVisualizationMesh", gnomonVisualizationMeshCreator, "Mesh Visualization", "");
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
