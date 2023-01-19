#include "gnomonCoreParameterNurbsObject.h"

#include <QtCore>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

class gnomonCoreParameterNurbsObjectPrivate {
public:
    gnomonCoreParameterNurbsObjectPrivate() = default;
    ~gnomonCoreParameterNurbsObjectPrivate() = default;

    void initPCurve(gnomonCoreParameterNurbs *param);
    void initPVisCurve(int dimension, bool is_function);

public:
    int figureNumber=-1;

    PyObject* pCurve = nullptr;
    PyObject* pVisCurve = nullptr;
};

void gnomonCoreParameterNurbsObjectPrivate::initPCurve(gnomonCoreParameterNurbs *param)
{
    if(!this->pCurve) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        PyObject* pName = PyUnicode_FromString("geomdl.BSpline");
        PyObject* pModule_bspline = PyImport_Import(pName);
        if(!pModule_bspline) {
            dtkWarn() << Q_FUNC_INFO << "Error importing module geomdl.BSpline";
            return;
        }
        Py_DECREF(pName);

        PyObject* pName2 = PyUnicode_FromString("geomdl.utilities");
        PyObject* pModule_utilities = PyImport_Import(pName2);
        if(!pModule_utilities) {
            qWarning() << Q_FUNC_INFO << "Error importing module geomdl.utilities";
            return;
        }
        Py_DECREF(pName2);


        // 1 create curve
        PyObject* pFunc = PyObject_GetAttrString(pModule_bspline, "Curve");
        this->pCurve = PyObject_CallNoArgs(pFunc);
        Py_DECREF(pFunc);

        //2 set curve params
        //2.1 degree
        PyObject *p_deg = PyLong_FromLong(param->degree());
        PyObject_SetAttrString(this->pCurve, "degree", p_deg);
        Py_DECREF(p_deg);

        //2.2 ctrlpoints
        if(param->controlPoints().size() > param->degree()) {
            PyObject *p_ctrlpts = PyList_New(int(param->controlPoints().size()));
            int i=0;
            for(auto point: param->controlPoints()) {
                PyObject *p_point = PyList_New(param->dimension());
                PyList_SetItem(p_point, 0, PyFloat_FromDouble(point[0]));
                PyList_SetItem(p_point, 1, PyFloat_FromDouble(point[1]));
                if(param->dimension() == 3) {
                    PyList_SetItem(p_point, 2, PyFloat_FromDouble(point[2]));
                }

                PyList_SetItem(p_ctrlpts, i++, p_point);
            }
            PyObject_SetAttrString(this->pCurve, "ctrlpts", p_ctrlpts);
            Py_DECREF(p_ctrlpts);
        }

        //2.3 update knotvector
        PyObject* pFunc_knot = PyObject_GetAttrString(pModule_utilities, "generate_knot_vector");
        PyObject* args = Py_BuildValue("(i, i)", param->degree(), int(param->controlPoints().size()));
        PyObject* pknotvector = PyObject_CallObject(pFunc_knot, args);
        if(!pknotvector) {
            dtkWarn() << " pknotvector error with args " << param->degree(), int(param->controlPoints().size());
        } else {
            PyObject_SetAttrString(this->pCurve, "knotvector", pknotvector);
        }
        Py_XDECREF(pknotvector);
        Py_DECREF(args);
        Py_DECREF(pFunc_knot);
        Py_DECREF(pModule_utilities);

        //2.4 delta
        PyObject* p_delta = PyFloat_FromDouble(param->delta());
        PyObject_SetAttrString(this->pCurve, "delta", p_delta);
        Py_DECREF(p_delta);

        PyGILState_Release(gstate);
    }
}

void gnomonCoreParameterNurbsObjectPrivate::initPVisCurve(int dimension, bool is_function)
{
    if(!this->pVisCurve) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        //3 set curve visu
        PyObject* pName3 = PyUnicode_FromString("gnomon.utils.VisMPL");
        PyObject* pModule_vis = PyImport_Import(pName3);
        if(!pModule_vis) {
            dtkWarn() << Q_FUNC_INFO << "Error importing module gnomon.utils.visMpl";
            return;
        }
        Py_DECREF(pName3);

        PyObject* pFunc2;
        if(dimension == 2)
            pFunc2 = PyObject_GetAttrString(pModule_vis, "VisCurve2D");
        else
            pFunc2 = PyObject_GetAttrString(pModule_vis, "VisCurve3D");

        PyObject* args = Py_BuildValue("OO", this->pCurve, is_function? Py_True : Py_False); //pis_function);
        this->pVisCurve = PyObject_CallObject(pFunc2, args);
        if(!this->pVisCurve) {
            dtkWarn() << "Error making VisCurve, no NURBS visu";
        } else {
            PyObject_SetAttrString(this->pCurve, "vis", this->pVisCurve);
        }

        Py_DECREF(args);
        Py_DECREF(pFunc2);
        Py_DECREF(pModule_vis);

        PyGILState_Release(gstate);
    }

}

gnomonCoreParameterNurbsObject::gnomonCoreParameterNurbsObject(gnomonCoreParameterNurbs *p) : dtkCoreParameterObject(p), m_param(p)
{
    d = new gnomonCoreParameterNurbsObjectPrivate();
}

gnomonCoreParameterNurbsObject::~gnomonCoreParameterNurbsObject(void)
{
    m_param = nullptr;
    delete d;
}

int gnomonCoreParameterNurbsObject::degree(void)
{
    return m_param->degree();
}

double gnomonCoreParameterNurbsObject::delta(void)
{
    return m_param->delta();
}

QStringList gnomonCoreParameterNurbsObject::controlPoints(void)
{
    gnomonCoreParameterNurbs::ctrls_type ctrl_points = m_param->controlPoints();
    QStringList res;
    for(auto point: ctrl_points) {
        QString p_str = "[" + QString::number(point[0], 'g', 2) + " , "
              + QString::number(point[1], 'g', 2) + " , "
              + QString::number(point[2], 'g', 2) + "]";
        res.append(p_str);
    }
    return res;
}

int gnomonCoreParameterNurbsObject::figureNumber(void)
{
    return d->figureNumber;
}

void gnomonCoreParameterNurbsObject::setDegree(int degree)
{
    m_param->setDegree(degree);
    if(!d->pCurve) {
        d->initPCurve(m_param);
    }

    if(d->pCurve) {

        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        PyObject *p_deg = PyLong_FromLong(m_param->degree());
        PyObject_SetAttrString(d->pCurve, "degree", p_deg);
        Py_DECREF(p_deg);

        PyGILState_Release(gstate);
    }
}

void gnomonCoreParameterNurbsObject::setDelta(double delta)
{
    m_param->setDelta(delta);

    if(!d->pCurve) {
        d->initPCurve(m_param);
    }

    if(d->pCurve) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        PyObject *p_delta = PyFloat_FromDouble(m_param->delta());
        PyObject_SetAttrString(d->pCurve, "delta", p_delta);
        Py_DECREF(p_delta);

        PyGILState_Release(gstate);
    } else {
        dtkWarn() << "Problem pCurve is not initialized";
    }
}

void gnomonCoreParameterNurbsObject::setControlPoints(const QStringList &ctrl_points_list)
{
    gnomonCoreParameterNurbs::ctrls_type ctrl_points;
    for( auto s_point : ctrl_points_list) {
        QString s = s_point;
        s.chop(1);
        s = s.remove(0,1);
        auto s_list = s.split(QLatin1Char(','));
        std::array<double, 3> arr = {s_list[0].toDouble(), s_list[1].toDouble(), s_list[2].toDouble()};
        ctrl_points.append(arr);
    }
    m_param->setControlPoints(ctrl_points);

    if(!d->pCurve) {
        d->initPCurve(m_param);
    }

    if(d->pCurve) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        PyObject *p_ctrlpts = PyList_New(int(m_param->controlPoints().size()));
        int i=0;
        for(auto point: m_param->controlPoints()) {
            PyObject *p_point = PyList_New(m_param->dimension());
            PyObject *px = PyFloat_FromDouble(point[0]);
            PyList_SetItem(p_point, 0, px);
            Py_DECREF(px);

            PyObject *py = PyFloat_FromDouble(point[1]);
            PyList_SetItem(p_point, 1, py);
            Py_DECREF(py);

            if(m_param->dimension() == 3) {
                PyObject *pz = PyFloat_FromDouble(point[2]);
                PyList_SetItem(p_point, 2, pz);
                Py_DECREF(pz);
            }

            PyList_SetItem(p_ctrlpts, i++, p_point);
            Py_DECREF(p_point);
        }

        PyObject_SetAttrString(d->pCurve, "ctrlpts", p_ctrlpts);
        Py_DECREF(p_ctrlpts);

        PyGILState_Release(gstate);
    } else {
        dtkWarn() << "Problem pCurve is not initialized";
    }

    //emit controlPointsChanged(ctrl_points_list);
}

void gnomonCoreParameterNurbsObject::setFigureNumber(int fig)
{
    d->figureNumber = fig;

    if(!d->pCurve) {
        d->initPCurve(m_param);
    }

    if(!d->pVisCurve) {
        d->initPVisCurve(m_param->dimension(), m_param->is_function());
    }

    if(d->pVisCurve && d->pCurve) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        PyObject* pFunc = PyObject_GetAttrString(d->pVisCurve, "setFigureNumber");
        PyObject *p_fig = PyLong_FromLong(fig);
        PyObject_CallOneArg(pFunc, p_fig);
        Py_DECREF(p_fig);
        Py_DECREF(pFunc);

        PyObject* pFunc2 = PyObject_GetAttrString(d->pCurve, "render");
        PyObject_CallNoArgs(pFunc2);
        Py_DECREF(pFunc2);

        PyGILState_Release(gstate);
    } else {
        dtkWarn() << "Problem pCurve or pVisCurve is not initialized";
    }
}
