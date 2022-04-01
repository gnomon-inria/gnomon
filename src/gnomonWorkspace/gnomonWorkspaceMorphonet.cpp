#include "gnomonWorkspaceMorphonet.h"

#include <gnomonCore/simpleCrypt.h>
#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonVisualization/gnomonView/gnomonViewForm.h>

#include <QtCore>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <dtkScript>



/*
from : https://discourse.vtk.org/t/convert-vtk-array-to-numpy-array/3152/3
from vtk.util.numpy_support import vtk_to_numpy, numpy_to_vtk

def vtkToNumpy(data):
    temp = vtk_to_numpy(data.GetPointData().GetScalars())
    dims = data.GetDimensions()
    component = data.GetNumberOfScalarComponents()
    if component == 1:
        numpy_data = temp.reshape(dims[2], dims[1], dims[0])
        numpy_data = numpy_data.transpose(2,1,0)
    elif component == 3 or component == 4:
        if dims[2] == 1: # a 2D RGB image
            numpy_data = temp.reshape(dims[1], dims[0], component)
            numpy_data = numpy_data.transpose(0, 1, 2)
            numpy_data = np.flipud(numpy_data)
        else:
            raise RuntimeError('unknow type')
    return numpy_data

def numpyToVTK(data, multi_component=False, type='float'):
    '''
    multi_components: rgb has 3 components
    type：float or char
    '''
    if type == 'float':
        data_type = vtk.VTK_FLOAT
    elif type == 'char':
        data_type = vtk.VTK_UNSIGNED_CHAR
    else:
        raise RuntimeError('unknown type')
    if multi_component == False:
        if len(data.shape) == 2:
            data = data[:, :, np.newaxis]
        flat_data_array = data.transpose(2,1,0).flatten()
        vtk_data = numpy_to_vtk(num_array=flat_data_array, deep=True, array_type=data_type)
        shape = data.shape
    else:
        assert len(data.shape) == 3, 'only test for 2D RGB'
        flat_data_array = data.transpose(1, 0, 2)
        flat_data_array = np.reshape(flat_data_array, newshape=[-1, data.shape[2]])
        vtk_data = numpy_to_vtk(num_array=flat_data_array, deep=True, array_type=data_type)
        shape = [data.shape[0], data.shape[1], 1]
    img = vtk.vtkImageData()
    img.GetPointData().SetScalars(vtk_data)
    img.SetDimensions(shape[0], shape[1], shape[2])
    return img

    */
class gnomonWorkspaceMorphonetPrivate{

public:
    gnomonWorkspaceMorphonetPrivate() = default;
    ~gnomonWorkspaceMorphonetPrivate() = default;

public:
    QString encodePassword(const QString& password);    
    QString decodePassword(const QString& encoded_password);

    bool selectDataset(int id);
    void loadMNDataAtTime(int time);

    enum Status {
        Morphonet_NotLoaded,
        Morphonet_disconnected,
        Morphonet_connected
    };

public:
    QSettings settings = QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    Status morphonet_status = Morphonet_NotLoaded;
    PyObject *mn_module, *mn_net = nullptr;
    int current_id = -1, start_time=-1, end_time=-1;
    gnomonPipelineManager *pipeline_manager;

    gnomonViewForm *view = nullptr;
    //gnomonAbstractDynamicForm *current_form; // time_series ? 

private: 
    SimpleCrypt crypto = SimpleCrypt(Q_UINT64_C(0x0c2ad6a4adb3f073)); 
};


QString gnomonWorkspaceMorphonetPrivate::encodePassword(const QString& password) 
{
    return crypto.encryptToString(password);

    //QByteArray pswdAndSalt (password.toStdString().c_str()) ;
    //pswdAndSalt.append(salt);
    //return QString(QCryptographicHash::hash((pswdAndSalt),QCryptographicHash::Md5).toHex());
}


QString gnomonWorkspaceMorphonetPrivate::decodePassword(const QString& encoded_password) 
{
    return crypto.decryptToString(encoded_password);
}

bool gnomonWorkspaceMorphonetPrivate::selectDataset(int id)
{
    if(id != -1) {
        qInfo() << Q_FUNC_INFO << "setting morphonet dataset id to" << id;
        current_id = id;
    }

    dtkScriptInterpreterPython::instance()->childAcquireLock();
    PyObject *pFunc = PyObject_GetAttrString(this->mn_net, "select_dataset_by_id");
    PyObject *pArgs = Py_BuildValue("(i)", this->current_id);
    PyObject_CallObject(pFunc, pArgs);
    Py_XDECREF(pFunc);
    Py_XDECREF(pArgs); 

    //  set start_time and end_time
    PyObject *pMinTime = PyObject_GetAttrString(this->mn_net, "minTime");
    this->start_time = PyLong_AsLong(pMinTime);
    Py_XDECREF(pMinTime);

    PyObject *pMaxTime = PyObject_GetAttrString(this->mn_net, "maxTime");
    this->end_time = PyLong_AsLong(pMaxTime);
    Py_XDECREF(pMaxTime);
    dtkScriptInterpreterPython::instance()->childReleaseLock();

    if(this->start_time == -1 || this->end_time == -1) {
        qWarning() << Q_FUNC_INFO << "Problem selecting dataset id:" << this->current_id << " time:[" << this->start_time << ", " << this->end_time << "]";
        return false;
    }

    return true;
}

void gnomonWorkspaceMorphonetPrivate::loadMNDataAtTime(int time)
{
    dtkScriptInterpreterPython::instance()->childAcquireLock();

    PyObject *pTime = PyLong_FromLong(long(time));

    // if  Net.is_image_at(self,t,channel=0):
      //arr = Net.get_image_at (t)
      //arr np.array of type np.uint8
      //add the data to the view

    // else // it's a mesh
      // obj = get_mesh_at
      // obj: string the 3d data for the mesh

    dtkScriptInterpreterPython::instance()->childReleaseLock();

    qDebug() << Q_FUNC_INFO << "not implemented";
}

gnomonWorkspaceMorphonet::gnomonWorkspaceMorphonet(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonWorkspaceMorphonetPrivate();

    d->pipeline_manager = gnomonPipelineManager::instance();
    d->view = new gnomonViewForm(this);
    //d->view->setAcceptForm("gnomonBinaryImage", true);
    //d->view->setAcceptForm("gnomonCellComplex",true);
    //d->view->setAcceptForm("gnomonCellImage",true);
    //d->view->setAcceptForm("gnomonImage",true);
    //d->view->setAcceptForm("gnomonMesh",true);
    //d->view->setAcceptForm("gnomonPointCloud",true);

    connect(d->view, &gnomonViewForm::exportedForm, [=] (gnomonAbstractDynamicForm *f) {
        //TODO what to do in pipeline manager if data coming from morphonet? 
        d->pipeline_manager->addForm(f);
    });


    dtkScriptInterpreterPython::instance()->childAcquireLock(); // getting lock from main interpreter
    d->mn_module = PyImport_ImportModule("morphonet");
    dtkScriptInterpreterPython::instance()->childReleaseLock();

    if(!d->mn_module) {
        //timer to wait until qml component is created. Otherwise, it won't catch the message
        QTimer::singleShot(100, this, [this](){
            emit message("Cannot load morphonet, please install it (pip install morphonet)");
        });
    } else {
        d->morphonet_status = gnomonWorkspaceMorphonetPrivate::Morphonet_disconnected;

        //automatically try to connect to Morphonet using saved login and password
        if(d->settings.contains("morphonet/login") 
           && d->settings.contains("morphonet/password")  
           && this->login(d->settings.value("morphonet/login").toString(), d->decodePassword(d->settings.value("morphonet/password").toString()))) {
            d->morphonet_status = gnomonWorkspaceMorphonetPrivate::Morphonet_connected;

            //timer to wait until qml component is created. Otherwise, it won't catch the message
            QTimer::singleShot(100, this, [this](){
                emit connectionStatusChanged();
            });

        }
    }
}


gnomonWorkspaceMorphonet::~gnomonWorkspaceMorphonet()
{
    Py_XDECREF(d->mn_module);

    if(d->mn_net) 
        Py_XDECREF(d->mn_net);
    delete d;
}

bool gnomonWorkspaceMorphonet::login(const QString& login, const QString& passwd)
{
    if(d->morphonet_status == gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        this->disconnect();
    }

    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_disconnected) {
        qWarning() << "Morphonet not loaded, nothing done";
        emit message("Morphonet not loaded, nothing done");
        return false;
    }

    if(login.isEmpty() || passwd.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Empty login or password";
        emit message("Empty login or password");
        return false;
    }

    //try login
    dtkScriptInterpreterPython::instance()->childAcquireLock(); // getting lock from main interpreter
    PyObject *pFunc = PyObject_GetAttrString(d->mn_module, "Net");
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject *pArgs = Py_BuildValue("(ss)", login.toStdString().c_str() , passwd.toStdString().c_str());
        d->mn_net = PyObject_CallObject(pFunc, pArgs);
        Py_XDECREF(pFunc);
        Py_XDECREF(pArgs); 

        //check id
        PyObject *pId = PyObject_GetAttrString(d->mn_net, "id_people");
        long id = PyLong_AsLong(pId);
        Py_XDECREF(pId);

        if(id == -1) {
            emit message("cannot connect to Morphonet with login: " + login);
            return false;
        }
        qInfo() << "Connected to morphonet with id:" << id;
    } else {
        emit message("problem with morphonet API, check the installation");
        return false;
    }

    dtkScriptInterpreterPython::instance()->childReleaseLock();

    // save to settings
    d->settings.setValue("morphonet/login", login);
    d->settings.setValue("morphonet/password", d->encodePassword(passwd));

    d->morphonet_status == gnomonWorkspaceMorphonetPrivate::Morphonet_connected;
    emit connectionStatusChanged();
    return true;
}

bool gnomonWorkspaceMorphonet::disconnect(void)
{
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        return false;
    }

    qDebug() << Q_FUNC_INFO << "TODO  not implemented";    

    d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_disconnected;
    emit connectionStatusChanged();
    return true;
}

bool gnomonWorkspaceMorphonet::connected(void)
{
    return d->morphonet_status == gnomonWorkspaceMorphonetPrivate::Morphonet_connected;
}


int gnomonWorkspaceMorphonet::currentId(void) const
{
    return d->current_id;
}

int gnomonWorkspaceMorphonet::timeStart(void) const
{
    return d->start_time;
}

int gnomonWorkspaceMorphonet::timeEnd(void) const
{
    return d->end_time;
}

void gnomonWorkspaceMorphonet::setTimeStart(int new_time)
{
    d->start_time = new_time;
}

void gnomonWorkspaceMorphonet::setTimeEnd(int new_time)
{
    d->end_time = new_time;
}

void gnomonWorkspaceMorphonet::setCurrentId(int new_id)
{
    d->current_id = new_id;
}

QString gnomonWorkspaceMorphonet::datasetsInfo(const QString& search)
{
    QString res = "";
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return res;
    }

    dtkScriptInterpreterPython::instance()->childAcquireLock(); // getting lock from main interpreter
      
    PyObject *pFunc = PyObject_GetAttrString(d->mn_net, "_request");
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject *pDict = PyDict_New();
        PyObject *pArgs = Py_BuildValue("(Oss)", pDict, "/api/userrelatedset/", "GET");
        PyObject *ds_list = PyObject_CallObject(pFunc, pArgs);
        Py_XDECREF(pFunc);
        Py_XDECREF(pDict);
        Py_XDECREF(pArgs); 

        //list of dict
        PyObject *pJson_mod = PyImport_ImportModule("json");
        PyObject *pJson_dumps = PyObject_GetAttrString(pJson_mod, "dumps");
        PyObject *pJson_str = PyObject_CallOneArg(pJson_dumps, ds_list);
        res = PyUnicode_AsUTF8(pJson_str);
        Py_XDECREF(pJson_mod);
        Py_XDECREF(pJson_dumps);
        Py_XDECREF(ds_list);
        Py_XDECREF(pJson_str);
    }
    dtkScriptInterpreterPython::instance()->childReleaseLock();

    //TODO
    // can use  get_guy_by_id(self,id_guy) to get author name? 

    return res;
}

void gnomonWorkspaceMorphonet::importDatasetPreview(int id)
{
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return;
    }

    bool ok = d->selectDataset(id);
    if(!ok) {
        message("cannot select dataset:" + id);
        return;
    }

    //import first time of selected dataset and set it to the view
    d->loadMNDataAtTime(d->start_time);
}


void gnomonWorkspaceMorphonet::importDataset(int time_start, int time_end, int id)
{
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return;
    }

    if(id != -1) {
        qInfo() << Q_FUNC_INFO << "setting morphonet dataset id to" << id;
        d->current_id = id;
    }

    qDebug() << Q_FUNC_INFO << "import dataset from id" << d->current_id;

    //1 select dataset
    bool ok = d->selectDataset(d->current_id);
    if(!ok) {
        message("cannot select dataset:" + d->current_id);
        return;
    }

    // for( each time) {
      //d->loadMNDataAtTime(d->start_time);
    // }

    qDebug() << Q_FUNC_INFO << "TODO  not implemented";    
}

void gnomonWorkspaceMorphonet::exportDataset()
{
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return;
    }

    //upload_image_at(time, raw_data, channel (int optional))

    //upload from what's in the view

    // raw_data needs to be np.uint8 array
    qDebug() << Q_FUNC_INFO << "TODO  not implemented";    
}

gnomonViewForm *gnomonWorkspaceMorphonet::view(void)
{
    return d->view;
}
