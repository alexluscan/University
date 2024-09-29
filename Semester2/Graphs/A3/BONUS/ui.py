from service import Service


class UI:
    def __init__(self):
        self.__service = Service()
        pass

    def print_graph(self):
        """
        Prints the graph to the console
        """
        print(str(self.__service.graph.get_number_of_vertices()) +
              " " + str(self.__service.graph.get_number_of_edges()))
        out_edges = self.__service.graph.get_out_edges()
        for x in out_edges:
            for y in out_edges[x]:
                print(str(out_edges[x][y]) + ") " + str(x) + " " + str(y) + " " +
                      str(self.__service.graph.get_cost_of_edge(out_edges[x][y])))

    def print_vertices(self):
        """
        Prints the vertices of the graph
        """
        vertices = self.__service.graph.get_set_of_vertices()
        print("The vertices of the graph are: ")
        for v in vertices:
            print(str(v))

    def check_if_edge_exists(self):
        """
        Displays console menu for checking if an edge exists in the graph
        """
        x, y = input("Please enter the 2 vertices: ").split()
        try:
            x, y = int(x), int(y)
        except ValueError:
            print("Invalid input! Please try again!")
            return
        if self.__service.graph.check_if_edge_exists(x, y):
            print("There is an edge from " + str(x) + " to " + str(y))
        else:
            print("There is no edge from " + str(x) + " to " + str(y))

    def get_in_degree_and_out_degree(self):
        """
        Displays console menu for getting the in degree and out degree of a vertex
        """
        try:
            v = int(input("Please enter the vertex: "))
        except ValueError:
            print("Invalid input!")
            return
        print("The in degree of " + str(v) + " is " +
              str(self.__service.graph.get_in_degree(v)))
        print("The out degree of " + str(v) + " is " +
              str(self.__service.graph.get_out_degree(v)))

    def parse_outbound_edges(self):
        """
        Parses the outbound edges of a vertex
        """
        try:
            v = int(input("Please enter the vertex: "))
        except ValueError:
            print("Invalid input!")
            return
        out_edges = self.__service.graph.get_out_edges()
        if v in out_edges:
            if len(out_edges[v]) == 0:
                print("There are no outbound edges for " + str(v))
            else:
                print("The outbound edges of " + str(v) + " are: ")
                for y in out_edges[v]:
                    print(str(v) + " " + str(y) + " " + str(out_edges[v][y]))
        else:
            print("There are no outbound edges for " + str(v))

    def parse_inbound_edges(self):
        """
        Parses the inbound edges of a vertex
        """
        try:
            v = int(input("Please enter the vertex: "))
        except ValueError:
            print("Invalid input!")
            return
        in_edges = self.__service.graph.get_in_edges()
        if v in in_edges:
            if len(in_edges[v]) == 0:
                print("There are no inbound edges for " + str(v))
            else:
                print("The inbound edges of " + str(v) + " are: ")
                for x in in_edges[v]:
                    print(str(x) + " " + str(v) + " " + str(in_edges[v][x]))
        else:
            print("There are no inbound edges for " + str(v))

    def get_endpoints_of_edge(self):
        """
        Prints the endpoint of an edge
        """
        try:
            e = int(input("Please enter the edge: "))
        except ValueError:
            print("Invalid input!")
            return
        x, y = self.__service.graph.get_endpoints_of_edge(e)
        if x == -1:
            print("There is no edge with ID " + str(e))
        else:
            print("The endpoints of edge " + str(e) +
                  " are " + str(x) + " and " + str(y))

    def modify_cost_of_edge(self):
        """
        Interactive User Menu for modifying the cost of an edge
        """
        try:
            id = int(input("Cost of which edge do you want to modify? "))
        except ValueError:
            print("Invalid input!")
            return
        try:
            new_cost = int(input("What is the new cost? "))
        except ValueError:
            print("Invalid input!")
            return
        self.__service.graph.modify_cost_of_edge(id, new_cost)
        print("The cost of the edge has been modified to " + str(new_cost))

    def add_edge(self):
        """
        Menu to add an edge to the graph. If the graph is empty, the user MUST provide the number of vertices first. In case the user adds an edge with a vertex that does not exist, the vertex will be added to the graph. The vertices MUST BE smaller than n - 1 and greater than 0.
        """
        if self.__service.graph.get_number_of_vertices() == 0:
            print("First, you have to add vertices before you can add an edge!")
            return
        try:
            x, y, cost = input(
                "Please enter the 2 vertices and the cost: ").split()
            x, y, cost = int(x), int(y), int(cost)
        except ValueError:
            print("Invalid input!")
            return
        if x >= self.__service.graph.get_number_of_vertices() or y >= self.__service.graph.get_number_of_vertices():
            print("The provided vertices MUST BE between 0 and n - 1. Try again!")
        elif self.__service.graph.check_if_edge_exists(x, y):
            print("There is already an edge from " + str(x) + " to " + str(y))
        else:
            self.__service.graph.add_edge_to_graph(x, y, cost)
            print("The edge from " + str(x) +
                  " to " + str(y) + " has been added")

    def remove_edge(self):
        """
        Menu to remove an edge from the graph. If the graph is empty, operation can not be performed.
        """
        if self.__service.graph.get_number_of_vertices() == 0:
            print("First, you have to add vertices before you can remove an edge!")
            return
        try:
            x, y = input("Please enter the 2 vertices: ").split()
            x, y = int(x), int(y)
        except ValueError:
            print("Invalid input!")
            return
        if self.__service.graph.check_if_edge_exists(x, y):
            self.__service.graph.remove_edge_from_graph(x, y)
            print("The edge from " + str(x) + " to " +
                  str(y) + " has been removed!")
        else:
            print("There is no edge from " + str(x) + " to " + str(y))

    def add_vertex(self):
        """
        Menu to add a vertex to the graph. If the graph is empty, the user MUST provide the number of vertices first. In case the user adds a vertex that already exists, the operation will have no effect on the graph.
        """
        if self.__service.graph.get_number_of_vertices() == 0:
            try:
                v = int(
                    input("The graph is empty! Please provide the number of vertices first: "))
            except ValueError:
                print("Invalid input!")
                return
            self.__service.graph.set_number_of_vertices(v)
        try:
            v = int(input("Please enter the vertex: "))
        except ValueError:
            print("Invalid input!")
            return
        self.__service.graph.add_vertex_in_graph(v)
        print("Done!")

    def remove_vertex(self):
        """
        Menu to remove a vertex from the graph. No extra requirements.
        """
        try:
            v = int(input("Please enter the vertex: "))
        except ValueError:
            print("Invalid input!")
            return
        self.__service.graph.remove_vertex_from_graph(v)
        print("Done!")

    def copy_graph(self):
        """
        Function to copy the graph
        """
        copy_of_graph = self.__service.graph.get_copy_of_graph()
        print("Done!")

    def print_the_cost(self):
        """
        Function to print the cost of the graph
        """
        try:
            id = int(input("Please enter the id of the edge: "))
        except ValueError:
            print("Invalid input!")
            return
        print("The cost of the graph is " +
              str(self.__service.graph.get_cost_of_edge(id)))

    def read_graph_from_file(self):
        """
        User Interface for reading a graph from a file. The user has to provide the filename.
        """
        filename = input("Please enter the filename: ")
        self.__service.read_graph_from_file(filename)
        print("Done!")

    def write_graph_to_file(self):
        """
        User Interface for writing a graph to the file. The user has to provide the filename.
        """
        filename = input("Please enter the filename: ")
        self.__service.write_graph_to_file(filename)
        print("Done!")

    def generate_random_graph(self):
        """
        Function to generate a random graph. The user has to provide the number of vertices and edges.
        """
        try:
            n, m = input(
                "Please enter the number of vertices and edges: ").split()
            n, m = int(n), int(m)
        except ValueError:
            print("Invalid input!")
            return
        self.__service.generate_random_graph(n, m)
        print("Done!")

    def find_the_lowest_cost_walk(self):
        try:
            n, m = input(
                "Please enter the start vertex and the end vertex: ").split()
            n, m = int(n), int(m)
        except ValueError:
            print("Invalid input!")
            return
        path, cost = self.__service.find_lowest_cost_walk(n, m, k);
        if path == None:
            print("There is no path from " + str(n) + " to " + str(m))
            return
        print("The path is " + str(path) + " and the cost is " + str(cost))

    def find_the_numeber_of_lowest_cost_walks(self):
        try:
            n, m = input(
                "Please enter the start vertex and the end vertex: ").split()
            n, m = int(n), int(m)
        except ValueError:
            print("Invalid input!")
            return
        path, cost = self.__service.find_minimum_cost_walk(n, m)
        if path == None:
            print("There is no path from " + str(n) + " to " + str(m))
            return
        print("The cost is " + str(cost))
        number = self.__service.find_number_of_walks_with_minimum_cost(n, m)
        print("The number of walks with the minimum cost is " + str(number))

    def find_number_of_walks_between_two_vertices(self):
        try:
            n, m = input(
                "Please enter the start vertex and the end vertex: ").split()
            n, m = int(n), int(m)
        except ValueError:
            print("Invalid input!")
            return
        number = self.__service.find_number_of_walks_between_two_vertices(n, m)
        print("The number of walks between " + str(n) +
              " and " + str(m) + " is " + str(number))

    def solve_bridge_and_torch_problem(self):
        try:
            n = input("Please enter the number of people: ")
            n = int(n)
        except ValueError:
            print("Invalid input!")
            return
        people = []
        for i in range(n):
            people.append(int(input("Time for person " + str(i + 1) + ": ")))
        cost = self.__service.find_minimum_time_for_transfer(people, n)
        print("The minimum time for transfer is " + str(cost))

    def print_menu(self):
        """
        Function to print the menu of the application. The user will choose an option from the menu.
        """
        print("Menu:")
        print("0. Read graph from file")
        print("BONUSES");
        print("1. Find the number of lowest length paths between 2 vertices")
        print("2. Find the number of walks between 2 vertices")
        print("3. Solve the bridge and torch problem")
        print("4. Exit")

    def run(self):
        """
        Main function that performs all the jobs
        """
        print("Welcome to the graph application!")
        while True:
            self.print_menu()
            command = input("Please enter a command: ")
            if command == "0":
                self.read_graph_from_file()
            elif command == "1":
                self.find_the_numeber_of_lowest_cost_walks()
            elif command == "2":
                self.find_number_of_walks_between_two_vertices()
            elif command == "3":
                self.solve_bridge_and_torch_problem()
            elif command == "4":
                filename = "graph" + \
                    str(self.__service.graph.get_number_of_vertices()) + "_modif.txt"
                self.__service.write_graph_to_file(filename)
                print("Goodbye!")
                return
            else:
                print("Invalid command!. Please try again!")
