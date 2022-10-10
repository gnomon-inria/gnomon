// gnomonCoreParameterGrpahical.h
//

#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtGui>

#include <dtkCore/dtkCoreParameter>


class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterGrpahical : public dtkCoreParameterBase<gnomonCoreParameterGrpahical>
{
public:

    gnomonCoreParameterGrpahical(void);
    gnomonCoreParameterGrpahical(const QString& label);

public:
    void setName(const QString&);
    QString name(void) const;

    // void setValue(const QString& id);


private:
    using dtkCoreParameter::m_label;

    QString m_n = QStringLiteral("no name");

private:
    class gnomonCoreParameterGrpahicalObject *m_object = nullptr;

};

GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream&, const gnomonCoreParameterGrpahical&);
GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream&, gnomonCoreParameterGrpahical&);
GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug, gnomonCoreParameterGrpahical);

DTK_DECLARE_PARAMETER(gnomonCoreParameterGrpahical);

//
// gnomonCoreParameterGrpahical.h ends here
