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

#pragma once

#include <gnomonCoreExport>

#include "gnomonAbstractForm.h"

#include <dtkCore>

class dtkImage;
class gnomonImagesSeriePrivate;

class GNOMONCORE_EXPORT gnomonImagesSerie : public gnomonAbstractForm
{
public:
             gnomonImagesSerie();
             gnomonImagesSerie(const gnomonImagesSerie&);
    virtual ~gnomonImagesSerie(void);

public:
    QString name(void) const override { return"gnomonImagesSerie";}
    QMap<QString,QString> metadata(void) const override;

public:
    dtkImage* image() const;
    dtkImage* image(unsigned long time) const;
    dtkImage* image(const QString& channel, unsigned long time=SIZE_MAX) const;

    void setImage(dtkImage*, unsigned long time);
    void setImage(dtkImage*, const QString& channel = "", unsigned long time = SIZE_MAX);

public:
    unsigned long time(void) const;
    void setTime(unsigned long);
    QList<unsigned long> times(void) const;

public:
    QString channel(void) const;
    void setChannel(const QString&);
    QStringList channels(void) const;

public:
    gnomonImagesSerie* copy() const;

private:
    class gnomonImagesSeriePrivate *d;
};

//
// gnomonImagesSerie.h ends here
