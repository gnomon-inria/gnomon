import os
import sys
import traceback
import logging
import warnings
import importlib
import inspect
import re
import pickle
import zipfile

from base64 import b64decode, b64encode
from functools import wraps
from typing import Callable
from pathlib import Path
from tempfile import TemporaryDirectory
from json import load, loads, dump

from pkg_resources import iter_entry_points, resource_filename

from setuptools import findall

import gnomon.core
from gnomon.utils.logCapture import StreamCapture
from dtkcore import dtkCoreParameter

__PLUGINS__ = []
DEBUG = False


def get_factory(plugin_group: str):
    return getattr(gnomon.core, f"{plugin_group}_pluginFactory")


def import_plugins(file, excludes=[]):
    file = os.path.dirname(file)
    module = os.path.basename(file)
    path = file.split('/')
    ind = [i for i, n in enumerate(path) if n == 'gnomonplugins'][-1]
    module_path = '.'.join(path[ind:])

    for submodule in os.listdir(file):
        if submodule == '__init__.py' or submodule[-3:] != '.py' or submodule[:-3] in excludes:
            continue
        try:
            __import__(module_path + "." + submodule[:-3], locals(), globals())
        except:
            logging.info(module_path + "." + submodule[:-3])
            traceback.print_exc()


def load_plugin_group(group_name: str):
    """
    Load a plugin group by importing the module of every entry point in the group

    Parameters
    ----------
    group_name: str
        Entry point group
    """
    for i, entry_point in enumerate(iter_entry_points(group=group_name, name=None)):
        logging.info(f"loading {entry_point.name}: ")
        try:
            importlib.import_module(entry_point.module_name)
        except Exception as e:
            logging.info(" --> FAIL")
            print(e)


def available_plugins(group_name: str) -> list[str]:
    """
    Return the name of every entry point registered in the group (group_name)

    Parameters
    ----------
    group_name: str
        plugin group

    Returns
    -------
    list[str]
        list of the plugin names in the plugin group (keys of the related factory)
    """
    return [ep.name for ep in iter_entry_points(group=group_name, name=None)]


def plugin_metadata(group_name: str, plugin_name: str) -> dict[str, str]:
    """
    Returns a dict of metadata regarding the plugin and its package.

    Package information:
        - package: the name of the package (in conda for instance as it may differ from the import statement)
        - conda_channel: channel from which to pull this package
    Parameters
    ----------
    group_name: str
        Name of the plugin group (or entry_point group)

    plugin_name: str
        Name of the plugin


    Returns
    -------
    Returns a dictionary of string to string containing metadata regarding the plugin and the package.

    """
    out = {}
    try:
        ep = next(iter_entry_points(group_name, name=plugin_name))
    except StopIteration:
        raise ValueError(f"No entry point found names {plugin_name} in group {group_name}")
    
    root_module = importlib.import_module(ep.module_name.split(".")[0])
    try:
        out["package"] = root_module.package
        out["conda_channel"] = root_module.conda_channel
    except AttributeError as e:
        print(f"Missing either 'package' or 'conda_channel' from root package {ep.module_name.split('.')[0]}")

    # plugin metadata
    *module, resource = ep.module_name.split(".")
    path = resource_filename(".".join(module), resource + ".json")
    if os.path.exists(path):
        with open(path, "r") as file:
            out.update(load(file))
    return out


def default_input_accessors(algo_class, form_class: type) -> tuple[str, str]:
    """
    Returns the default accessors for an input of type form_class from algo_class.

    Requires algo_class to define the static methods:
        defaultGetter(form_class.__name__)

        defaultSetter(form_class.__name__)

    Parameters
    ----------
    algo_class: gnomon.core.gnomonAbstractAlgorithm
        base class of the plugin which must implement two static methods
        `defaultGetter(form_class.__name__)` and `defaultSetter(form_class.__name__)`
    form_class: gnomon.core.gnomonAbstractForm
        type of the form for those accessors

    Returns
    -------

    """
    setter_method = algo_class.defaultSetter(form_class.__name__)
    if not setter_method:
        raise KeyError(f"{algo_class.__name__} does not accept input form {form_class.__name__}")
    getter_method = algo_class.defaultGetter(form_class.__name__)
    if not getter_method:
        raise KeyError(f"{algo_class.__name__} does not accept input form {form_class.__name__}")
    return getter_method, setter_method


def default_output_accessors(algo_class, form_class) -> str:
    """
    Returns the default getter for an output of type form_class from algo_class.

    Requires algo_class to define the static methods:
        defaultOutput(form_class.__name__)

    Parameters
    ----------
    algo_class: gnomon.core.gnomonAbstractAlgorithm
        base class of the plugin which must implement two static methods
        `defaultOutput(form_class.__name__)`
    form_class: gnomon.core.gnomonAbstractForm
        type of the form for those accessors

    Returns
    -------

    """
    bound_method = algo_class.defaultOutput(form_class.__name__)
    if not bound_method:
        raise KeyError(f"{algo_class.__name__} does not accept output form {form_class.__name__}")
    return bound_method


def gnomon_declare_plugins(path: str) -> dict[str, list[str]]:
    """
    Returns the entry_points dict used to declare the plugins in plugin groups.

    Pre-defines setuptools entry_points by parsing every python file in path (recursively)
    scanning for the class name and the base class to generate the correct entry point.

    Parsing as follows: `class [class name](gnomonAbstract[base class])`
    where [base class] is the entry_points group and [class name] the entry_point name.

    The entry_point exposes the module.

    Parameters
    ----------
    path: str
        path where to scan the modules for entry_points (usually package root)

    Returns
    -------
    entry_points dict used to declare the plugins in setuptools
    """
    script = findall(path)
    script = [f for f in script if (f.endswith('.py')) and ('__init__' not in f)]

    module_dict = {}

    path_form = [f.replace('/', '.').replace('src.', '') for f in script]

    for module, file in zip(path_form, script):
        with open(file) as f:
            print("  --> Parsing " + str(file))
            datafile = f.readlines()
            for line in datafile:
                if 'class' in line and 'gnomonAbstract' in line:
                    cls = line.split('gnomonAbstract')[1].split(')')[0]
                    name = line.split('class ')[1].split('(')[0]
                    cls = cls[0].lower() + cls[1:]
                    if cls not in module_dict.keys():
                        module_dict[cls] = []
                    module_dict[cls].append(name + ' = ' + module.replace('.py', ''))

    return module_dict


def gnomonParametric(cls):
    """
    Class decorator: implements methods and special methods related to dtkCoreParameter use.

    Those methods access dtkCoreParameter (cross-parameters) which are stored in the dict attribute
    _parameters mapping keys to dtkCoreParameter.

    Implements:
        special methods __setitem__ and __getitem__ to set and get values to and from parameters

        setParameter(self, parameter_name, parameter_value)
            sets parameter_value to self._parameters[parameter_name]
        setParameters(self, params)
            params is a dict of (parameter_name, parameter_value) and setParameters sets the value
            of each self._parameters[parameter_name] to parameter_value.
            parameter_name must already be a key of self._parameters
        parameters(self)
            returns a copy of _parameters
        parameterDict(self)
            returns a dict of (parameter_name, parameter_value)
        parameterGroups(self)
            used the hidden attribute _parameter_groups to define a map that associates parameter names
            to a group name. By default, the group name "" is assigned to all parameters

    Returns
    -------
    decorated class
    """

    # -----------------------------------------------------
    # Plugin parameters concept
    # -----------------------------------------------------

    def __setitem__(self, key, value):
        self.setParameter(key, value)

    cls.__setitem__ = __setitem__

    def __getitem__(self, key):
        if  self._parameters[key].typeName() == "dtkCoreParameterPath":
            return self._parameters[key].path()
        else:
            return self._parameters[key].value()

    cls.__getitem__ = __getitem__

    def setParameter(self, parameter_name, parameter_value):
        logging.info(f"{parameter_name}: {parameter_value}")
        if parameter_name in self._parameters:
            if isinstance(parameter_value, dtkCoreParameter):
                self._parameters[parameter_name] = parameter_value
            else:
                self._parameters[parameter_name].setValue(parameter_value)
        else:
            warnings.warn("'" + parameter_name + "' is not a valid parameter")

    cls.setParameter = setParameter

    def setParameters(self, params):
        for (parameter_name, parameter_value) in params.items():
            self.setParameter(parameter_name, parameter_value)

    cls.setParameters = setParameters

    def parameters(self):
        return {k: v for k, v in self._parameters.items()}

    cls.parameters = parameters

    def parameterDict(self):
        return {key: value.value() for key, value in self.parameters().items()}

    cls.parameterDict = parameterDict

    def parameterGroups(self):
        groups = {}
        for parameter_name in self._parameters.keys():
            if not hasattr(self, "_parameter_groups") or parameter_name not in self._parameter_groups.keys():
                groups[parameter_name] = ""
            else:
                groups[parameter_name] = self._parameter_groups[parameter_name]
        return groups

    cls.parameterGroups = parameterGroups

    return cls


def serialize(attr):
    """
    Decorator which implements a 'serialize' and 'deserialize' method.

    'serialize' serializes attribute attr with pickle and then encodes it in base64
    'deserialize' does the inverse operation

    Parameters
    ----------
    attr: str
        name of the attribute that will be serialized

    Returns
    -------
    class with a 'serialize' and 'deserialize' method implemented
    """

    def decorator(cls: type):
        def serialize_func(self: object) -> str:
            return b64encode(pickle.dumps(getattr(self, attr))).decode("ascii")

        def deserialize_func(self: object, serialization: str):
            setattr(self, attr, pickle.loads(b64decode(serialization.encode("ascii"))))

        setattr(cls, "serialize", serialize_func)
        setattr(cls, "deserialize", deserialize_func)
        return cls

    return decorator


def seriesReader(form_attr: str, path_attr: str = "path"):
    """
    Decorator for Reader plugins which enables the use of the series container format.

    Wraps the run method to extract and read the forms from the container when a .zip
    file is selected.

    The decorated plugin must have a run method which can read multiple files
    (string of comma-seperated paths).

    Parameters
    ----------
    form_attr: str
        Name of the form attribute where the form read are stored.
    path_attr: str
        Name of the attribute containing the path to be read.
        
    Returns
    -------
    Class
        Decorated plugin
    """
    def seriesReaderDecorator(cls: type):
        def run_decorator(f: Callable):
            @wraps(f)
            def run_wrapper(self):
                old_paths = getattr(self, path_attr).split(",")
                path = old_paths[0]
                ext = Path(path).suffix
                # logging.info("======================" + repr(ext) + repr(old_paths))
                if ext == ".zip" and len(old_paths) > 1:
                    raise RuntimeError("When opening a series container expected only one.")
                elif ext != ".zip":
                    return f(self)

                # series handling
                new_paths = []
                time_stamps = []
                with TemporaryDirectory() as tmpdirname:
                    # zip file
                    # logging.info(path)
                    container = zipfile.ZipFile(path, mode="r")
                    if "manifest.json" not in container.namelist():
                        raise RuntimeError("Invalid series container, no manifest.json file found.")
                    manifest = loads(container.read("manifest.json").decode("utf-8"))
                    for t, filename in manifest["series"].items():
                        new_paths.append(container.extract(filename, tmpdirname))
                        time_stamps.append(t)
                    container.close()
                    # logging.info(new_paths)
                    self.setPath(",".join(new_paths))
                    f(self)
                    #logging.info(getattr(self, form_attr))
                    #setattr(self, form_attr, {t: getattr(self, form_attr)[i] for i, t in enumerate(time_stamps)})
                    #logging.info(getattr(self, "image")())

                self.setPath = old_paths

            return run_wrapper

        setattr(cls, "run", run_decorator(cls.run))

        def preview(self):
            return f"{os.path.splitext(inspect.getfile(cls))[0]}.png"
    
        setattr(cls, "preview", preview)

        return cls
    return seriesReaderDecorator


def seriesWriter(form_attr: str, path_attr: str = "path"):
    """
    Decorator for Writer plugins which enables the use of the series container format.

    Wraps the run method to write each frame of a form series in a .zip container
    as well as saving the timestamps. Only applies for series of more than one frame (timestamp).

    The decorated plugin must have a run method which can write a file.

    Parameters
    ----------
    form_attr: str
        Name of the form attribute where the form_series written to the disk is stored.
    path_attr: str
        Name of the attribute containing the path where to write.

    Returns
    -------
    Class
        Decorated plugin
    """
    def seriesWriterDecorator(cls: type):
        def writerDecorator(f):
            @wraps(f)
            def run_wrapper(self):
                paths = getattr(self, path_attr).split(",")
                path = paths[0]
                forms = getattr(self, form_attr)
                if len(forms) == 1:
                    return f(self)
                # writing the series
                with TemporaryDirectory() as tmpdirname:
                    container = zipfile.ZipFile(Path(path).with_suffix(".zip"), "w", compression=zipfile.ZIP_DEFLATED,
                                                compresslevel=5)
                    ext = Path(path).suffix if Path(path).suffix != ".zip" else self.extensions()[0]
                    manifest = {"extension": ext[1:], "series": {}}
                    for t, form in forms.items():
                        filename = Path(path).stem + "_t" + "%05.2f" % t + ext
                        manifest["series"][t] = filename
                        filepath = Path(tmpdirname).joinpath(filename)
                        self.setPath(str(filepath))
                        setattr(self, form_attr, {t: form})
                        f(self)
                        container.write(str(filepath), str(filename))
                    # writing manifest
                    filepath = Path(tmpdirname).joinpath("manifest.json")
                    with open(filepath, "w") as file:
                        dump(manifest, file)
                    container.write(filepath, "manifest.json")

                    container.close()
                    # resetting
                    setattr(self, form_attr, forms)
                    setattr(self, path_attr, paths)

            return run_wrapper

        setattr(cls, "run", writerDecorator(cls.run))
        return cls

    return seriesWriterDecorator


def formDataPlugin(version: str, coreversion: str, data_setter: str, data_getter: str, base_class=None):
    """
    Registers form data plugins to the plugin factory.

    Must be the top decorator as it will wrap every method of the class to suppress errors.
    Error suppression can be deactivated by setting gnomon.utils.gnomonPlugin.DEBUG to True.

    A form data plugin is a class which implements a subclass of gnomon.core.gnomonAbstractFormData

    Parameters
    ----------
    version: str
        Version of the plugin.
    coreversion: str
        Exact version of gnomon to check for API compatibility.
    data_setter: str
        Name of the setter method which sets the data attribute (where the data is internally stored).
        The setter must take only one argument of the type of the data attribute. For instance, if
        the data attribute is a numpy array, then the prototype of the data_setter method must be
        `def data_setter(self, arr: np.ndarray) -> None:`
    data_getter: str
        Name of the getter method which returns a reference to the data attribute.
        The getter takes no arguments and returns (a reference to) the data attribute.
    base_class

    Returns
    -------

    """

    def decorator(cls):
        if not issubclass(cls, gnomon.core.gnomonAbstractFormData):
            raise TypeError(f"Class {cls.__name__} should be a subclass of a gnomonAbstractFormData interface."
                            f" Otherwise try using corePlugin or visualizationPlugin")

        cls.__data_setter = getattr(cls, data_setter)
        cls.__data_getter = getattr(cls, data_getter)
        cls = _gnomonPlugin(version, coreversion, cls, namespace=gnomon.core, base_class=base_class)
        return cls

    return decorator


def algorithmPlugin(version: str, coreversion: str, base_class=None):
    """
    Registers algorithm plugins to the plugin factory.

    Must be the top decorator as it will wrap every method of the class to suppress errors.
    Error suppression can be deactivated by setting gnomon.utils.gnomonPlugin.DEBUG to True.

    A form data plugin is a class which implements a subclass of gnomon.core.gnomonAbstractAlgorithm

    Applies the gnomonParametric decorator:
        Implements methods and special methods related to dtkCoreParameter use.
        Those methods access dtkCoreParameter (cross-parameters) which are stored in the dict attribute
        _parameters mapping keys to dtkCoreParameter.

        Implements:
            special methods __setitem__ and __getitem__ to set and get values to and from parameters

            setParameter(self, parameter_name, parameter_value)
                sets parameter_value to self._parameters[parameter_name]
            setParameters(self, params)
                params is a dict of (parameter_name, parameter_value) and setParameters sets the value
                of each self._parameters[parameter_name] to parameter_value.
                parameter_name must already be a key of self._parameters
            parameters(self)
                returns a copy of _parameters
            parameterDict(self)
                returns a dict of (parameter_name, parameter_value)


    Parameters
    ----------
    version: str
        Version of the plugin.
    coreversion: str
        Exact version of gnomon to check for API compatibility.
    base_class

    Returns
    -------

    """

    def decorator(cls):
        if not issubclass(cls, gnomon.core.gnomonAbstractAlgorithm):
            raise TypeError(f"Class {cls.__name__} should be a subclass of a gnomonAbstractAlgorithm interface."
                            f" Otherwise try using formDataPlugin or visualizationPlugin")
        cls = gnomonParametric(cls)  # integrating gnomonParametric in wrapper
        cls = _gnomonPlugin(version, coreversion, cls, namespace=gnomon.core, base_class=base_class)
        return cls

    return decorator


def corePlugin(version: str, coreversion: str, base_class=None):
    """
    Registers gnomon plugins which implements an interface from gnomon.core to the plugin factory.

    Must be the top decorator as it will wrap every method of the class to suppress errors.
    Error suppression can be deactivated by setting gnomon.utils.gnomonPlugin.DEBUG to True.

    Applies the gnomonParametric decorator:
        Implements methods and special methods related to dtkCoreParameter use.
        Those methods access dtkCoreParameter (cross-parameters) which are stored in the dict attribute
        _parameters mapping keys to dtkCoreParameter.

        Implements:
            special methods __setitem__ and __getitem__ to set and get values to and from parameters

            setParameter(self, parameter_name, parameter_value)
                sets parameter_value to self._parameters[parameter_name]
            setParameters(self, params)
                params is a dict of (parameter_name, parameter_value) and setParameters sets the value
                of each self._parameters[parameter_name] to parameter_value.
                parameter_name must already be a key of self._parameters
            parameters(self)
                returns a copy of _parameters
            parameterDict(self)
                returns a dict of (parameter_name, parameter_value)


    Parameters
    ----------
    version: str
        Version of the plugin.
    coreversion: str
        Exact version of gnomon to check for API compatibility.
    base_class

    Returns
    -------

    """

    def decorator(cls):
        cls = gnomonParametric(cls)  # integrating gnomonParametric in wrapper
        cls = _gnomonPlugin(version, coreversion, cls, namespace=gnomon.core, base_class=base_class)
        return cls

    return decorator


def visualizationPlugin(version: str, coreversion: str, base_class=None):
    """
    Registers visualization plugins to the plugin factory.

    Must be the top decorator as it will wrap every method of the class to suppress errors.
    Error suppression can be deactivated by setting gnomon.utils.gnomonPlugin.DEBUG to True.

    A form data plugin is a class which implements a subclass of either
    gnomon.visualization.gnomonAbstractVisualization or gnomon.visualization.gnomonAbstractMatplotlibVisualization

    Applies the gnomonParametric decorator:
        Implements methods and special methods related to dtkCoreParameter use.
        Those methods access dtkCoreParameter (cross-parameters) which are stored in the dict attribute
        _parameters mapping keys to dtkCoreParameter.

        Implements:
            special methods __setitem__ and __getitem__ to set and get values to and from parameters

            setParameter(self, parameter_name, parameter_value)
                sets parameter_value to self._parameters[parameter_name]
            setParameters(self, params)
                params is a dict of (parameter_name, parameter_value) and setParameters sets the value
                of each self._parameters[parameter_name] to parameter_value.
                parameter_name must already be a key of self._parameters
            parameters(self)
                returns a copy of _parameters
            parameterDict(self)
                returns a dict of (parameter_name, parameter_value)


    Parameters
    ----------
    version: str
        Version of the plugin.
    coreversion: str
        Exact version of gnomon to check for API compatibility.
    base_class

    Returns
    -------

    """

    def decorator(cls):
        import gnomon.visualization

        if not (issubclass(cls, gnomon.visualization.gnomonAbstractVisualization) or
                issubclass(cls, gnomon.visualization.gnomonAbstractMatplotlibVisualization)):
            raise TypeError(f"Class {cls.__name__} should be a subclass of a gnomonAbstractVisualization interface."
                            f" Otherwise try using corePlugin or formDataPlugin")
        cls = gnomonParametric(cls)  # integrating gnomonParametric in wrapper
        cls = _gnomonPlugin(version, coreversion, cls, namespace=gnomon.visualization, base_class=base_class)
        return cls

    return decorator


def _gnomonPlugin(version, coreversion, cls, namespace, base_class=None):
    # -----------------------------------------------------
    # Doc and Version
    # -----------------------------------------------------

    def documentation(self):
        doc = self.__doc__
        if doc is not None:
            # doc = doc.replace("    ","")
            doc = re.split("--+", doc)[0]
            doc = re.split("\n  +[A-z]*\n", doc)[0]
            doc = doc.replace("\n    \n", "\n\n\n")
            doc = doc.replace("\n\n", "\n\n\n")
            doc = doc.replace("\n    ", " ")
            doc = doc.replace("\n ", "\n")
            doc = "\n" + doc + "\n\n"
        else:
            doc = "\nThis plugin has no documentation\n\n"
        return doc

    cls.documentation = documentation

    cls.__version__ = version

    def _version(self):
        return self.__version__

    cls.version = _version

    # -----------------------------------------------------
    # TCP Logging
    # -----------------------------------------------------

    # attach output capture to run method
    if hasattr(cls, "run"):
        _old_run = cls.run

        @wraps(_old_run)
        def logger_init(self, *args, **kwargs):
            # logger init
            _logger = None
            try:
                _logger = StreamCapture([sys.stdout, sys.stderr], echo=True)
            except Exception as e:
                logging.warn("Could not initialize logger. Server probably not found.")
                pass
            # base run
            out = _old_run(self, *args, **kwargs)
            # cleanup
            if _logger:
                _logger.close()
            return out

        cls.run = logger_init

    # -----------------------------------------------------
    # Python error management
    # -----------------------------------------------------

    def wrapper(f):
        @wraps(f)
        def func(self, *args, **kwargs):
            try:
                return f(self, *args, **kwargs)
            except Exception as e:
                if DEBUG:  # if debug let it throw
                    raise
                traceback.print_exc()
                print(e)

        return func

    for key, value in cls.__dict__.items():
        if callable(value):
            setattr(cls, key, wrapper(value))

    def pluginName(self):
        return cls.__name__

    setattr(cls, "pluginName", pluginName)

    # -----------------------------------------------------
    # Factory registration
    # -----------------------------------------------------
    if base_class is None:
        base_class = cls.__bases__[0]

    gnomonPluginBaseClass = getattr(namespace, base_class.__name__ + "Plugin")

    def checkVersion(plugin_coreversion: str) -> bool:
        coreversion = ("${gnomon_VERSION}").split('.')
        plugin_coreversion = plugin_coreversion.split('.')

        if int(coreversion[0]) != int(plugin_coreversion[0]):
            return False

        # if minor level of ref < elem return false
        # TODO put < when major > 0
        if int(coreversion[1]) < int(plugin_coreversion[1]):
            return False
        else:
            # no patch level specified in ref
            if len(coreversion) < 3 or len(plugin_coreversion) < 3:
                return True

            # if same minor level, compare patch level
            if (int(coreversion[1]) == int(plugin_coreversion[1])) and (
                    int(coreversion[2]) < int(plugin_coreversion[2])):
                return False
            # else minor level of elem < ref , then don't compare patch level

        return True

    class gnomonPluginClass(gnomonPluginBaseClass):
        def __init__(self):
            super(gnomonPluginClass, self).__init__()
            self.thisown = 0

        def create(self):
            try:
                obj = cls()
                obj.__disown__()
                return obj
            except Exception as e:
                print(e)
                raise e

    __PLUGINS__.append(gnomonPluginClass())

    plugin_factory_name = base_class.__name__.replace("gnomon", "", 1).replace("Abstract", "", 1)
    plugin_factory_name = plugin_factory_name[0].lower() + plugin_factory_name[1:]
    plugin_factory_name += '_pluginFactory'
    factory = getattr(namespace, plugin_factory_name)()

    plugin_name = cls.__name__
    plugin_name = plugin_name[0].lower() + plugin_name[1:]

    # TODO
    # check plugin gnomon_version to actual version before registering it
    # register plugin_version to be able to get it ?
    if checkVersion(coreversion):
        factory.recordPlugin(plugin_name, __PLUGINS__[-1])
        if plugin_name in factory.keys():
            logging.info("Python plugin " + str(plugin_name) + " has been successfully loaded!")
    else:
        logging.warn("Python plugin" + str(plugin_name) + "defined for core version " + str(
            coreversion) + " but actual version is ${gnomon_VERSION}")
        logging.warn("plugin not loaded")
    return cls
