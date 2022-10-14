// gnomonCoreParameterGraphicalObject.h
//


#pragma once

#include <gnomonVisualizationExport>

#include <dtkCore/dtkCoreParameterObject>

#include "gnomonCoreParameterGraphical.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterGraphicalObject declaration
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterGraphicalObject : public dtkCoreParameterObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    gnomonCoreParameterGraphicalObject(gnomonCoreParameterGraphical *);
    ~gnomonCoreParameterGraphicalObject(void);

public:
    void setIdentifier(const QString&);
    QString identifier(void) const;

    void setName(const QString&);
    QString name(void) const;

signals:
    void nameChanged(const QString&);

public:
    void notifyName(const QString&);

private:
    gnomonCoreParameterGraphical *m_param = nullptr;

};

//
// gnomonCoreParameterGraphicalObject.h ends here