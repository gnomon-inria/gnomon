#include "gnomonCoreParameterNurbsObject.h"

#include <math.h>

#include <QtCore>
#include <vtkActor.h>
#include <vtkRenderWindow.h>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#include <vtkPythonUtil.h>
#pragma pop_macro("slots")

// /////////////////////////////////////////////////////////////////////////////
// gnomonCoreParameterNurbsObjectPrivate
// /////////////////////////////////////////////////////////////////////////////
class gnomonCoreParameterNurbsObjectPrivate {
public:
    gnomonCoreParameterNurbsObjectPrivate() = default;
    ~gnomonCoreParameterNurbsObjectPrivate(void);

    void initPCurve(gnomonCoreParameterNurbs *param);
    void initPVisCurve(int dimension, bool is_function);
    void initPSurface(gnomonCoreParameterNurbs *param);
    void initPVisSurface(void);

public:
    int figureNumber=-1;
    double z_buffer = 0.;
    gnomonNurbsView* nurbsView = nullptr;
    gnomonNurbsView* nurbsViewWidget = nullptr;

    PyObject* pCurve = nullptr;
    PyObject* pVisCurve = nullptr;
    PyObject* pSurface = nullptr;
    PyObject* pVisSurface = nullptr;
    PyObject* pSurfaceWidget = nullptr;
    PyObject* pVisSurfaceWidget = nullptr;
};

gnomonCoreParameterNurbsObjectPrivate::~gnomonCoreParameterNurbsObjectPrivate(void)
{
    if(this->nurbsView)
        delete nurbsView;
    if(this->nurbsViewWidget)
        delete nurbsViewWidget;
}
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
                z_buffer = point[2];

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
            dtkWarn() << " pknotvector error with args " << param->degree() << int(param->controlPoints().size());
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

void gnomonCoreParameterNurbsObjectPrivate::initPSurface(gnomonCoreParameterNurbs *param) 
{
    if(!this->pSurface) {
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

        // 1 create surface
        PyObject* pFunc = PyObject_GetAttrString(pModule_bspline, "Surface");
        this->pSurface = PyObject_CallNoArgs(pFunc);
        Py_DECREF(pFunc);

        //2 set surface params
        //2.1 degree
        int degree = 3;
        PyObject *p_deg_u = PyLong_FromLong(degree);
        PyObject *p_deg_v = PyLong_FromLong(degree);
        PyObject_SetAttrString(this->pSurface, "degree_u", p_deg_u);
        PyObject_SetAttrString(this->pSurface, "degree_v", p_deg_v);
        Py_DECREF(p_deg_u);
        Py_DECREF(p_deg_v);

        //2.2 ctrlpoints
        if(param->controlPoints().size() > param->degree()) {
            PyObject *p_ctrlpts = PyList_New(int(param->controlPoints().size()));
            int i=0;
            for(auto point: param->controlPoints()) {
                PyObject *p_point = PyList_New(3);
                PyList_SetItem(p_point, 0, PyFloat_FromDouble(point[0]));
                PyList_SetItem(p_point, 1, PyFloat_FromDouble(point[1]));
                PyList_SetItem(p_point, 2, PyFloat_FromDouble(point[2]));
                PyList_SetItem(p_ctrlpts, i++, p_point);
                // Py_DECREF(p_point);
            }
            if(!p_ctrlpts) {
                dtkWarn() << Q_FUNC_INFO << "Error Building ControlPoint list!";
                Py_XDECREF(p_ctrlpts);
                return;
            }

            PyObject_SetAttrString(this->pSurface, "ctrlpts_size_u", PyLong_FromLong(param->cpsize()[0]));
            PyObject_SetAttrString(this->pSurface, "ctrlpts_size_v", PyLong_FromLong(param->cpsize()[1]));
            PyObject_SetAttrString(this->pSurface, "ctrlpts", p_ctrlpts);

            Py_DECREF(p_ctrlpts);
        }

        //2.3 update knotvector

        PyObject* pFunc_knot_u = PyObject_GetAttrString(pModule_utilities, "generate_knot_vector");
        PyObject* args_u = Py_BuildValue("(i, i)", degree, int(param->cpsize()[0]));
        PyObject* pknotvector_u = PyObject_CallObject(pFunc_knot_u, args_u);
        if(!pknotvector_u) {
            dtkWarn() << " pknotvector_u error with args " << degree << int(param->cpsize()[0]);
            return;
        } else {
            PyObject_SetAttrString(this->pSurface, "knotvector_u", pknotvector_u);
        }
        Py_DECREF(pknotvector_u);
        Py_XDECREF(args_u);
        Py_XDECREF(pFunc_knot_u);

        PyObject* pFunc_knot_v = PyObject_GetAttrString(pModule_utilities, "generate_knot_vector");
        PyObject* args_v = Py_BuildValue("(i, i)", degree, int(param->cpsize()[1]));
        PyObject* pknotvector_v = PyObject_CallObject(pFunc_knot_v, args_v);
        if(!pknotvector_v) {
            dtkWarn() << " pknotvector_u error with args " << degree << int(param->cpsize()[1]);
            return;
        } else {
            PyObject_SetAttrString(this->pSurface, "knotvector_v", pknotvector_v);
        }
        Py_DECREF(pknotvector_v);
        Py_XDECREF(args_v);
        Py_XDECREF(pFunc_knot_v);
        Py_DECREF(pModule_utilities);

        //2.4 delta
        PyObject* p_delta = PyFloat_FromDouble(std::sqrt(param->delta())/2); // 3D can not handle 0.01 delta
        PyObject_SetAttrString(this->pSurface, "delta", p_delta);
        Py_DECREF(p_delta);

        //3 Deepcopy pSurface into pSurfaceWidget
        PyObject* pNameCopy = PyUnicode_FromString("copy");
        PyObject* pModule_copy = PyImport_Import(pNameCopy);
        Py_DECREF(pNameCopy);
        if(pModule_copy) {
            PyObject* pDeepcopyFunc = PyObject_GetAttrString(pModule_copy, "deepcopy");
            if(pDeepcopyFunc && PyCallable_Check(pDeepcopyFunc)) {
                PyObject* deepcopy_args = Py_BuildValue("(O)", this->pSurface);
                this->pSurfaceWidget = PyObject_CallObject(pDeepcopyFunc, deepcopy_args);
                Py_DECREF(pDeepcopyFunc);
            }
            Py_DECREF(pModule_copy);
        }
        PyGILState_Release(gstate);
    }
}

void gnomonCoreParameterNurbsObjectPrivate::initPVisSurface(void)
{
    if(!this->pVisSurface) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        //3 set surface visu
        PyObject* pName3 = PyUnicode_FromString("gnomon.utils.VisVTK");
        PyObject* pModule_vis = PyImport_Import(pName3);
        if(!pModule_vis) {
            dtkWarn() << Q_FUNC_INFO << "Error importing module gnomon.utils.VisVTK";
            return;
        }
        Py_DECREF(pName3);

        PyObject* pFunc2 = PyObject_GetAttrString(pModule_vis, "VisSurface");
        PyObject* args = Py_BuildValue("(O)", this->pSurface);
        this->pVisSurface = PyObject_CallObject(pFunc2, args);
        if(!this->pVisSurface) {
            dtkWarn() << "Error making VisSurface, no NURBS visu";
        } else {
            PyObject_SetAttrString(this->pSurface, "vis", this->pVisSurface);
        }

        PyObject* pRenderFunc = PyObject_GetAttrString(this->pSurface, "render");
        PyObject* result = PyObject_CallNoArgs(pRenderFunc);

        PyObject* argsWidget = Py_BuildValue("(O)", this->pSurfaceWidget);
        this->pVisSurfaceWidget = PyObject_CallObject(pFunc2, argsWidget);
        if(!this->pVisSurfaceWidget) {
            dtkWarn() << "Error making Widget VisSurface, no NURBS widget visu";
        } else {
            PyObject_SetAttrString(this->pSurfaceWidget, "vis", this->pVisSurfaceWidget);
        }

        PyObject* pRenderFuncWidget = PyObject_GetAttrString(this->pSurfaceWidget, "render");
        PyObject_CallNoArgs(pRenderFuncWidget);

        Py_DECREF(argsWidget);
        Py_DECREF(pFunc2);
        Py_DECREF(pModule_vis);
        Py_DECREF(pRenderFunc);
        Py_DECREF(pRenderFuncWidget);

        PyGILState_Release(gstate);
    }
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonCoreParameterNurbsObject
// /////////////////////////////////////////////////////////////////////////////

gnomonCoreParameterNurbsObject::gnomonCoreParameterNurbsObject(gnomonCoreParameterNurbs *p) : dtkCoreParameterObject(p), m_param(p)
{
    d = new gnomonCoreParameterNurbsObjectPrivate();
    if(m_param->type() == gnomonCoreParameterNurbs::NURBS_TYPE::SURFACE) {
        d->nurbsView = new gnomonNurbsView(this);
        d->nurbsViewWidget = new gnomonNurbsView(this);
    }
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

void gnomonCoreParameterNurbsObject::updateControlPointsFromPython(void)
{
    //get theupdated list of control points
    // they may have changed with mouse interaction!
    if(!d->pCurve && !d->pSurface)
        return;

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    PyObject *p_ctrl_pts;
    if(d->pSurface) {
        p_ctrl_pts = PyObject_GetAttrString(d->pSurface, "_control_points");
    } else {
        p_ctrl_pts = PyObject_GetAttrString(d->pCurve, "_control_points");
    }

    gnomonCoreParameterNurbs::ctrls_type ctrl_points;
    if(PyList_Check(p_ctrl_pts) ) {
        int nb_points = PyList_Size(p_ctrl_pts);
        int nb_elem = PyList_Size(PyList_GET_ITEM(p_ctrl_pts, 0));
        for(int pt=0; pt < nb_points; ++pt) {
            double x,y,z;
            PyObject *p_pt = PyList_GET_ITEM(p_ctrl_pts, pt);
            x = PyFloat_AsDouble(PyList_GET_ITEM(p_pt, 0));
            y = PyFloat_AsDouble(PyList_GET_ITEM(p_pt, 1));
            if(nb_elem == 3) {
                z = PyFloat_AsDouble(PyList_GET_ITEM(p_pt, 2));
            } else {
                z = d->z_buffer;
            }
            ctrl_points.append({x,y,z});
        }
    }

    Py_XDECREF(p_ctrl_pts);
    PyGILState_Release(gstate);
    m_param->setControlPoints(ctrl_points);
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

gnomonNurbsView* gnomonCoreParameterNurbsObject::nurbsView(void) 
{
    return d->nurbsView;
}

gnomonNurbsView* gnomonCoreParameterNurbsObject::nurbsViewWidget(void) 
{
    return d->nurbsViewWidget;
}

int gnomonCoreParameterNurbsObject::nurbsType(void) const
{
    return m_param->type();
}

void gnomonCoreParameterNurbsObject::setDegree(int degree)
{
    if(m_param->type() == gnomonCoreParameterNurbs::SURFACE) {

        m_param->setDegree(degree);
        if(!d->pSurface) {
            d->initPSurface(m_param);
        }

        if(d->pSurface) {

            PyGILState_STATE gstate;
            gstate = PyGILState_Ensure();

            PyObject *p_deg_u = PyLong_FromLong(m_param->degree());
            PyObject *p_deg_v = PyLong_FromLong(m_param->degree());
            PyObject_SetAttrString(d->pSurface, "degree_u", p_deg_u);
            PyObject_SetAttrString(d->pSurface, "degree_v", p_deg_v);
            Py_DECREF(p_deg_u);
            Py_DECREF(p_deg_v);

            PyGILState_Release(gstate);
        }
    } else {
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
}

void gnomonCoreParameterNurbsObject::setDelta(double delta)
{
    m_param->setDelta(delta);

    PyObject *obj = nullptr;
    if(m_param->type() == gnomonCoreParameterNurbs::SURFACE) {
        if(!d->pSurface) {
            d->initPSurface(m_param);
        }
        obj = d->pSurface;
    } else {
        if(!d->pCurve) {
            d->initPCurve(m_param);
        }
        obj = d->pCurve;
    }


    if(obj) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        PyObject *p_delta = PyFloat_FromDouble(m_param->delta());
        PyObject_SetAttrString(obj, "delta", p_delta);
        Py_DECREF(p_delta);

        PyGILState_Release(gstate);
    } else {
        dtkWarn() << "Problem pCurve/pSurface is not initialized";
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

    if(!ctrl_points.empty())
        d->z_buffer = ctrl_points.first()[2];

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

void gnomonCoreParameterNurbsObject::notifyControlPointsChanged()
{
    emit controlPointsChanged();
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

void gnomonCoreParameterNurbsObject::buildNurbsPatch(void)
{
    if(!d->pSurface) {
        d->initPSurface(m_param);
    }

    if(!d->pVisSurface) {
        d->initPVisSurface();
    }

    this->updateRenderWindow();
}

void gnomonCoreParameterNurbsObject::updateRenderWindow(void) {
    static bool do_it_once = true;
    if(m_param->type() == gnomonCoreParameterNurbs::SURFACE) {
        if (d->pVisSurface) {
            PyGILState_STATE gstate;
            gstate = PyGILState_Ensure();

            auto render_window = d->nurbsView->renderWindow();
            PyObject *pRenderWindow = vtkPythonUtil::GetObjectFromPointer(static_cast<vtkObjectBase *>(render_window));
            qDebug() << Q_FUNC_INFO << "set_render_window";
            if (!pRenderWindow) {
                dtkWarn() << "Could not convert render window from C++";
            } else {
                PyObject_CallMethod(d->pVisSurface, "set_render_window", "(O)", pRenderWindow);
            }
            Py_DECREF(pRenderWindow);
            if(do_it_once) {
                auto render_window_widget = d->nurbsViewWidget->renderWindow();
                PyObject *pRenderWindowWidget = vtkPythonUtil::GetObjectFromPointer(static_cast<vtkObjectBase *>(render_window_widget));
                qDebug() << Q_FUNC_INFO << "set_render_window";
                if (!pRenderWindowWidget) {
                    dtkWarn() << "Could not convert render window from C++";
                } else {
                    PyObject_CallMethod(d->pVisSurfaceWidget, "set_render_window", "(O)", pRenderWindowWidget);
                }
                Py_DECREF(pRenderWindowWidget);
                do_it_once = false;
            }

            PyGILState_Release(gstate);
        } else {
            dtkWarn() << "Problem pVisSurface is not initialized";
        }
    }
}