import copy

from A4.src.Service.directed_graph import DirectedGraph
from A4.src.Domain.graph import Graph


class DAG:
    def __init__(self, directed_graph):
        self.directed_graph = directed_graph
        self._durations = {}
        self._prerequisites = {}

    # def init_durations(self):
    #     for vertex in range(0, 7):
    #         self._durations[vertex] = []
    #         self.directed_graph.add_vertex(vertex)
    #
    #     self._durations[0].append(1)
    #     self._durations[5].append(2)
    #     self._durations[6].append(5)
    #     self._durations[4].append(1)
    #     self._durations[1].append(2)
    #     self._durations[3].append(2)
    #     self._durations[2].append(1)
    #
    # def init_prerequisites(self):
    #     for vertex in range(0, 10):
    #         self._prerequisites[vertex] = []
    #
    #     self._prerequisites[0] = float('inf')  # no prerequisites
    #     self._prerequisites[5] = float('inf')  # no prerequisites
    #     self._prerequisites[6].append(0)
    #     self._prerequisites[6].append(5)
    #     self._prerequisites[4].append(5)
    #     self._prerequisites[1].append(6)
    #     self._prerequisites[3].append(4)
    #     self._prerequisites[3].append(6)
    #     self._prerequisites[2].append(3)
    #     self._prerequisites[2].append(6)

    def initialize_dag(self, number_of_activities):
        for vertex in range(0, int(number_of_activities)):
            self._durations[vertex] = []
            self._prerequisites[vertex] = []

    def initialize_durations(self, vertex, duration):
        self.directed_graph.add_vertex(vertex)
        self._durations[vertex].append(duration)

    def initialize_prerequisites(self, vertex, prerequisite):
        if prerequisite == '-':
            self._prerequisites[vertex] = float('inf')
        else:
            self._prerequisites[vertex].append(prerequisite)

    def create_corresponding_graph(self):
        # In this function the graph corresponding to the table is created
        self.directed_graph.edges = 0
        number_of_vertices = len(self._durations)
        self.directed_graph.initialize_graph(number_of_vertices)
        for vertex in range(0, number_of_vertices):
            if self._prerequisites[vertex] != float('inf'):
                for prerequisite in self._prerequisites[vertex]:
                    self.directed_graph.add_edge(prerequisite, vertex, 0)

    def top_sort_predecessor_counting(self):
        # In this function we do the topological sort
        sorted_ = []
        number_of_vertices = len(self._durations)
        q = []  # queue
        count = {}
        for vertex in self.directed_graph.parse_vertices():
            count[vertex] = self.directed_graph.get_in_degree(vertex)
            if count[vertex] == 0:
                q.append(vertex)
        while len(q) != 0:
            x = q.pop(0)
            sorted_.append(x)
            for y in self.directed_graph.parse_outbound_vertices(x):
                count[y] = count[y] - 1
                if count[y] == 0:
                    q.append(y)
        if len(sorted_) < number_of_vertices:
            sorted_ = None
        return sorted_

    def earliest_scheduling(self, sorted_list):
        # For the earliest scheduling we compute the scheduling of all the prerequisites and the start time is the
        # max between the ending times of the prerequisites.
        # start(x) = max(end(y)) : x depends on y (0 if no prerequisites)
        # end(x) = start(x) + duration(x)
        auxiliary_durations = copy.deepcopy(self._durations)
        for x in sorted_list:
            if self._prerequisites[x] == float('inf'):
                auxiliary_durations[x] = []
                duration = self._durations[x][0]
                auxiliary_durations[x].append(0)  # start
                auxiliary_durations[x].append(auxiliary_durations[x][0] + duration)  # end
            else:
                maximum_end = 0
                for prerequisites in self._prerequisites[x]:
                    if auxiliary_durations[prerequisites][1] > maximum_end:
                        maximum_end = auxiliary_durations[prerequisites][1]
                auxiliary_durations[x] = []
                duration = self._durations[x][0]
                auxiliary_durations[x].append(maximum_end)  # start
                auxiliary_durations[x].append(auxiliary_durations[x][0] + duration)  # end
        return auxiliary_durations

    def latest_scheduling(self, sorted_list, project_duration):
        # For the latest scheduling we start from right (end of project) and move towards left.
        # end(x) = min(start(y)) : y depends on x (project duration if nothing depends on x)
        # start(x) = end(x) - duration(x)
        auxiliary_durations = copy.deepcopy(self._durations)
        for vertex in reversed(sorted_list):
            sem = 1
            for key in self._prerequisites.keys():
                if self._prerequisites[key] != float('inf'):
                    for val in self._prerequisites[key]:
                        if vertex == val:
                            sem = 0
            if sem == 1:
                auxiliary_durations[vertex] = []
                duration = self._durations[vertex][0]
                auxiliary_durations[vertex].append(project_duration - duration)  # start
                auxiliary_durations[vertex].append(project_duration)  # end
            else:
                maximum_start = project_duration + 1
                for key in reversed(sorted_list):
                    if self._prerequisites[key] != float('inf'):
                        for val in self._prerequisites[key]:
                            if vertex == val:
                                if auxiliary_durations[key][0] < maximum_start:
                                    maximum_start = auxiliary_durations[key][0]
                auxiliary_durations[vertex] = []
                duration = self._durations[vertex][0]
                auxiliary_durations[vertex].append(maximum_start - duration)  # start
                auxiliary_durations[vertex].append(maximum_start)  # end
        return auxiliary_durations

    def read_dag_from_file(self, file_name):
        # With this function we read the table from the text file
        try:
            file = open(file_name, 'rt')
            lines = file.readlines()
            first_line = False
            for line in lines:
                line = line.strip('\n')
                if first_line is False:
                    graph_parameters = line.split(' ')
                    self.initialize_dag(int(graph_parameters[0]))
                    first_line = True
                elif line == '':
                    continue
                else:
                    graph_parameters = line.split(' ')
                    self.initialize_durations(int(graph_parameters[0]), int(graph_parameters[1]))
                    prerequisites = graph_parameters[2].split(',')
                    for i in range(0, len(prerequisites)):
                        if prerequisites[i] == '-':
                            self.initialize_prerequisites(int(graph_parameters[0]), prerequisites[i])
                        else:
                            self.initialize_prerequisites(int(graph_parameters[0]), int(prerequisites[i]))
        except Exception as e:
            pass




    # def init_durations(self):
    #     for vertex in range(0, 7):
    #         self._durations[vertex] = []
    #         self.directed_graph.add_vertex(vertex)
    #
    #     self._durations[0].append(1)
    #     self._durations[5].append(2)
    #     self._durations[6].append(5)
    #     self._durations[4].append(1)
    #     self._durations[1].append(2)
    #     self._durations[3].append(2)
    #     self._durations[2].append(1)
    #
    # def init_prerequisites(self):
    #     for vertex in range(0, 10):
    #         self._prerequisites[vertex] = []
    #
    #     self._prerequisites[0] = float('inf')  # no prerequisites
    #     self._prerequisites[5] = float('inf')  # no prerequisites
    #     self._prerequisites[6].append(0)
    #     self._prerequisites[6].append(5)
    #     self._prerequisites[4].append(5)
    #     self._prerequisites[1].append(6)
    #     self._prerequisites[3].append(4)
    #     self._prerequisites[3].append(6)
    #     self._prerequisites[2].append(3)
    #     self._prerequisites[2].append(6)