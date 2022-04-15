%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init %{
import_array();
%}


%define %apply_numpy_typemaps(TYPE, DATA_TYPECODE)

// 1 D array
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) (TYPE* IN_ARRAY1, int DIM) {
  int ndims_ok = PyArray_NDIM((PyArrayObject*)$input) == 1 ? 1 : 0;
    
  $1 = (($input)
        && PyArray_Check($input) 
        && ndims_ok
        && PyArray_EquivTypenums(PyArray_TYPE((PyArrayObject*)$input), DATA_TYPECODE) ) ? 1 : 0;
}
%typemap(in, fragment="NumPy_Fragments")
  (TYPE* IN_ARRAY1, int DIM)
  (PyArrayObject* array=NULL, int is_new_object=0)
{
  npy_intp size[1] = { -1 };
  array = obj_to_array_contiguous_allow_conversion($input,
                                                   DATA_TYPECODE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 1) ||
      !require_size(array, size, 1)) SWIG_fail;
  $1 = (TYPE*) array_data(array);
  $2 = (int) array_size(array,0);
}
%typemap(freearg) (TYPE* IN_ARRAY1, int DIM)
{
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}


//2D array
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) (TYPE* IN_ARRAY2, int DIM1, int DIM2) {
  int ndims_ok = PyArray_NDIM((PyArrayObject*)$input) == 2 ? 1 : 0;
  $1 = (($input)
        && PyArray_Check($input) 
        && ndims_ok
        && PyArray_EquivTypenums(PyArray_TYPE((PyArrayObject*)$input), DATA_TYPECODE) ) ? 1 : 0;
}
%typemap(in, fragment="NumPy_Fragments")
  (DATA_TYPE* IN_ARRAY2, DIM_TYPE DIM1, DIM_TYPE DIM2)
  (PyArrayObject* array=NULL, int is_new_object=0)
{
  npy_intp size[2] = { -1, -1 };
  array = obj_to_array_contiguous_allow_conversion($input, DATA_TYPECODE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 2) ||
      !require_size(array, size, 2)) SWIG_fail;
  $1 = (DATA_TYPE*) array_data(array);
  $2 = (DIM_TYPE) array_size(array,0);
  $3 = (DIM_TYPE) array_size(array,1);
}
%typemap(freearg) (DATA_TYPE* IN_ARRAY2, DIM_TYPE DIM1, DIM_TYPE DIM2)
{
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}

//3D array
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) (TYPE* IN_ARRAY3, int DIM1, int DIM2, int DIM3) {
  int ndims_ok = PyArray_NDIM((PyArrayObject*)$input) == 3 ? 1 : 0;
  $1 = (($input)
        && PyArray_Check($input) 
        && ndims_ok
        && PyArray_EquivTypenums(PyArray_TYPE((PyArrayObject*)$input), DATA_TYPECODE) ) ? 1 : 0;
}
%typemap(in, fragment="NumPy_Fragments")
  (DATA_TYPE* IN_ARRAY3, DIM_TYPE DIM1, DIM_TYPE DIM2, DIM_TYPE DIM3)
  (PyArrayObject* array=NULL, int is_new_object=0)
{
  npy_intp size[3] = { -1, -1, -1 };
  array = obj_to_array_contiguous_allow_conversion($input, DATA_TYPECODE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 3) ||
      !require_size(array, size, 3)) SWIG_fail;
  $1 = (DATA_TYPE*) array_data(array);
  $2 = (DIM_TYPE) array_size(array,0);
  $3 = (DIM_TYPE) array_size(array,1);
  $4 = (DIM_TYPE) array_size(array,2);
}
%typemap(freearg) (DATA_TYPE* IN_ARRAY3, DIM_TYPE DIM1, DIM_TYPE DIM2, DIM_TYPE DIM3)
{
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}

//4D array
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) (TYPE* IN_ARRAY4, int DIM1, int DIM2, int DIM3, int DIM4) {
  int ndims_ok = PyArray_NDIM((PyArrayObject*)$input) == 4 ? 1 : 0;
  $1 = (($input)
        && PyArray_Check($input) 
        && ndims_ok
        && PyArray_EquivTypenums(PyArray_TYPE((PyArrayObject*)$input), DATA_TYPECODE) ) ? 1 : 0;
}
%typemap(in, fragment="NumPy_Fragments")
  (DATA_TYPE* IN_ARRAY4, DIM_TYPE DIM1, DIM_TYPE DIM2, DIM_TYPE DIM3, DIM_TYPE DIM4)
  (PyArrayObject* array=NULL, int is_new_object=0)
{
  npy_intp size[3] = { -1, -1, -1, -1 };
  array = obj_to_array_contiguous_allow_conversion($input, DATA_TYPECODE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 4) ||
      !require_size(array, size, 4)) SWIG_fail;
  $1 = (DATA_TYPE*) array_data(array);
  $2 = (DIM_TYPE) array_size(array,0);
  $3 = (DIM_TYPE) array_size(array,1);
  $4 = (DIM_TYPE) array_size(array,2);
  $5 = (DIM_TYPE) array_size(array,3);
}
%typemap(freearg) (DATA_TYPE* IN_ARRAY4, DIM_TYPE DIM1, DIM_TYPE DIM2, DIM_TYPE DIM3, DIM_TYPE DIM4)
{
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}


%extend QVariant {
    void setValue(TYPE *IN_ARRAY1, int DIM) {
        QVector<TYPE> vec(IN_ARRAY1, IN_ARRAY1 + DIM);
        $self->setValue(vec);
    }

    void setValue(TYPE *IN_ARRAY2, int DIM1, int DIM2) {
        QVector<QVector<TYPE>> vec(DIM1, QVector<TYPE>(DIM2));
        for(int i=0; i<DIM1; ++i) {
            for(int j=0; j<DIM2; ++j) {
                vec[i][j] = IN_ARRAY2[i*DIM2 + j];
            }
        }
        $self->setValue(vec);
    }

    void setValue(TYPE *IN_ARRAY3, int DIM1, int DIM2, int DIM3) {
        QVector<QVector<QVector<TYPE>>> vec(DIM1,
                                            QVector<QVector<TYPE>>(DIM2,
                                                                   QVector<TYPE>(DIM3)));
        for(int i=0; i<DIM1; ++i) {
            for(int j=0; j<DIM2; ++j) {
                for(int k=0; k<DIM3; ++k) {
                    vec[i][j][k] = IN_ARRAY3[i*DIM2*DIM3 + j*DIM3 + k];
                }
            }
        }
        $self->setValue(vec);
    }

    void setValue(TYPE *IN_ARRAY4, int DIM1, int DIM2, int DIM3, int DIM4) {
        QVector<QVector<QVector<QVector<TYPE>>>> vec(DIM1,
                                                     QVector<QVector<QVector<TYPE>>>(DIM2,
                                                                                     QVector<QVector<TYPE>>(DIM3,
                                                                                                            QVector<TYPE>(DIM4))));
        for(int i=0; i<DIM1; ++i) {
            for(int j=0; j<DIM2; ++j) {
                for(int k=0; k<DIM3; ++k) {
                    for(int l=0; l<DIM4; ++l) {
                        vec[i][j][k][l] = IN_ARRAY4[i*DIM2*DIM3*DIM4 + j*DIM3*DIM4 + k*DIM4 + l];
                    }
                }
            }
        }
        $self->setValue(vec);
    }

}

%inline %{
    PyObject *toNpArray1##TYPE (const QVariant &var) {
        QVector<TYPE> vec = var.value<QVector<TYPE>>();
        npy_intp dims[1] = { vec.size() };
        PyObject *array = PyArray_SimpleNew(1, dims, DATA_TYPECODE);
        if (!array) {
            qWarning() << Q_FUNC_INFO << "cant create new python array of dim" << dims[0] << " and type DATA_TYPECODE";
            return nullptr;
        }
        TYPE *data = (TYPE *) PyArray_DATA((PyArrayObject*)array);
        for(int i=0; i< vec.size(); ++i) {
            data[i] = vec[i];
        }

        return array;
    }

    PyObject *toNpArray2##TYPE (const QVariant &var) {
        QVector<QVector<TYPE>> vec = var.value<QVector<QVector<TYPE>>>();
        int rows = vec.size();
        int cols = vec[0].size();
        npy_intp dims[2] = { rows, cols };
        PyObject *array = PyArray_SimpleNew(2, dims, DATA_TYPECODE);
        if (!array) {
            qWarning() << Q_FUNC_INFO << "cant create new python array of dim [" << dims[0] << "," << dims[1] << "] and type DATA_TYPECODE";
            return nullptr;
        }
        TYPE *data = (TYPE *) PyArray_DATA((PyArrayObject*)array);
        for(int i=0; i< rows; ++i) {
            for(int j=0; j< cols; ++j) {
                data[i*cols + j] = vec[i][j];
            }
        }

        return array;
    }

    PyObject *toNpArray3##TYPE (const QVariant &var) {
        QVector<QVector<QVector<TYPE>>> vec = var.value<QVector<QVector<QVector<TYPE>>>>();
        int d = vec.size();
        int dd = vec[0].size();
        int ddd = vec[0][0].size();
        npy_intp dims[3] = { d, dd, ddd };
        PyObject *array = PyArray_SimpleNew(3, dims, DATA_TYPECODE);
        if (!array) {
            qWarning() << Q_FUNC_INFO << "cant create new python array of dim [" 
                       << dims[0] << ", " << dims[1] << ", " << dims[2] << "] and type DATA_TYPECODE";
            return nullptr;
        }
        TYPE *data = (TYPE *) PyArray_DATA((PyArrayObject*)array);
        for(int i=0; i< d; ++i) {
            for(int j=0; j< dd; ++j) {
                for(int k=0; k < ddd; ++k) {
                    data[i*dd*ddd + j*ddd + k] = vec[i][j][k];
                }
            }
        }

        return array;
    }

    PyObject *toNpArray4##TYPE (const QVariant &var) {
        QVector<QVector<QVector<QVector<TYPE>>>> vec = var.value<QVector<QVector<QVector<QVector<TYPE>>>>>();
        int d = vec.size();
        int dd = vec[0].size();
        int ddd = vec[0][0].size();
        int dddd = vec[0][0][0].size();
        npy_intp dims[4] = { d, dd, ddd, dddd };
        PyObject *array = PyArray_SimpleNew(4, dims, DATA_TYPECODE);
        if (!array) {
            qWarning() << Q_FUNC_INFO << "cant create new python array of dim [" 
                       << dims[0] << ", " << dims[1] << ", " << dims[2] << ", " << dims[3] << "] and type DATA_TYPECODE";
            return nullptr;
        }
        TYPE *data = (TYPE *) PyArray_DATA((PyArrayObject*)array);
        for(int i=0; i< d; ++i) {
            for(int j=0; j< dd; ++j) {
                for(int k=0; k < ddd; ++k) {
                    for(int l=0; l < dddd; ++l) {
                        data[i*dd*ddd*dddd + j*ddd*dddd + k*dddd + l] = vec[i][j][k][l];
                    }
                }
            }
        }

        return array;
    }

%}

%enddef    /* %apply_numpy_typemaps() macro */
