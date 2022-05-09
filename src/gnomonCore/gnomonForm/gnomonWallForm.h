#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractForm.h"
#include "gnomonTypeDef.h"

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonWallForm : public gnomonAbstractForm
{

public:
    gnomonWallForm(void);
    explicit gnomonWallForm(QJsonObject& serialization);
    ~gnomonWallForm(void);

public:
    gnomonAbstractForm *clone() override;

public:
    void setNormal(gnomon::vec3_t);
    void setPosition(gnomon::vec3_t);

public:
    QString name(void) const override ;
    QMap<QString,QString> metadata(void) const override { return QMap<QString,QString>(); }
    QString dataName(void) const override { return "internal";}

    const QString pluginName(void) override;

    QJsonObject serialize(void) override;

    void deserialize(QJsonObject &object) override;


    const gnomon::vec3_t normal(void) const;
    const gnomon::vec3_t position(void) const;


private:
    class gnomonWallFormPrivate *d;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT (gnomonWallForm *)

//
// gnomonWallContext.h ends here
