from A4.src.Domain.graph import GraphException, Graph
from A4.src.Service.directed_graph import DirectedGraph
from A4.src.read_file import read_graph_from_file
from A4.src.write_file import write_graph_to_file
from A4.src.generate_graph import generate_random_graph
from A4.src.write_filr2 import write_graph_to_file2
import copy
from A4.src.DAGGraph.DAG import DAG


class Console:
    def __init__(self, service, graph, validation, dag_graph):
        self._graph = graph
        self._service = service
        self._validation = validation
        self._dag_graph = dag_graph

    def display_number_of_vertices(self):
        print("The number of vertices is:", len(self._graph.vertices))

    def display_the_vertices(self):
        if len(self._graph.vertices) == 0:
            print("There doesn't exist a graph!")
        else:
            print("The vertices are:")
            vertices = self._graph.parse_vertices()
            for vertex in vertices:
                print(vertex)

    def display_inbound_vertices(self):
        vertex = int(input("Type in the vertex: "))
        if len(self._service.inbounds) == 0:
            print("There doesn't exist a graph!")
        elif self._graph.check_existence_of_vertex(vertex):
            vertices = self._service.parse_inbound_vertices(vertex)
            for vertex in vertices:
                print(vertex)
        else:
            print("The given vertex doesn't exist!")

    def change_cost_edge(self):
        if len(self._graph.vertices) == 0:
            print("There doesn't exist a graph!")
        else:
            first_vertex = int(input("Type in the first vertex: "))
            second_vertex = int(input("Type in the second vertex: "))
            if first_vertex == second_vertex:
                raise GraphException("Invalid input!")
            self._validation.exist_vertex(first_vertex, second_vertex)
            if self._service.check_existence_edge(first_vertex, second_vertex) is False:
                print("There doesn't exists and edge from", first_vertex, "to", second_vertex)
            else:
                cost = int(input("Type in the new cost: "))
                self._service.change_cost_edge(first_vertex, second_vertex, cost)
                print("The cost has been changed successfully")

    def display_outbound_vertices(self):
        vertex = int(input("Type in the vertex: "))
        if len(self._service.outbounds) == 0:
            print("There doesn't exist a graph!")
        elif self._graph.check_existence_of_vertex(vertex):
            vertices = self._service.parse_outbound_vertices(vertex)
            for vertex in vertices:
                print(vertex)
        else:
            print("The given vertex doesn't exist!")

    def get_in_degree(self):
        vertex = int(input("Type in the vertex: "))
        if self._graph.check_existence_of_vertex(vertex) is True:
            print(self._service.get_in_degree(vertex))
        else:
            print("The given vertex doesn't exist!")

    def get_out_degree(self):
        vertex = int(input("Type in the vertex: "))
        if self._graph.check_existence_of_vertex(vertex) is True:
            print(self._service.get_out_degree(vertex))
        else:
            print("The given vertex doesn't exist!")

    def edge_between_vertices(self):
        if len(self._graph.vertices) == 0:
            print("There doesn't exist a graph!")
        else:
            first_vertex = int(input("Type in the first vertex: "))
            second_vertex = int(input("Type in the second vertex: "))
            if first_vertex == second_vertex:
                raise GraphException("Invalid input!")
            self._validation.exist_vertex(first_vertex, second_vertex)
            if self._service.check_existence_edge(first_vertex, second_vertex):
                print("There exists an edge from", first_vertex, "to", second_vertex)
            else:
                print("There doesn't exists and edge from", first_vertex, "to", second_vertex)

    def add_vertex(self):
        print("Type in the vertex you would you like to add: ")
        new_vertex = int(input())
        self._service.add_vertex(new_vertex)
        print("The vertex was added to the graph!")

    def remove_vertex(self):
        print("Type in the vertex you would like to remove: ")
        vertex = int(input())
        self._service.remove_vertex(vertex)
        print("The vertex was deleted from the graph!")

    def add_an_edge(self):
        print("Type in the first vertex: ")
        first_vertex = int(input())
        print("Type in the second vertex: ")
        second_vertex = int(input())
        if first_vertex == second_vertex:
            raise GraphException("Invalid input!")
        print("Type in the cost of the edge: ")
        cost = int(input())
        self._service.add_edge(first_vertex, second_vertex, cost)
        print("The edge was added to the graph with success!!!")

    def delete_an_edge(self):
        print("Type in the first vertex: ")
        first_vertex = int(input())
        print("Type in the second vertex: ")
        second_vertex = int(input())
        if first_vertex == second_vertex:
            raise GraphException("Invalid input!")
        self._service.delete_edge(first_vertex, second_vertex)
        print("The edge was deleted with success!")

    def copy_graph(self):
        self._service.copy_graph()
        print("The graph was copied with success!")

    def read_graph(self):
        file_name = input("Type in the file name: ")
        read_graph_from_file(file_name, self._graph, self._service)
        print("The command executed with success!")

    def display_graph(self):
        file_name = input("Type in the file name: ")
        criteria = int(input("Type 1 for displaying the graph without isolated points or 2 for displaying the graph "
                         "with isolated points"))
        if criteria != 1 and criteria != 2:
            raise GraphException("Invalid input!")
        if criteria == 1:
            write_graph_to_file(file_name, self._graph, self._service)
        elif criteria == 2:
            write_graph_to_file2(file_name, self._graph, self._service)
        print("The command executed with success!")

    def generate_a_random_graph(self):
        print("Type in the number of vertices: ")
        number_of_vertices = int(input())
        print("Type in the number of edges: ")
        number_of_edges = int(input())
        generate_random_graph(self._graph, self._service, number_of_vertices, number_of_edges)
        print("The graph was generated with success!!!")

    def get_cost(self):
        first_vertex = int(input("Type in the first vertex: "))
        second_vertex = int(input("Type in the second vertex: "))
        if self._graph.check_existence_of_vertex(first_vertex) and self._graph.check_existence_of_vertex(second_vertex):
            if self._service.check_existence_edge(first_vertex, second_vertex):
                cost = self._service.get_cost_for_edge(first_vertex, second_vertex)
                print(cost)
            else:
                print("There is no edge between these two vertices!")
        else:
            print("The given vertices doesn't exist!")

    def lowest_length_path(self):
        """
        With this function we compute the lowest length path between two vertices
        :return:
        """
        first_vertex = int(input("Type in the first vertex: "))
        second_vertex = int(input("Type in the second vertex: "))
        if self._graph.check_existence_of_vertex(first_vertex) and self._graph.check_existence_of_vertex(second_vertex):
            dist, predecessors = self._service.find_lowest_length_path(first_vertex, second_vertex)
        else:
            raise GraphException("The given vertices doesn't exist!\n")

        if dist == 0:
            raise GraphException("There doesn't exist a path from the first vertex to the other one\n")

        path = []
        backward = second_vertex
        path.append(backward)

        # Here we reproduce the path in reverse order
        while predecessors[backward] != -1:
            path.append(predecessors[backward])
            backward = predecessors[backward]

        print("Shortest path length is: " + str(dist))

        print("\nPath is:")

        # Because the path was reproduced in reverse order, we have to print it from the "end" to the "start"
        for i in range(len(path) - 1, -1, -1):
            print(path[i], end=' ')

    def minimum_cost_walk(self):
        print("\n")
        first_vertex = int(input("Type in the first vertex: "))
        second_vertex = int(input("Type in the second vertex: "))
        if self._graph.check_existence_of_vertex(first_vertex) and self._graph.check_existence_of_vertex(second_vertex):
            predecessors, dist = self._service.find_minimum_cost_walk(first_vertex, second_vertex)
        else:
            raise GraphException("The given vertices doesn't exist!\n")

        if dist == float("inf"):
            raise GraphException("There doesn't exist a walk from the first vertex to the other one\n")

        path=[]
        backward = second_vertex
        path.append(backward)

        # Here we reproduce the path in reverse order
        while predecessors[backward] != -1:
            path.append(predecessors[backward])
            backward = predecessors[backward]

        print("Minimum cost walk is: " + str(dist))

        print("Path is:")

        # Because the path was reproduced in reverse order, we have to print it from the "end" to the "start"
        for i in range(len(path) - 1, -1, -1):
            print(path[i], end=' ')

    def dag_functionality(self):
        # self._dag_graph.init_durations()
        # self._dag_graph.init_prerequisites()
        self._dag_graph.read_dag_from_file("DAG.txt")
        self._dag_graph.create_corresponding_graph()

        sorted_list = self._dag_graph.top_sort_predecessor_counting()
        if sorted_list is None:
            print('The graph is not a DAG!')
        else:
            print('The graph is a DAG and the topological sorting is:')
            print(sorted_list)
        print('\n')

        print('The earliest scheduling:')
        earliest_scheduling = self._dag_graph.earliest_scheduling(sorted_list)
        project_duration = 0
        for vertex in sorted_list:
            for find in earliest_scheduling:
                if vertex == find:
                    if earliest_scheduling[vertex][1] > project_duration:
                        project_duration = earliest_scheduling[vertex][1]
                    print(str(vertex) + ': ' + str(earliest_scheduling[vertex][0]) + ' ' + str(
                        earliest_scheduling[vertex][1]))
        print('\n')

        print('The latest scheduling:')
        latest_scheduling = self._dag_graph.latest_scheduling(sorted_list, project_duration)
        for vertex in sorted_list:
            for find in latest_scheduling:
                if vertex == find:
                    print(
                        str(vertex) + ': ' + str(latest_scheduling[vertex][0]) + ' ' + str(latest_scheduling[vertex][1]))
        print('\n')

        print('\nThe total time of the project is ' + str(project_duration))

        print('\nThe critical activities are:')

        for vertex in sorted_list:
            for find in earliest_scheduling:
                if vertex == find:
                    if earliest_scheduling[vertex][0] == latest_scheduling[vertex][0] and earliest_scheduling[vertex][1] == \
                            latest_scheduling[vertex][1]:
                        print(vertex)

        print("The corresponding graph for the project is: ")
        for first_vertex in self._service.outbounds:
            vertices = self._service.get_outbound_vertices(first_vertex)
            second_vertices = self._service.get_inbound_vertices(first_vertex)
            if len(vertices) == 0 and len(second_vertices) == 0:
                print(first_vertex)
            else:
                for second_vertex in vertices:
                    final_key = str(first_vertex) + '-' + str(second_vertex)
                    params = str(first_vertex) + ' ' + str(second_vertex) + ' ' + str(self._service.cost[final_key])
                    print(params)
        # self._service.draw_graph()

    def display_menu(self):
        print("1. Display the number of vertices")
        print("2. Display the set of vertices")
        print("3. Display if there exists an edge from a vertex to another one")
        print("4. Display the in degree of a vertex")
        print("5. Display the out degree of a vertex")
        print("6. Display the set of outbound edges of a specified vertex")
        print("7. Display the set of inbound edges of a specified vertex")
        print("8. Change the cost of a specified edge")
        print("9. Add an edge")
        print("10. Remove an edge")
        print("11. Add a vertex")
        print("12. Remove a vertex")
        print("13. Make a copy of the graph")
        print("14. Read the graph from the file")
        print("15. Write the graph into a file")
        print("16. Generate a random graph")
        print("17. Get the cost of a specified edge")
        print("18. Display the lowest length path between two vertices")
        print("19. Display the minimum cost walk between two vertices")
        print("20. DAG Functionality!")

    def start(self):

        print('\nMENU\n')
        self.display_menu()
        are_we_done_yet = False
        command_dict = {'0': self.display_menu, '1': self.display_number_of_vertices, '2': self.display_the_vertices,
                        '3': self.edge_between_vertices, '4': self.get_in_degree, '5': self.get_out_degree,
                        '6': self.display_outbound_vertices, '7': self.display_inbound_vertices,
                        '8': self.change_cost_edge, '9': self.add_an_edge, '10': self.delete_an_edge,
                        '11': self.add_vertex, '12': self.remove_vertex, '13': self.copy_graph, '14': self.read_graph,
                        '15': self.display_graph, '16': self.generate_a_random_graph, '17': self.get_cost,
                        '18': self.lowest_length_path, '19': self.minimum_cost_walk, '20': self.dag_functionality}
        while not are_we_done_yet:
            command = input('\nWhat would you like to do? Enter command: \n')
            if command in command_dict:
                try:
                    command_dict[command]()
                except ValueError as ve:
                    print(ve)
                except GraphException as ge:
                    print(ge)
            elif command == '0':
                self.print_menu()
            elif command == 'x':
                print("Goodbye!")
                are_we_done_yet = True
            else:
                print("Invalid command")
