#pragma once

#include <gnomonVisualizationExport>

#include <dtkCore/dtkCoreParameterObject>

#include "gnomonCoreParameterNurbs.h"

class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterNurbsObject : public dtkCoreParameterObject
{
    Q_OBJECT

    Q_PROPERTY(int figureNumber READ figureNumber WRITE setFigureNumber NOTIFY figureNumberChanged);
    Q_PROPERTY(int degree READ degree WRITE setDegree NOTIFY degreeChanged)
    Q_PROPERTY(double delta READ delta WRITE setDelta NOTIFY deltaChanged)
    Q_PROPERTY(QStringList controlPoints READ controlPoints WRITE setControlPoints NOTIFY controlPointsChanged)

public:
    gnomonCoreParameterNurbsObject(gnomonCoreParameterNurbs *p);
    ~gnomonCoreParameterNurbsObject(void);

public:
    Q_INVOKABLE void updateControlPointsFromPython(void);

public:
    int degree(void);
    double delta(void);
    QStringList controlPoints(void);
    int figureNumber(void);

    void setDegree(int);
    void setDelta(double);
    void setControlPoints(const QStringList&);
    void notifyControlPointsChanged(void);

signals:
    void degreeChanged(int);
    void deltaChanged(double);
    void controlPointsChanged(void);
    void figureNumberChanged(int);


public slots:
    void setFigureNumber(int fig);

private:
    class gnomonCoreParameterNurbsObjectPrivate *d;
    gnomonCoreParameterNurbs *m_param = nullptr;
};
