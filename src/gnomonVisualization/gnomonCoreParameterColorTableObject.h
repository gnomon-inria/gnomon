#pragma once

#include <gnomonVisualizationExport>

#include <dtkCore/dtkCoreParameterObject>

#include "gnomonCoreParameterColorTable.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorTableObject declaration
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterColorTableObject : public dtkCoreParameterObject
{
    Q_OBJECT
    Q_PROPERTY(gnomonColorTable colorTable READ colorTable WRITE setColorTable NOTIFY colorTableChanged)
    Q_PROPERTY(QVariantMap value READ value WRITE setValue NOTIFY valueChanged)

public:
     gnomonCoreParameterColorTableObject(gnomonCoreParameterColorTable *);
    ~gnomonCoreParameterColorTableObject(void);

    void setColorTable(const gnomonColorTable&);
    gnomonColorTable colorTable(void) const;

    void setValue(const QVariantMap&);
    QVariantMap value(void) const;

signals:
    void colorTableChanged(const gnomonColorTable&);
    void valueChanged(const QVariantMap&);

public:
    Q_INVOKABLE const QColor& color(long i) const;
    Q_INVOKABLE void setColor(long i, const QColor& color);

public:
    gnomonCoreParameterColorTable *parameter(void) override;

public:
    void notifyColorTable(const gnomonColorTable&);

private:
    gnomonCoreParameterColorTable *m_param = nullptr;
};

//
// gnomonCoreParameterColorObject.h ends here
