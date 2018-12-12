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

class gnomonViewForm;

class gnomonImagesSerie;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationImagesSerie : public QObject
{
    Q_OBJECT

public:
     gnomonVisualizationImagesSerie(gnomonViewForm *view);
    ~gnomonVisualizationImagesSerie(void);

public:
	void setImagesSerie(gnomonImagesSerie *imagesSerie);
    void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public slots:
    void update(void);
    void render(void);

private:
	class gnomonVisualizationImagesSeriePrivate *d;
};


//
// gnomonVisualizationImagesSerie.h ends here