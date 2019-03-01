// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <QtCore>

#include <gnomonWidgetsExport.h>

#include "gnomonAbstractVisualizationCellImage.h"

class gnomonViewForm;

class gnomonCellImage;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationCellImageVolume : public gnomonAbstractVisualizationCellImage
{
    Q_OBJECT

public:
     gnomonVisualizationCellImageVolume(void);
    ~gnomonVisualizationCellImageVolume(void);

public:
	void setCellImage(gnomonCellImage *cellImage) override;

public slots:
    void updateOpacity(void);
    void updateValueRange(void);

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;


private:
	class gnomonVisualizationCellImageVolumePrivate *dd;
};


inline gnomonAbstractVisualizationCellImage *gnomonVisualizationCellImageVolumeCreator(void)
{
    return new gnomonVisualizationCellImageVolume();
}


//
// gnomonVisualizationCellImage.h ends here