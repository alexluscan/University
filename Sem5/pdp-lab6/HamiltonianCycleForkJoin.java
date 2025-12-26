import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringTokenizer;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public final class HamiltonianCycleForkJoin {

    private static final int START_NODE = 1;

    public static void main(String[] args) throws IOException {
        if (args.length < 1) {
            System.err.println("Usage: java HamiltonianCycleForkJoin <input-file>");
            return;
        }

        Graph graph = Graph.fromFile(args[0]);
        if (graph.size() == 0) {
            System.err.println("Graph is empty.");
            return;
        }

        ForkJoinPool pool = new ForkJoinPool();
        long startTime = System.nanoTime();

        boolean[] visited = new boolean[graph.size() + 1];
        visited[START_NODE] = true;
        ArrayList<Integer> seedPath = new ArrayList<>();
        seedPath.add(START_NODE);

        CycleTask task = new CycleTask(graph, seedPath, visited);
        List<Integer> cycle = pool.invoke(task);

        double elapsedSeconds = (System.nanoTime() - startTime) / 1_000_000_000.0;
        if (cycle != null) {
            System.err.printf("Found a hamiltonian cycle in %.3f seconds.%n", elapsedSeconds);
            printCycle(cycle);
        } else {
            System.err.printf("No hamiltonian cycle found in %.3f seconds.%n", elapsedSeconds);
        }
        pool.shutdown();
    }

    private static void printCycle(List<Integer> cycle) {
        StringBuilder sb = new StringBuilder();
        for (int node : cycle) {
            sb.append(node).append(' ');
        }
        sb.append(cycle.get(0));
        System.out.println(sb);
    }

    private record Graph(int size, List<List<Integer>> adj) {
        static Graph fromFile(String path) throws IOException {
            try (BufferedReader reader = new BufferedReader(new FileReader(path))) {
                String header = reader.readLine();
                if (header == null || header.isEmpty()) {
                    return new Graph(0, List.of());
                }
                StringTokenizer st = new StringTokenizer(header);
                int n = Integer.parseInt(st.nextToken());
                int m = Integer.parseInt(st.nextToken());

                List<List<Integer>> adj = new ArrayList<>(n + 1);
                for (int i = 0; i <= n; i++) {
                    adj.add(new ArrayList<>());
                }

                for (int i = 0; i < m; i++) {
                    String line = reader.readLine();
                    if (line == null) {
                        break;
                    }
                    StringTokenizer edge = new StringTokenizer(line);
                    int u = Integer.parseInt(edge.nextToken());
                    int v = Integer.parseInt(edge.nextToken());
                    adj.get(u).add(v);
                }
                return new Graph(n, adj);
            }
        }

        boolean hasEdge(int from, int to) {
            return adj.get(from).contains(to);
        }

        List<Integer> neighbors(int node) {
            return adj.get(node);
        }
    }

    private static final class CycleTask extends RecursiveTask<List<Integer>> {
        private final Graph graph;
        private final ArrayList<Integer> path;
        private final boolean[] visited;

        CycleTask(Graph graph, ArrayList<Integer> path, boolean[] visited) {
            this.graph = graph;
            this.path = path;
            this.visited = visited;
        }

        @Override
        protected List<Integer> compute() {
            if (path.size() == graph.size()) {
                int last = path.get(path.size() - 1);
                return graph.hasEdge(last, path.get(0)) ? path : null;
            }

            int current = path.get(path.size() - 1);
            List<Integer> neighbors = graph.neighbors(current);

            List<CycleTask> forks = new ArrayList<>();
            List<Integer> result = null;
            int remaining = graph.size() - path.size();

            for (int i = 0; i < neighbors.size(); i++) {
                int next = neighbors.get(i);
                if (visited[next]) {
                    continue;
                }

                boolean[] nextVisited = Arrays.copyOf(visited, visited.length);
                nextVisited[next] = true;
                ArrayList<Integer> nextPath = new ArrayList<>(path);
                nextPath.add(next);

                CycleTask subTask = new CycleTask(graph, nextPath, nextVisited);
                if (remaining > 6 && i < neighbors.size() - 1) {
                    subTask.fork();
                    forks.add(subTask);
                } else {
                    result = subTask.compute();
                    if (result != null) {
                        return result;
                    }
                }
            }

            for (CycleTask fork : forks) {
                result = fork.join();
                if (result != null) {
                    break;
                }
            }
            return result;
        }
    }
}

