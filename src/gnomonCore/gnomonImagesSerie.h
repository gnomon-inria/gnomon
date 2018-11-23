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
    void setImage(dtkImage*);

public:
    size_t time(void) const;
    void setTime(size_t);
    size_t times(void) const;   

public:
    QString channel(void) const;
    void setChannel(const QString&);
    const QStringList& channels(void) const;

public:
    gnomonImagesSerie* copy() const;

private:
    class gnomonImagesSeriePrivate *d;
};

//
// gnomonImagesSerie.h ends here