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

#include <dtkCore>

class gnomonViewForm;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonAbstractVisualization : public QObject
{
    Q_OBJECT

public:
     gnomonAbstractVisualization(gnomonViewForm *view);
    ~gnomonAbstractVisualization(void);

public:
    void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
	virtual QImage imageRendering(void) = 0;

public slots:
    virtual void update(void) = 0;
    virtual void render(void) = 0;

protected:
    class gnomonAbstractVisualizationPrivate *d;
};


//
// gnomonAbstractVisualization.h ends here