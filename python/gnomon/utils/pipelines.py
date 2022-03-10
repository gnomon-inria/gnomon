from typing import Tuple, List, Dict, Callable

from gnomon.utils.gnomonPlugin import load_plugin_group, get_factory
from gnomon.pipeline import gnomonPipeline, gnomonPipelineNode, gnomonPipelineEdge, gnomonPipelinePort
from gnomon.core import gnomonAbstractDynamicForm, gnomonAbstractAlgorithm


class PNodeRunner:
    name: str
    algo: gnomonAbstractAlgorithm
    inputs_connections: Dict[str, Tuple[str, str]]
    inputs: Dict[str, Callable[[gnomonAbstractDynamicForm], None]] = {}
    outputs: Dict[str, Callable[[], gnomonAbstractDynamicForm]] = {}

    def __init__(self, node: gnomonPipelineNode):
        self.name = node.name()
        load_plugin_group(node.algorithmClass())
        factory = get_factory(node.algorithmClass())
        self.algo = factory().create(node.algorithmPlugin())

        # generating input setters
        for input_name in node.inputPortsNames():
            self.inputs[input_name] = lambda form: getattr(self.algo, input_name)(form)
        # generating output getters
        for output_name in node.outputPortsNames():
            self.outputs[output_name] = lambda: getattr(self.algo, output_name)()

        # making connections
        for input_name in node.inputPortsNames():
            edge: gnomonPipelineEdge = node.inputEdgeFromPort(input_name)
            if edge:
                source: gnomonPipelinePort = edge.source()
                self.inputs_connections[input_name] = (source.node().name(), source.name())

    def run(self):
        self.algo.run()

    def iter_connections(self):
        for target_port, edge in self.inputs_connections.items():
            yield edge.source(),


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
            node.inputs[target_port](self.nodes[source_node].outputs[source_port]())

    def run(self):
        groups: List[List[str]] = self.pipeline.scheduleGroups()

        # getting sources
        sources = groups[0]
        for source_node in sources:
            self.nodes[source_node].run()

        for node_group in groups[1:-1]:
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
    return pipeline
    print("making runner")
    return PipelineRunner(pipeline)
