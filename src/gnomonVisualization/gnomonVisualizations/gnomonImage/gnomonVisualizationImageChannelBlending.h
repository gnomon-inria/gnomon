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

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationImageChannelBlending : public gnomonAbstractVisualizationImage
{
    Q_OBJECT

public:
     gnomonVisualizationImageChannelBlending(void);
    ~gnomonVisualizationImageChannelBlending(void);

public:
	void setImage(gnomonImageSeries *image) override;

public slots:
    void updateOpacity(void);

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
	class gnomonVisualizationImageChannelBlendingPrivate *dd;
};

inline gnomonAbstractVisualizationImage *gnomonVisualizationImageChannelBlendingCreator(void)
{
    return new gnomonVisualizationImageChannelBlending();
}

//
// gnomonVisualizationImageChannelBlending.h ends here