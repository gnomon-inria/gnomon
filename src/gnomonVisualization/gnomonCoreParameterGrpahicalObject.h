// gnomonCoreParameterGrpahicalObject.h
//


#pragma once

#include <gnomonVisualizationExport>

#include <dtkCore/dtkCoreParameterObject>

#include "gnomonCoreParameterGrpahical.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterGrpahicalObject declaration
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterGrpahicalObject : public dtkCoreParameterObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    gnomonCoreParameterGrpahicalObject(gnomonCoreParameterGrpahical *);
    ~gnomonCoreParameterGrpahicalObject(void);

public:
    void setIdentifier(const QString&);
    QString identifier(void) const;

    void setName(const QString&);
    QString name(void) const;

signals:
    void nameChanged(const QString&);

public:
    void notifyName(cont QString&);

private:
    gnomonCoreParameterGrpahical m_param = nullptr;

}

//
// gnomonCoreParameterGrpahicalObject.h ends here