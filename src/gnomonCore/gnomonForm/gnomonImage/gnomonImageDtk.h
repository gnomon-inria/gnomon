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

#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <dtkCore>


class dtkImage;

class gnomonImagePrivate;

class GNOMONCORE_EXPORT gnomonImage : public gnomonAbstractForm
{
public:
             gnomonImage();
             gnomonImage(const gnomonImage&);
            ~gnomonImage(void);

public:
    dtkImage *image(QString channel="") const;

    void setImage(dtkImage *, QString channel="");

public:
    QStringList channels(void) const;

public:
    gnomonAbstractForm *clone() const;

public:
    QString name() const override { return "gnomonImage"; };
    QMap<QString,QString> metadata(void) const override;
    QString dataName(void) const override { return "dtkImage" }

private:
    class gnomonImagePrivate *d;
};


DTK_DECLARE_OBJECT(gnomonImage *)

// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonImage> gnomonImageSeries;
Q_DECLARE_METATYPE(gnomonImageSeries *)

//class GNOMONCORE_EXPORT gnomonImageSeries : public gnomonTimeSeries<gnomonImage>
//{
//public:
//    using gnomonTimeSeries<gnomonImage>::insert; // required for SWIG
//    using gnomonTimeSeries<gnomonImage>::at; // required for SWIG
//    using gnomonTimeSeries<gnomonImage>::times; // required for SWIG
//    QStringList channels(void) const {
//        QStringList channels;
//        for (double time : this->times()) {
//            channels.append(dynamic_cast<gnomonImage*>(this->at(time))->channels());
//        }
//        channels.removeDuplicates();
//        return channels;
//    };
//};

//
// gnomonImage.h ends here