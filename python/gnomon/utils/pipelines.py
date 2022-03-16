from typing import Tuple, List, Dict, Callable

from gnomon.utils.gnomonPlugin import load_plugin_group, get_factory
from gnomon.pipeline import gnomonPipeline, gnomonPipelineNode, gnomonPipelineEdge, gnomonPipelinePort
from gnomon.core import gnomonAbstractDynamicForm, gnomonAbstractAlgorithm


class PNodeRunner:
    name: str
    algo: gnomonAbstractAlgorithm
    inputs_connections: Dict[str, Tuple[str, str]]
    inputs: Dict[str, Callable[[gnomonAbstractDynamicForm], None]]
    outputs: Dict[str, Callable[[], gnomonAbstractDynamicForm]]
    _node: gnomonPipelineNode

    def __init__(self, node: gnomonPipelineNode):
        self._node = node
        algo_name = node.name()
        algo_class = node.algorithmClass()
        self.name = algo_name
        load_plugin_group(algo_class)
        factory = get_factory(algo_class)
        self.algo = factory().create(node.algorithmPlugin())

        # generating input setters
        self.inputs = {}
        for input_name in node.inputPortsNames():
            input_method = "set{}{}".format(input_name[0].capitalize(), input_name[1:])
            self.inputs[input_name] = getattr(self.algo, input_method)
        # generating output getters
        self.outputs = {}
        for output_name in node.outputPortsNames():
            self.outputs[output_name] = getattr(self.algo, output_name)

        # making connections
        self.inputs_connections = {}
        for input_name in node.inputPortsNames():
            edge: gnomonPipelineEdge = node.inputEdgeFromPort(input_name)
            if edge:
                source: gnomonPipelinePort = edge.source()
                self.inputs_connections[input_name] = (source.node().name(), source.name())

        # if reader or writer, set default path
        if hasattr(self.algo, "setPath"):
            #print(f"path for {self.name} -> {node.path()}")
            self.algo.setPath(node.path())

        # setting parameters
        for param_name in node.parametersName():
            param = self.algo._parameters[param_name]
            #print(type(param), param)
            node.configureParameter(param_name, param)
            self.algo.setParameter(param_name, param)
            #print(" ----> ", type(self.algo._parameters[param_name]), self.algo._parameters[param_name])

    def run(self):
        print(f" -- running {self.name}")
        self.algo.run()


class PipelineRunner:
    pipeline: gnomonPipeline
    nodes: Dict[str, PNodeRunner]

    def __init__(self, pipeline: gnomonPipeline):
        self.pipeline = pipeline
        self.nodes = {}
        for node_name in self.pipeline.nodeNames():
            print(f"making node {node_name}")
            self.nodes[node_name] = PNodeRunner(self.pipeline.node(node_name))

    def update_node_inputs(self, node_name: str):
        node = self.nodes[node_name]
        for target_port, source in node.inputs_connections.items():
            source_node, source_port = source
            tmp = self.nodes[source_node].outputs[source_port]()
            # print(tmp)
            node.inputs[target_port](tmp)

    def run(self):
        groups: List[List[str]] = self.pipeline.scheduleGroups()

        # getting sources
        sources = groups[0]
        print("computing group : ", sources)
        for source_node in sources:
            self.nodes[source_node].run()

        for node_group in groups[1:]:
            print("computing group : ", node_group)
            # schedule nodes
            for node_name in node_group:
                self.update_node_inputs(node_name)

            for node_name in node_group:
                self.nodes[node_name].run()


def load_pipeline(path: str):
    print("instantiating")
    pipeline = gnomonPipeline()
    print("reading")
    pipeline.readFromJson(path)
    print("making runner")
    return PipelineRunner(pipeline)
