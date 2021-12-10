# Defining a new Form to implement a data structure

The objective is to add a Form concept (e.g. `gnomonNewForm`) to the gnomoncore layer, but to do it in such way that the object could be seen from different perspectives, using a Bridge design pattern. Then the idea is to use an implementation of this concept using a Python library.

## Define the Form abstraction on the C++ side

```
gnomon
└───src
    └───gnomoncore
        └───gnomonForm
            └───gnomonNewForm
                │   CMakeLists.txt
                │   gnomonNewForm
                │   gnomonNewForm.h
                │   gnomonAbstractNewFormData
                │   gnomonAbstractNewFormData.h
                │   gnomonAbstractNewFormData.cpp

```

### Define the abstraction for the data of the Form

The starting point is the definition of an abstract class that will be implemented by one or several plugins (C++ or Python)


* Create an empty header file that will contain an abstract class to define the abstraction of the data contained in the Form structure: `gnomonAbstractNewFormData.h`

```c++
#pragma once

#include <gnomonCoreExport.h>
```

```c++
// gnomonAbstractNewFormData.h ends here
```

#### Abstract data class definition

 * Define the abstract class to set the members and methods of the data structure. All the class methods should be defined as pure virtual so that deriving classes will have to implement these functions. Start with a constructor and a destructor, as well as a `clone` method that will be useful for the bridge construct.

```c++
//  ///////////////////////////////////////////////////////////////////
//  gnomonAbstractNewFormData
//  ///////////////////////////////////////////////////////////////////

class gnomonAbstractNewFormData
{
public:
             gnomonAbstractNewFormData(void) = default;
    virtual ~gnomonAbstractNewFormData(void) {};

    virtual gnomonAbstractNewFormData* clone(void) const = 0;

};

```

* Define the methods required by the `gnomonAbstractForm` class. ote that at this stage, they are pure virtual methods, and do not override any method. However it will be the case in the bridge class, that will inherit  `gnomonAbstractForm`.

```c++
//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
```

* Then define the specific API of the class. Depending on the concept, you may have few or many methods to declare. For instance we can imagine our `newForm` to be composed of `elements` with unique ids and `float` numerical properties, accessible through such an API:

```c++
public:
    virtual QList<long> elementIds(void) const = 0;
    virtual long elementCount(void) const = 0;

    virtual QList<QString> elementPropertyNames(void) const = 0;

    virtual const QMap<long, float>& elementProperty(const QString& propertyName) const = 0;
    virtual       QMap<long, float>& elementProperty(const QString& propertyName) = 0;

```

* In case the class API relies on third-party libraries, don't forger to include the corresponding headers. In this case:

```c++
#include <QtCore>
```

* Use forward-declaration for symbols that will be defined elsewhere, to ensure that compilation goes well.

#### Add the dTK macros for the visibility of the abstraction

* Include the tools to make the concept visible for the application and add the export macro to the definition of the class...

```c++
#include <QtCore>

#include <gnomonCoreExport.h>
#include <dtkCore>

//  ///////////////////////////////////////////////////////////////////
//  gnomonAbstractNewFormData
//  ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractNewFormData

```

* ...and the declaration/namespace macros at the end of the file. Be careful to declare the concept starting with a lowercase letter: `newFormData`.

```c++
// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractNewFormData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractNewFormData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractNewFormData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractNewFormData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractNewFormData, GNOMONCORE_EXPORT, newFormData);
}

//
// gnomonAbstractNewFormData.h ends here
```

#### Write corresponding `.cpp` file

* Create the corresponding `gnomonAbstractNewFormData.cpp` file that will implement the namespace registration macro that will enable its registration to the manager of the layer. Again, be careful to have a lowercase concept.

```c++
#include "gnomonCore.h"
#include "gnomonAbstractNewFormData.h"

#include "gnomonCore.h"

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonAbstractNewFormData, newFormData, gnomonCore);
}

//
// gnomonNewFormData.cpp ends here

```

* Create an include file without extension `gnomonAbstractNewFormData` to be able to include the class (cpp guidelines)

```c++
#include "gnomonAbstractCellImageData.h"
```

### Define the concrete Form class as a data bridge

We will now create an **instantiable** class that will basically wrap some or all of the abstraction methods to produce an interface for some given use. Note that one abstraction may have several bridges, exposing different aspects of the class depending on the final use.

#### Write the Form bridge header

* Create a new empty header file `gnomonNewForm.h`. The class is no longer abstract but will inheriting the `gnomonAbstractForm` (abstract) class. This concrete class only has one member which is a **pointer** on an implementation of the abstract data class:

```c++
#pragma once


#include "gnomonAbstractNewFormData.h"
#include "gnomonForm/gnomonAbstractForm.h"


// ///////////////////////////////////////////////////////////////////
// gnomonNewForm
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonNewForm : public gnomonAbstractForm
{
protected:
    gnomonAbstractNewFormData *m_data;

};

//
// gnomonImage.h ends here
```

* The class being no longer abstract, all its methods have to be implemented. Since their implementation will be very short, we implemented them directly in the header file, starting with the constructor and destructor, that simply create/clone and delete the private data member.

```c++
public:
    explicit gnomonNewForm(void) : m_data(nullptr) {}
    explicit gnomonNewForm(gnomonAbstractNewFormData *data) : m_data(data) {}
             gnomonNewForm(const gnomonNewForm& other) : m_data(other.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonNewForm(*this); };

    ~gnomonNewForm(void) { if (m_data) { delete m_data; } m_data = nullptr; }
```

* We also add on overload of the `=` operator so that the setting an instance of the bridge class with another one goes well memory-wise (and copies the underlying data).

```c++
public:
    gnomonNewForm& operator = (const gnomonNewForm& other)
    {
        if (m_data != other.m_data) {
            if (m_data != nullptr) {
                delete m_data;
            }
            if(other.m_data != nullptr) {
                m_data = other.m_data->clone();
            } else {
                m_data = nullptr;
            }
        }
        return *this;
    }
```

* We also add getter and setters for the data member, making sure that the previous data object is deleted.

```c++
public:
    const gnomonAbstractNewFormData *data(void) const { return m_data; }
          gnomonAbstractNewFormData *data(void)       { return m_data; }

    void setData(gnomonAbstractNewFormData* data)
    {
        if (m_data) {
            delete m_data;
        }
        m_data = data;
    }
```

* The `gnomonAbstractForm` API requires a `name`method. Here, we declare a (fixed) name for the Form instances, correponding to the class name.

```c++
public:
    QString name(void) const override { return "gnomonNewForm"; }
```

* Then all the other class methods (including the ones overriding the `gnomonAbstractForm` methods) will simply be calls to the same method of the abstract data class, passing the arguments when necessary.

```c++
public:
    QMap<QString,QString> metadata(void) const override { return m_data->metadata(); }
    QString dataName(void) const override { return m_data->dataName(); }

public:
    QList<long> elementIds(void) const { return m_data->elementIds(); };
    long elementCount(void) const { return m_data->elementCount(); };

    QList<QString> elementPropertyNames(void) const { return m_data->elementPropertyNames(); };

    const QMap<long, float>& elementProperty(const QString& propertyName) const { return m_data->elementProperty(propertyName); };
          QMap<long, float>& elementProperty(const QString& propertyName)       { return m_data->elementProperty(propertyName); };
```

* Declare the `gnomonTimeSeries` corresponding to the Form directly in the header:

```c++
#include "gnomonAbstractNewFormData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"
```

```c++
// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonNewForm> gnomonNewFormSeries;
Q_DECLARE_METATYPE(gnomonNewFormSeries *)
```


* Add the `dtk` machinery and macros so that the export runs smoothly.

```c++
#include <gnomonCoreExport.h>

#include "gnomonAbstractNewFormData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

class GNOMONCORE_EXPORT gnomonNewForm
```

```c++
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonCellImage *)

//
// gnomonNewForm.h ends here
```

### Edit the compilation files to include the new abstraction

* In the `CMakeLists.txt` of the `gnomonNewForm/` directory, add the headers and sources using the gnomon-specific macros (defined in `gnomon/cmake/gnomonSubdirectoryAddFiles.cmake`)

```cmake
## #################################################################
## Sources
## #################################################################


ADD_GNOMON_SUBDIRECTORY_HEADERS(
  gnomonAbstractNewFormData
  gnomonAbstractNewFormData.h
  gnomonNewForm
  gnomonNewForm.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
  gnomonAbstractNewFormData.cpp)


######################################################################
### CMakeLists.txt ends here

```

* In the `CMakeLists.txt` of the `gnomonForm/` directory, add the `gnomonNewForm`  subdirectory, and also add the (exported) `_HEADERS` and `_SOURCES` in the respective sections.

```cmake
## #################################################################
## Inputs
## #################################################################

...
add_subdirectory(gnomonNewForm)
...

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
  ...
  ${${PROJECT_NAME}_FORM_NEWFORM_HEADERS}
  ...)


ADD_GNOMON_SOURCES(
  ...
  ${${PROJECT_NAME}_FORM_NEWFORM_SOURCES}
  ...)

######################################################################
### CMakeLists.txt ends here
```

### Include the headers at the top-level of the gnomonCore layer

* In the file `src/gnomonCore/gnomonCore`, include the newly defined headers, so that they are accessible when we do a `#include <gnomonCore>` outside the layer.

```c++
...
#include "gnomonForm/gnomonNewForm/gnomonAbstractNewFormData.h"
#include "gnomonForm/gnomonNewForm/gnomonNewForm.h"
...
```

### Wrap the abstraction to make it available in Python

* In the SWIG input file `wrp/gnomonCore/gnomonCore.i` first include the headers for the Form class and the abstract data class:

```c++
...
#include <gnomonCore/gnomonForm/gnomonNewForm/gnomonAbstractNewFormData.h>
#include <gnomonCore/gnomonForm/gnomonNewForm/gnomonNewForm.h>
...
```

* Use the `
  WRAP_GNOMONCORE_FORM_SERIES` SWIG macro to declare the typemaps that convert `gnomonTimeSeries` to / form a Python dictionary of Forms indexed by time:

```c++
WRAP_GNOMONCORE_FORM_SERIES(NewForm)
```

* At the bottom of the file, after the typemaps and before the templates, include the headers for SWIG.

```c++
%include <gnomonCore/gnomonForm/gnomonNewForm/gnomonAbstractNewFormData.h>
%include <gnomonCore/gnomonForm/gnomonNewForm/gnomonNewForm.h>
```

* Here, we can extend the wrapped class with a `__repr__` method that will allow it to be more nicely diplayed in Python.

```c++
%extend gnomonNewForm {
    const char* __repr__()
    {
        static std::string s;
        auto&& newForm = $self;
        QString str("<gnomoncore.gnomonNewForm");
        str += QString(" with %1 element(s)").arg(newForm->elementCount());
        str += QString(" at 0x%1>").arg((quintptr)newForm, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
    }
}
```

### Compile gnomon and install it with the new (wrapped) abstraction

* Run the compilation with the install option

```bash
cd build
make -j8 install
```

* In some cases, it might be necessary to clean the existing compiled wrappers:

```bash
rm -rf wrp/
rm -rf $CONDA_PREFIX/wrp/gnomon*
```

* If all goes well, you should be able to run in `ipython`:

```python
from gnomoncore import gnomonAbstractNewFormData, newFormData_pluginFactory
```

## Provide a Python plugin that implements the form abstraction

Within an [already existing plugin package](package.md), we will implement a Python class inheriting the abstract data class `gnomonAbstractNewFormData` that will override all its pure virtual methods.

The concrete class will typically **wrap an existing Python data structure** (e.g. `MyStructure`) to adapt it to the API specified by the abstract C++ class. The idea is then that the class has a **data structure member** (e.g. `data`) that will be used to fill in the different methods.

### Implement the Python class

* Create a new module `newFormDataMyStructure.py` that defines a class inheriting our Form data abstraction

```python
from gnomoncore import gnomonAbstractNewFormData

class newFormDataMyStructure(gnomonAbstractNewFormData):
    def __init__(self):
        super().__init__()
```

* Add a (hidden) `_data` member (that should be of type `MyStructure`) containing the actual data structure representing the Form. We also add a specific `set_data` method that *makes a copy* of the provided data structure to keep it as the `_data` member.

```python
from copy import deepcopy

from my_module import MyStructure
```

```python
    def __init__(self):
        super().__init__()
        self._data = MyStructure()

    def __del__(self):
        del self._data

    def set_data(self, data: MyStructure):
        self._data = deepcopy(data)
```

* To make sure that Forms can be manipulated safely memorywise on the C++ side, we need to implement the `clone` method that duplicates the underlying data structure:

```python
    def clone(self):
        _clone = newFormDataMyStructure()
        _clone.set_data(self._data)
        _clone.__disown__()
        return _clone
```

* To enable the **interoperability** of the form data class we also define a method to instantiate our Python class from an existing instance of the Form (potentially implemented by a different form data plugin)

```python
    def from_gnomonNewForm(self, form):
        form_data = form.data()
        if isinstance(data, newFormDataMyStructure):
            self.set_data(form_data._data)
        else:
            self._data = MyStructure()
            
            # fill in the structure using the Form API
            for eid in data.elementIds():
                self._data.add_element(eid)
                ...
        
        return self
```

* Then, we implement the virtual methods of the abstract class using the methods / attributes of the underlying data structure. First the methods needed by `gnomonAbstractForm`:

```python
    def metadata(self):
        metadata = {}
        metadata['Number of elements'] = str(self._data.nb_elements())
        
        return metadata

    def dataName(self):
        return "my_module.MyStructure"
```

* ...and the rest of the Form API

```python
    def elementIds(self):
        return [int(eid) for eid in self._data.elements()]

    def elementCount(self):
        return self._data.nb_elements()
    
    def elementPropertyNames(self):
        ...

    def elementProperty(self, propertyName):
        ...
```

* Finally, we add the `gnomonDecorator` plugin that implements all the
  necessary methods to register the plugin to the platform. Since the abstraction (and its plugin factory) is registered to the `gnomonCore` namespace, we need to specify it explicitly to the decorator.

```python
import gnomoncore
from gnomoncore import gnomonAbstractNewFormData

from gnomon_utils import gnomonPlugin

@gnomonPlugin(version="0.1.0", coreversion="0.18.0", namespace=gnomoncore)
class newFormDataMyStructure(gnomonAbstractNewFormData):
```