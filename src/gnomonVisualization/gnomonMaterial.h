#pragma once

#include <QtCore>
#include <QtGui>

// ///////////////////////////////////////////////////////////////////
// gnomonMaterial
// ///////////////////////////////////////////////////////////////////

struct gnomonMaterial {

    QColor ambient;
    QColor specular;
    QColor emission;
    double diffuse;
    double shininess;
    double transparency;

    bool operator == (const gnomonMaterial& mat) const
    {
        return (ambient == mat.ambient)
        && (specular == mat.specular)
        && (emission == mat.emission)
        && (diffuse == mat.diffuse)
        && (shininess == mat.shininess)
        && (transparency == mat.transparency);
    }

    bool operator != (const gnomonMaterial& mat) const
    {
        return !(*this == mat);
    }
};


//
// gnomonMaterial.h ends here
