import copy
import queue
from re import search
import collections

from A4.src.Domain.graph import GraphException, Graph


class DirectedGraph:

    def __init__(self, graph):
        self._graph = graph
        self._inbounds = {}
        self._outbounds = {}
        self._cost = {}
        self._new_cost = {}
        # self._edges = 0
        # self.initialize_vertices_in()
        # self.initialize_vertices_out()

    # def initialize_edges(self, edges):
    #     self._edges = edges
    #     self._graph.initialize_edges(edges)

    # def draw_graph(self):
    #     graph = graphviz.Graph()
    #     for vertex in self._inbounds:
    #         if len(self._inbounds[vertex]) == 0:
    #             graph.node(str(vertex))
    #
    #     for vertex in self.parse_vertices():
    #         for second_vertex in self._inbounds[vertex]:
    #             graph.edges(str(vertex), str(second_vertex))
    #
    #     for key in self._cost.keys():
    #         vertices = key.split('-')
    #         first_vertex = int(vertices[0])
    #         second_vertex = int(vertices[1])
    #         self._new_cost[(first_vertex, second_vertex)] = self._cost[key]
    #         # if first_vertex <= second_vertex:
    #         #     graph.edges(str(first_vertex), str(second_vertex))
    #     graph.edges((str(x), str(y)) for x, y in self._new_cost.keys() if x <= y)
    #     graph.view()

    @property
    def inbounds(self):
        return self._inbounds

    @inbounds.setter
    def inbounds(self, key, value):
        self._inbounds[key] = value

    @property
    def edges(self):
        return self._graph.edges

    @edges.setter
    def edges(self, value):
        self._graph.edges = value

    @property
    def outbounds(self):
        return self._outbounds

    @outbounds.setter
    def outbounds(self, key, value):
        self._outbounds[key] = value

    @property
    def cost(self):
        return self._cost

    @cost.setter
    def cost(self, key, value):
        self._cost[key] = value

    def parse_vertices(self):
        return self._graph.parse_vertices()

    def copy_graph(self):
        return copy.deepcopy(self._graph), copy.deepcopy(self)

    def initialize_graph(self, number_of_vertices):
        self._inbounds.clear()
        self._outbounds.clear()
        self._cost.clear()
        self._graph.initialize_vertices(number_of_vertices)
        self.initialize_costs()
        self.initialize_vertices_in()
        self.initialize_vertices_out()

    def initialize_costs(self):
        for i in range(0, self._graph.edges):
            self._cost[i] = []

    def initialize_vertices_in(self):
        for i in range(len(self._graph.vertices)):
            self._inbounds[i] = []

    def initialize_vertices_out(self):
        for i in range(len(self._graph.vertices)):
            self._outbounds[i] = []

    def get_inbound_vertices(self, vertex):
        return self._inbounds[vertex]

    def get_outbound_vertices(self, vertex):
        return self._outbounds[vertex]

    def add_vertex(self, new_vertex):
        self._graph.add_vertex(new_vertex)
        self._inbounds[new_vertex] = []
        self._outbounds[new_vertex] = []

    def remove_vertex(self, vertex):
        self._graph.remove_vertex(vertex)
        for existent_vertex in self._inbounds:
            if vertex in self._inbounds[existent_vertex]:
                self._inbounds[existent_vertex].remove(vertex)
            elif vertex in self._outbounds[existent_vertex]:
                self._outbounds[existent_vertex].remove(vertex)
        del self._inbounds[vertex]
        del self._outbounds[vertex]

    def add_edge(self, first_vertex, second_vertex, cost):
        if self._graph.check_existence_of_vertex(first_vertex) and self._graph.check_existence_of_vertex(second_vertex):
            if second_vertex in self._outbounds[first_vertex]:
                raise GraphException('The given edge already exists!')
            else:
                self._inbounds[second_vertex].append(first_vertex)
                self._outbounds[first_vertex].append(second_vertex)
                final_key = str(first_vertex) + '-' + str(second_vertex)
                self._cost[final_key] = cost
                self._graph.edges = self._graph.edges + 1
        else:
            raise GraphException('One of the given vertices does not exist! Try again!!!')

    def delete_edge(self, first_vertex, second_vertex):
        if self._graph.check_existence_of_vertex(first_vertex) and self._graph.check_existence_of_vertex(second_vertex):
            if second_vertex not in self._outbounds[first_vertex]:
                raise GraphException('The given edge does not exists!')
            else:
                self._inbounds[second_vertex].remove(first_vertex)
                self._outbounds[first_vertex].remove(second_vertex)
                final_key = str(first_vertex) + '-' + str(second_vertex)
                del self._cost[final_key]
                self._graph.edges = self._graph.edges - 1

    def add_cost(self, first_edge, second_edge, cost):
        final_key = str(first_edge) + '-' + str(second_edge)
        self._cost[final_key] = cost

    def add_vertex_to_inbounds(self, first_vertex, second_vertex):
        self._inbounds[second_vertex].append(first_vertex)

    def add_vertex_to_outbounds(self, first_vertex, second_vertex):
        self._outbounds[first_vertex].append(second_vertex)

    def parse_outbound_vertices(self, vertex):
        vertices = self.get_outbound_vertices(vertex)
        for vertex in vertices:
            yield vertex

    def parse_inbound_vertices(self, vertex):
        vertices = self.get_inbound_vertices(vertex)
        for vertex in vertices:
            yield vertex

    def get_in_degree(self, vertex):
        inbound_vertices = self.get_inbound_vertices(vertex)
        return len(inbound_vertices)

    def get_cost(self):
        for key in self._cost.keys():
            yield key

    def get_out_degree(self, vertex):
        outbound_vertices = self.get_outbound_vertices(vertex)
        return len(outbound_vertices)

    def check_existence_edge(self, first_edge, second_edge):
        outbound_vertices = self.get_outbound_vertices(first_edge)
        for vertex in outbound_vertices:
            if vertex == second_edge:
                return True
        return False

    def change_cost(self, key, cost):
        self._cost[key] = cost

    def change_cost_edge(self, first_vertex, second_vertex, cost):
        final_key = str(first_vertex) + '-' + str(second_vertex)
        keys = self.get_cost()
        for key in keys:
            if key == final_key:
                self.change_cost(key, cost)

    def get_cost_for_edge(self, first_vertex, second_vertex):
        final_key = str(first_vertex) + '-' + str(second_vertex)
        return self._cost[final_key]

    def find_minimum_cost_walk(self, first_vertex, second_vertex):
        inf = float('inf')
        distance = {}
        for vertex in self._graph.vertices:
            distance[vertex] = inf

        distance[first_vertex] = 0
        # With this dictionary we compute the minimum cost between the vertices

        predecessor = {}
        # With this dictionary we compute the walk between the vertices
        for vertex in self._graph.vertices:
            predecessor[vertex] = -1

        changed = True
        index = 0
        while changed and index < len(self._graph.vertices):
            changed = False
            for x in self._graph.vertices:
                for y in self._outbounds[x]:
                    edge_weight = str(x) + '-' + str(y)
                    if distance[y] > distance[x] + self._cost[edge_weight] and distance[x] != float('inf'):
                        distance[y] = distance[x] + self._cost[edge_weight]
                        predecessor[y] = x
                        changed = True
            index = index + 1

        for x in self._graph.vertices:
            for y in self._outbounds[x]:
                edge_weight = str(x) + '-' + str(y)
                if distance[y] > distance[x] + self._cost[edge_weight] and distance[x] != float('inf'):
                    raise GraphException("Negative weight cycle exist in the graph")

        return predecessor, distance[second_vertex]

    def find_lowest_length_path(self, first_vertex, second_vertex):
        """
        With this function we compute the lowest length path between two vertices
        :param first_vertex: the vertex from which we start the search
        :param second_vertex: the vertex we want to reach
        :return: it returns the distance from the two vertices and the dictionary of predecessors
        """

        visited = [0] * len(self._graph.vertices)
        # With this list we mark if the vertex was visited or not

        distance = {}
        for i in range(len(self._graph.vertices)):
            distance[i] = 0
        # With this dictionary we compute the distance between the vertices

        predecessors = {}
        for i in range(len(self._graph.vertices)):
            predecessors[i] = -1
        # With this dictionary we compute the path between the vertices

        Q = queue.Queue()
        Q.put(first_vertex)

        visited[first_vertex] = True
        done = False
        while not Q.empty() and not done:
            x = Q.get()
            for i in range(len(self._outbounds[x])):
                y = self._outbounds[x][i]
                if visited[y] == 1:
                    continue

                predecessors[y] = x
                distance[y] = distance[x] + 1
                Q.put(y)
                visited[y] = 1
                if second_vertex == y:
                    done = True
        return distance[second_vertex], predecessors














        # for vertex in self._graph.vertices:
        #     print("Source Node(" + str(first_vertex) + ") -> Destination Node(" + str(vertex) + ") : " +
        #           str(distance[vertex]))


        # prev = {}
        # dist = {}
        # visited = [vertex]
        # queue = []
        # dist[vertex] = 0
        # queue.append(vertex)
        # while queue:
        #     x = queue.pop()
        #     for y in self._outbounds[x]:
        #         if y not in visited:
        #             queue.append(y)
        #             visited.append(y)
        #             dist[y] = dist[x] + 1
        #             prev[y] = x
        # accessible = visited
        # return accessible, prev

        # parent = {vertex: None}
        # distance = {vertex: 0}
        #
        # visited = set()
        # q = queue.Queue()
        # q.put(vertex)
        # visited.add(vertex)
        # while not q.empty():
        #     current = q.get()
        #     for dest in self._outbounds[current]:
        #         if dest not in visited:
        #             visited.add(dest)
        #             parent[dest] = current
        #             distance[dest] = distance[current] + 1
        #             q.put(dest)
        #
        # return (parent, distance)



    # def convert_graph(self):
    # #     total_number_of_edges = len(self._outbounds) + len(self._inbounds)
    # #     graph = [[0, 0, 0] for index in range(total_number_of_edges)]
    #     graph = []
    #     for vertex in self._graph.vertices:
    #         for y in self._outbounds[vertex]:
    #             weight = str(vertex) + '-' + str(y)
    #             graph[vertex][y] = self._cost[weight]
    #
    #     for vertex in self._graph.vertices:
    #         for y in self._inbounds[vertex]:
    #             weight = str(vertex) + '-' + str(y)
    #             graph[y][vertex] = self._cost[weight]
    #
    #     return graph


    # def isNegCycleBellmanFord(self, src):
    #     graph = self.convert_graph()
    #     V = len(self._graph.vertices)
    #     E = len(graph)
    #
    #     dist = []
    #     # Step 1: Initialize distances from src
    #     # to all other vertices as INFINITE
    #     for i in range(V):
    #         dist[i] = 10 ** 18
    #     dist[src] = 0
    #
    #     # Step 2: Relax all edges |V| - 1 times.
    #     # A simple shortest path from src to any
    #     # other vertex can have at-most |V| - 1
    #     # edges
    #     for i in range(1, V):
    #         for j in range(E):
    #             u = graph[j][0]
    #             v = graph[j][1]
    #             weight = graph[j][2]
    #             if (dist[u] != 10 ** 18 and dist[u] + weight < dist[v]):
    #                 dist[v] = dist[u] + weight
    #
    #     # Step 3: check for negative-weight cycles.
    #     # The above step guarantees shortest distances
    #     # if graph doesn't contain negative weight cycle.
    #     # If we get a shorter path, then there
    #     # is a cycle.
    #     for i in range(E):
    #         u = graph[i][0]
    #         v = graph[i][1]
    #         weight = graph[i][2]
    #         if (dist[u] != 10 ** 18 and dist[u] + weight < dist[v]):
    #             return True
    #
    #     return False


    # def check_for_isolated_vertex(self, vertex):
    #     first_found = False
    #     second_found = False
    #     for key in self._inbounds.keys():
    #         if vertex == key:
    #             first_found = True
    #
    #     for key in self._outbounds.keys():
    #         if vertex == key:
    #             second_found = True
    #
    #     if first_found and second_found:
    #         return True
    #     elif first_found and not second_found:
    #         return True
    #     elif not first_found and second_found:
    #         return True
    #     else:
    #         return False

    # def remove_vertex(self, vertex):
    #     self._graph.remove_vertex(vertex)
    #
    #     for to_find_vertex in self._inbounds:
    #         if to_find_vertex == vertex:
    #             self._inbounds[to_find_vertex].remove(vertex)
    #
    #     del self._outbounds[vertex]
    #
    # def remove_edges_with_cost(self, vertex):
    #     string_vertex = str(vertex)
    #     for key in self._cost.keys():
    #         if search(string_vertex, key):
    #             del self._cost[key]