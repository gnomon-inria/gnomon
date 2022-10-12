from typing import Tuple, List, Dict, Callable
from threading import Thread
from functools import partial
import pathlib

import gnomon.utils.gnomonPlugin
from gnomon.utils.gnomonPlugin import load_plugin_group, get_factory
from gnomon.pipeline import gnomonPipeline, gnomonPipelineNode, gnomonPipelineNodeTask, gnomonPipelineEdge, gnomonPipelinePort
from gnomon.core import gnomonAbstractDynamicForm, gnomonAbstractAlgorithm

# if a plugin fail, everything should fail
gnomon.utils.gnomonPlugin.DEBUG = True

THREADING = True


class PNodeRunner:
    """
    Node wrapper which instantiate the corresponding plugin, parametrize them
    and provides a unified interface.

    Attributes
    ----------
    name: str
        Name of the node.
    algo: gnomonAbstractAlgorithm
        Algorithm plugin.
    inputs_connections: Dict[str, Tuple[str, str]]
        Dictionary of input-port -> (source-node, source-port) which maps the input connections
        of the node.
    inputs: Dict[str, Callable[[gnomonAbstractDynamicForm], None]]
        Dictionary mapping input ports with their respective setter method.
    outputs: Dict[str, Callable[[], gnomonAbstractDynamicForm]]
        Dictionary mapping output ports with their respective getter method.
    """
    name: str
    algo: gnomonAbstractAlgorithm
    inputs_connections: Dict[str, Tuple[str, str]]
    inputs: Dict[str, Callable[[gnomonAbstractDynamicForm], None]]
    outputs: Dict[str, Callable[[], gnomonAbstractDynamicForm]]
    data_dir: pathlib.Path
    _has_path: bool
    _is_task: bool
    _node: gnomonPipelineNode

    def __init__(self, node: gnomonPipelineNode, data_dir: str = ""):
        """
        Node wrapper which instantiate the corresponding plugin, parametrize them
        and provides a unified interface.

        Parameters
        ----------
        node: gnomonPipelineNode
        """
        self._node = node
        algo_name = node.name()
        algo_class = node.algorithmClass()
        self.name = algo_name
        self.data_dir = pathlib.Path(data_dir)
        self._has_path = False

        # making connections
        self.inputs_connections = {}
        for input_name in node.inputPortsNames():
            edge: gnomonPipelineEdge = node.inputEdgeFromPort(input_name)
            if edge:
                source: gnomonPipelinePort = edge.source()
                self.inputs_connections[input_name] = (source.node().name(), source.name())

        if algo_class == "task":
            self._is_task = True
            self._node = gnomonPipelineNodeTask._dynamic_cast(self._node)
            # no plugin instances, need local storage
            # generating input setters
            self.inputs = {}
            self._inputs_storage = {}
            def _setter(storage_dict, key, form):
                storage_dict[key] = form
            for input_name in node.inputPortsNames():
                self.inputs[input_name] = partial(_setter, self._inputs_storage, input_name)
            # generating output getters
            self.outputs = {}
            self._outputs_storage = {}
            def _getter(storage_dict, key):
                return storage_dict[key]
            for output_name in node.outputPortsNames():
                self.outputs[output_name] = partial(_getter, self._outputs_storage, output_name)
        else:
            self._is_task = False
            # instantiating algorithm
            if algo_class == "formAlgorithm":
                tmp = {}
                exec(node.getParameterAsString("python_code") + f"\nalgo = {node.algorithmPlugin()}()", tmp)  # Oh no D:
                self.algo = tmp["algo"]
            else:
                load_plugin_group(algo_class)
                factory = get_factory(algo_class)
                self.algo = factory().create(node.algorithmPlugin())

            if self.algo is None:
                raise RuntimeError(f"Could not instantiate plugin {node.algorithmPlugin()} from plugin group"
                                   f" {algo_class}. It might not be installed.")

            # generating input setters
            self.inputs = {}
            for input_name in node.inputPortsNames():
                input_method = "set{}{}".format(input_name[0].capitalize(), input_name[1:])
                self.inputs[input_name] = getattr(self.algo, input_method)
            # generating output getters
            self.outputs = {}
            for output_name in node.outputPortsNames():
                self.outputs[output_name] = getattr(self.algo, output_name)

            # if reader or writer, set default path
            if hasattr(self.algo, "setPath"):
                self._has_path = True
                self.algo.setPath(node.path())

            # setting parameters
            for param_name in node.parametersName():
                if param_name not in ("python_code",):
                    param = self.algo._parameters[param_name]
                    node.configureParameter(param_name, param)
                    self.algo.setParameter(param_name, param)

    def run(self):
        """
        Calls the `run()` method of this node plugin.
        """
        print(f" -- running {self.name}")
        if self._is_task:
            self._outputs_storage.update(self._node.runTask(self._inputs_storage))
        else:
            self.algo.run()

    def has_path(self) -> bool:
        """
        Returns True if this node has a path parameter.
        """
        return self._has_path

    def setPath(self, path: str):
        """
        Sets the path of this node if it has a path parameter (else do nothing).

        Parameters
        ----------
        path: str
            New path for the node.
        """
        if self.has_path():
            # double-checking if called from outside
            new_path = []
            for fp in path.split(","):
                fp = pathlib.Path(fp)
                if fp.is_absolute() or str(self.data_dir) in str(fp):
                    new_path.append(fp)
                else: 
                    new_path.append(self.data_dir.joinpath(fp))
            new_path = ",".join(map(str, new_path))
            self.algo.setPath(new_path)


class PipelineRunner:
    """
    Instantiate the plugins and runs the pipeline.

    Takes a gnomonPipeline object and instantiate the plugins of
    each node through the use of PNodeRunner.
    Use `run()` to execute the pipeline.

    Attributes
    ----------
    pipeline: gnomonPipeline
        Source gnomonPipeline object used to build the object.
    nodes: Dict[str, PNodeRunner]
        Dictionary mapping node_name --> PNodeRunner object
    path_dict: Dict[str, str]
        Dictionary mapping node_name --> path for each node having a path parameter
    data_dir: pathlib.Path, default=""
        Path to the directory containing the data. Used to resolve relative paths.

    """
    pipeline: gnomonPipeline
    nodes: Dict[str, PNodeRunner]
    path_dict: Dict[str, str]
    data_dir: pathlib.Path

    def __init__(self, pipeline: gnomonPipeline, data_dir: str = ""):
        self.pipeline = pipeline
        self.nodes = {}
        self.path_dict = {}
        self.data_dir = pathlib.Path(data_dir)
        for node_name in self.pipeline.nodeNames():
            print(f"making node {node_name}")
            node = self.pipeline.node(node_name)
            runner = PNodeRunner(node, data_dir)
            if runner.has_path():
                self.path_dict[node_name] = node.path()
            self.nodes[node_name] = runner

    def update_node_inputs(self, node_name: str):
        """
        Updates the inputs of the node from connected nodes.

        Parameters
        ----------
        node_name: str
            Name of the node updated.
        """
        node = self.nodes[node_name]
        for target_port, source in node.inputs_connections.items():
            source_node, source_port = source
            tmp = self.nodes[source_node].outputs[source_port]()
            node.inputs[target_port](tmp)

    def run(self):
        """
        Runs the pipeline.

        Works as follows :
         1. Gets the equirequirement groups from the `gnomonPipeline` object.
         2. Sets the path from `path_dict` for nodes having a path parameter.
         3. Run each source node in threads then await those threads.
         4. For each equirequirement node group:
                1. Update input of each node of the group
                2. Run each node of the group inside their respective threads.
                3. Await those threads
        """
        groups: List[List[str]] = self.pipeline.scheduleGroups()

        # setting path
        for node_name, path in self.path_dict.items():
            # checking path here in case data_dir is changed in PipelineRunner
            new_path = []
            for fp in path.split(","):
                fp = pathlib.Path(fp)
                new_path.append(fp if fp.is_absolute() else self.data_dir.joinpath(fp))
            new_path = ",".join(map(str, new_path))
            self.nodes[node_name].setPath(new_path)

        # getting sources
        sources = groups[0]
        print("computing group : ", sources)
        jobs = []
        for source_node in sources:
            if THREADING:
                job = Thread(target=self.nodes[source_node].run)
                job.start()
                jobs.append(job)
            else:
                self.nodes[source_node].run()

        for job in jobs:
            job.join()

        for node_group in groups[1:]:
            print("computing group : ", node_group)
            # schedule nodes
            for node_name in node_group:
                self.update_node_inputs(node_name)

            jobs = []
            for node_name in node_group:
                job = Thread(target=self.nodes[node_name].run)
                job.start()
                jobs.append(job)

            for job in jobs:
                job.join()

    def update_paths(self, path_dict: Dict[str, str]):
        """
        Updates the attribute `path_dict`.

        Parameters
        ----------
        path_dict: Dict[str, str]
            Dictionary mapping a node_name to a path it should use.
            Useful to configure reader or writer nodes.
        """
        self.path_dict.update(path_dict)


def load_pipeline(path: str, data_dir: str = ""):
    """
    Load a pipeline from path and returns a PipelineRunner object

    Parameters
    ----------
    path: str
        Path to pipeline file

    data_dir: str
        path to the data directory

    ios: Dict[str, str]
        dictionary of [input_output name : path] to apply the pipeline to new datas

    Returns
    ----------
    PipelineRunner object
    """

    pipeline = gnomonPipeline()
    ok = pipeline.readFromJson(path)
    if (not ok):
        print("cannot read pipeline from path " + path)
        return None
    else:
        return PipelineRunner(pipeline, data_dir=data_dir)


def run_pipeline(path: str, data_dir: str = "", ios: Dict[str, str]= None):
    """
    run a pipeline from a path
    Parameters
    ----------
    path: str
        Path to pipeline file

    data_dir: str
        path to the data directory

    ios: Dict[str, str]
        dictionary of [input_output name : path] to apply the pipeline to new datas

    """

    pipeline_runner = load_pipeline(path, data_dir)
    if not pipeline_runner:
        return

    if ios:
        print("has ios")
        print(ios)
        nodes = ios[::2]
        paths = ios[1::2]
        for node_name, path in zip(nodes, paths):
            pipeline_runner.path_dict[node_name] = path
    pipeline_runner.run()


def install_missing_package():
    pass
