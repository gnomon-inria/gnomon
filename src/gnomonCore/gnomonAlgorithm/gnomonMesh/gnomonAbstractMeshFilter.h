#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonMesh/gnomonMesh.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class dtkCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractMeshFilter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractMeshFilter(void) = default;

public:
    virtual void setInput(std::shared_ptr<gnomonMeshSeries> mesh_series) = 0;
    virtual inline void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };

    virtual std::shared_ptr<gnomonMeshSeries> input() = 0;
    virtual inline std::shared_ptr<gnomonCellImageSeries> cellImage(void) const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };

    virtual std::shared_ptr<gnomonMeshSeries> output() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"setInput"};
        } else if (formName == "gnomonCellImage") {
            return {"setCellImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"input"};
        } else if (formName == "gnomonCellImage") {
            return {"cellImage"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonMesh") {
            return {"output"};
        }
        return {};
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshFilter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshFilter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshFilter, GNOMONCORE_EXPORT, meshFilter, gnomonCore)


//
// gnomonAbstractMeshFilter.h ends here
