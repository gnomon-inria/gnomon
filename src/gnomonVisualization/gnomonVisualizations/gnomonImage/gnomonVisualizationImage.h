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

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractVisualizationImage.h"

class gnomonViewForm;

class gnomonImage;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationImage : public gnomonAbstractVisualizationImage
{
    Q_OBJECT

public:
     gnomonVisualizationImage(void);
    ~gnomonVisualizationImage(void);

public:
	void setImage(gnomonImageSeries *image) override;

public slots:
    void updateOpacity(void);
    void updateChannelColorMap(void);

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public slots:
    void clear(void);

public slots:
    void onTimeChanged(double) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

private:
	class gnomonVisualizationImagePrivate *dd;
};

inline gnomonAbstractVisualizationImage *gnomonVisualizationImageCreator(void)
{
    return new gnomonVisualizationImage();
}

//
// gnomonVisualizationImage.h ends here