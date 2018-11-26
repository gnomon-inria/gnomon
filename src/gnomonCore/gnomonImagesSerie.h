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

#include <dtkCore>

class dtkImage;
class gnomonImagesSeriePrivate;

class GNOMONCORE_EXPORT gnomonImagesSerie
{
public:
             gnomonImagesSerie();
             gnomonImagesSerie(const gnomonImagesSerie&);
    virtual ~gnomonImagesSerie(void);

public:
    dtkImage* image() const;
    dtkImage* image(size_t time) const;
    dtkImage* image(const QString& channel, size_t time=SIZE_MAX) const;

    void setImage(dtkImage*, size_t time);
    void setImage(dtkImage*, const QString& channel = "", size_t time = SIZE_MAX);

public:
    size_t time(void) const;
    void setTime(size_t);
    QList<size_t> times(void) const;

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
