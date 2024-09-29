from A4.src.Domain.graph import Graph
from A4.src.Service.directed_graph import DirectedGraph
from A4.src.Console.main import Console
from A4.src.Validation.validation import Validation
from A4.src.read_file import read_graph_from_file
from A4.src.write_file import write_graph_to_file
from A4.src.DAGGraph.DAG import DAG

graph = Graph()
service = DirectedGraph(graph)
validation = Validation(graph, service)
dag_graph = DAG(service)
console = Console(service, graph, validation, dag_graph)
console.start()

