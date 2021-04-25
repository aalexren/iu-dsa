import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class Main {

    public static void main(String[] args) throws FileNotFoundException {
        UserInput userInput = new UserInput();

//        userInput.startLoop();
        userInput.shortestPath();
    }
}

class Vertex<E> {
    public E name;

    public Vertex(E value) {
        this.name = value;
    }
}

class Edge<E, V> {

    public Edge(Vertex<E> from, Vertex<E> to, V weight) {
        this.from = from;
        this.to = to;
        this.weight = weight;
    }

    public Edge(Vertex<E> from, Vertex<E> to, V weight, V bandwidth) {
        this.from = from;
        this.to = to;
        this.weight = weight;
        this.bandwidth = bandwidth;
    }

    public Vertex<E> from;
    public Vertex<E> to;
    public V weight;
    public V bandwidth;
}

interface Graph<E, V> {
    Vertex<E> addVertex(E value);
    Vertex<E> removeVertex(Vertex<E> v);
    Edge<E, V> addEdge(Vertex<E> from, Vertex<E> to, V weight);
    Edge<E, V> removeEdge(Edge<E, V> e);
    ArrayList<Edge<E, V>> edgesFrom(Vertex<E> v);
    ArrayList<Edge<E, V>> edgesTo(Vertex<E> v);
    Vertex<E> findVertex(E value);
    Edge<E, V> findEdge(E from_value, E to_value);
    boolean hasEdge(Vertex<E> u, Vertex<E> v);
}

class AdjacencyMatrixGraph<E, V> implements Graph<E, V> {

    public HashMap<Vertex<E>, HashMap<Vertex<E>, Edge<E, V>>> matrix;

    // members to discover cycle in the graph
    private HashMap<Vertex<E>, Integer> colors;
    private Vertex<E> cycle_start;
    private Vertex<E> cycle_end;
    private ArrayList<Vertex<E>> cycle_path;
    private HashMap<Vertex<E>, Vertex<E>> parents; // child -> parent

    private HashMap<E, Vertex<E>> findVertexMap; // additional hashmap to make findVertex for O(1)

    AdjacencyMatrixGraph() {
        matrix = new HashMap<>();
        colors = new HashMap<>();
        cycle_start = null;
        cycle_end = null;
        cycle_path = null;
        parents = null;
        findVertexMap = new HashMap<>();
    }

    @Override
    public Vertex<E> addVertex(E value) {
        Vertex<E> v = new Vertex<>(value);

        if (findVertex(value) == null) {
            matrix.put(v, new HashMap<>());
            findVertexMap.put(value, v);
            return v;
        }

        return null;
    }

    @Override
    public Vertex<E> removeVertex(Vertex<E> v) {
        matrix.remove(v);
        findVertexMap.remove(v.name);

        // looking for that vertex like adjacent
        for (Vertex<E> k: matrix.keySet()) {
            matrix.get(k).remove(v);
        }

        return v;
    }

    @Override
    public Edge<E, V> addEdge(Vertex<E> from, Vertex<E> to, V weight) {
        Edge<E, V> e = new Edge<>(from, to, weight);
        if (matrix.get(from) != null) { // safe from possible exception
            matrix.get(from).put(to, e);
        }

        return e;
    }

    public Edge<E, V> addEdge(Vertex<E> from, Vertex<E> to, V weight, V bandwidth) {
        Edge<E, V> e = new Edge<>(from, to, weight, bandwidth);
        if (matrix.get(from) != null) { // safe from possible exception
            matrix.get(from).put(to, e);
        }

        return e;
    }

    @Override
    public Edge<E, V> removeEdge(Edge<E, V> e) {
        if (matrix.get(e.from) != null) { // safe from possible exception
            matrix.get(e.from).remove(e.to);
        }

        return e;
    }

    /**
     * return a collection or edge objects that are going from vertex v;
     */
    @Override
    public ArrayList<Edge<E, V>> edgesFrom(Vertex<E> v) {
        return new ArrayList<>(matrix.get(v).values());
    }

    /**
     * return a collection or edge objects that are going into vertex v;
     */
    @Override
    public ArrayList<Edge<E, V>> edgesTo(Vertex<E> v) {
        ArrayList<Edge<E, V>> ret = new ArrayList<>();

        for (Vertex<E> k: matrix.keySet()) {
            if (v != k && matrix.get(k).containsKey(v)) {
                ret.add(matrix.get(k).get(v));
            }
        }

        return ret;
    }

    @Override
    public Vertex<E> findVertex(E value) {
        return findVertexMap.get(value);
    }

    @Override
    public Edge<E, V> findEdge(E from_value, E to_value) {
        Vertex<E> from = findVertex(from_value);
        Vertex<E> to = findVertex(to_value);

        return (from != null && to != null) ? matrix.get(from).get(to) : null;
    }

    @Override
    public boolean hasEdge(Vertex<E> u, Vertex<E> v) {
        return matrix.get(u) != null && matrix.get(u).containsKey(v);
    }

    // Try to use DFS for it
    // Take vertex, visit all the possible in connected component
    // If there is the same met twice when there is a cycle
    public ArrayList<Edge<E, V>> isAcyclic() {
        // to observe the color of vertex
        // 0 - not visited
        // 1 - in progress
        // 2 - fully visited
        colors = new HashMap<>();
        for (Vertex<E> k: matrix.keySet()) {
            colors.put(k, 0);
        }
        cycle_start = null; // abcense of cycle
        cycle_end = null;

        cycle_path = new ArrayList<>(); // vertices in cycle if exist
        parents = new HashMap<>();
        for (Vertex<E> k: matrix.keySet()) {
            if (DFS(k)) break;
        }

        // restore the cycle
        if (cycle_start != null) {
            cycle_path.add(cycle_start);
            Vertex<E> v = cycle_end;
            while (v != cycle_start) {
                cycle_path.add(v);
                v = parents.get(v); // like predecessors array, but actually hashmap, because it more comfortable
            }
            cycle_path.add(cycle_start);

            Collections.reverse(cycle_path); // because we added it from the end

            // make an array list of edges using the vertices we got
            ArrayList<Edge<E, V>> ret = new ArrayList<>();
            for (int i = 0; i < cycle_path.size() - 1; ++i) {
                ret.add(matrix.get(cycle_path.get(i)).get(cycle_path.get(i + 1)));
            }
            return ret;
        }
        else return null;
    }

    // pretty simple standard DFS
    private boolean DFS(Vertex<E> start) {
        colors.put(start, 1);

        // check all adjacent
        for (Vertex<E> to: matrix.get(start).keySet()) {
            if (colors.get(to) == 0) {
                parents.put(to, start);
                if (DFS(to)) return true;
            }
            if (colors.get(to) == 1) {
                cycle_start = to;
                cycle_end = start;
                return true;
            }
        }

        colors.put(start, 2);
        return false;
    }

    /**
     * Make a new HashMap to fill;
     * Just iterate for every vertex (to consider all the edges)
     * Swap target and source in edges
     */
    public void transpose() {
        HashMap<Vertex<E>, HashMap<Vertex<E>, Edge<E, V>>> ret = new HashMap<>();

        for (Vertex<E> k: matrix.keySet()) {
            ret.put(k, new HashMap<>());
        }

        for (Vertex<E> k: matrix.keySet()) {
            for (Vertex<E> l: matrix.get(k).keySet()) {
                Edge<E, V> e = matrix.get(k).get(l);
                Vertex<E> tmp = e.from;
                e.from = e.to;
                e.to = tmp;
                ret.get(l).put(k, e);
            }
        }

        matrix = ret;
    }
}

// special declared class for task 3
class ShortestPath {
    // since we know the conditions
    // we can use non-template types
    public AdjacencyMatrixGraph<String, Integer> graph;

    public ShortestPath() {
        graph = new AdjacencyMatrixGraph<>();
    }

    /**
     * To find the shortest path from the source to the target
     * we use Dijkstra algorithm.
     * Consider start vertex, all the adjacent, choosing the minimal weight with
     * Making relaxing, mark vertex as considered
     */
    public void shortestPath(Vertex<String> start, Vertex<String> finish, int bandWidth) {
        HashMap<Vertex<String>, Integer> dist = new HashMap<>();
        HashMap<Vertex<String>, Vertex<String>> prev = new HashMap<>();
        HashMap<Vertex<String>, Boolean> used = new HashMap<>(); // instead of PriorityQueue
        // priority queue actually harder to use than one more hashmap

        for (Vertex<String> k: graph.matrix.keySet()) {
            dist.put(k, Integer.MAX_VALUE);
            prev.put(k, null);
            used.put(k, false);
        }
        dist.put(start, 0);

        for (int i = 0; i < graph.matrix.size(); ++i) {
            Vertex<String> minVertex = null;
            for (Vertex<String> v: graph.matrix.keySet()) {
                if (!used.get(v) && (minVertex == null || dist.get(v) < dist.get(minVertex))) {
                    minVertex = v;
                }
            }
            if (dist.get(minVertex) == Integer.MAX_VALUE) break;
            used.put(minVertex, true);

            for (Vertex<String> k: graph.matrix.get(minVertex).keySet()) {
                int weight = graph.matrix.get(minVertex).get(k).weight;
                int bandwidth = graph.matrix.get(minVertex).get(k).bandwidth;
                if (!dist.get(minVertex).equals(Integer.MAX_VALUE) &&
                        bandwidth >= bandWidth && // bandwidth from the condition
                        dist.get(minVertex) + weight < dist.get(k)) {
                    dist.put(k, dist.get(minVertex) + weight);
                    prev.put(k, minVertex);
                }
            }
        }

        if (dist.get(finish).equals(Integer.MAX_VALUE)) {
            System.out.println("IMPOSSIBLE");
        }
        else {
            int retBandwidth = Integer.MAX_VALUE;
            int retDist = 0;
            int retPrevCount = 1;
            Vertex<String> son = finish;
            Vertex<String> father = prev.get(son); // iterate for predecessors
            ArrayList<Vertex<String>> path = new ArrayList<>();
            path.add(son);
            while (father != null) {
                path.add(father);
                retPrevCount += 1;
                retDist += graph.matrix.get(father).get(son).weight;
                retBandwidth = Math.min(graph.matrix.get(father).get(son).bandwidth, retBandwidth);
                son = father;
                father = prev.get(son);
            }
            System.out.println(retPrevCount + " " + retDist + " " + retBandwidth);
            Collections.reverse(path); // cause we started from the end
            for (Vertex<String> v : path) {
                System.out.print(v.name + " ");
            }
        }
    }
}

class UserInput {
    public AdjacencyMatrixGraph<String, Integer> graph;
    public ShortestPath shortestPath;

    public UserInput() {
        graph = new AdjacencyMatrixGraph<>();
        shortestPath = new ShortestPath();
    }

    public void shortestPath() throws FileNotFoundException {
        Scanner scanner = new Scanner(System.in);
        int N = scanner.nextInt(), M = scanner.nextInt();
        for (int i = 1; i <= N; ++i) {
            shortestPath.graph.addVertex(Integer.toString(i));
        }

        for (int i = 0; i < M; ++i) {
            int source = scanner.nextInt();
            int target = scanner.nextInt();
            int weight = scanner.nextInt();
            int bandwidth = scanner.nextInt();
            Vertex<String> sv = shortestPath.graph.findVertex(Integer.toString(source));
            Vertex<String> tv = shortestPath.graph.findVertex(Integer.toString(target));
            shortestPath.graph.addEdge(sv, tv, weight, bandwidth);
        }
        int start_s = scanner.nextInt();
        int finish_s = scanner.nextInt();
        int W = scanner.nextInt();
        Vertex<String> start = shortestPath.graph.findVertex(Integer.toString(start_s));
        Vertex<String> finish = shortestPath.graph.findVertex(Integer.toString(finish_s));

        shortestPath.shortestPath(start, finish, W);
    }

    public void startLoop() {
        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNext()) {
            String[] strs = scanner.nextLine().split(" ");
            try {
                getCommand(strs[0], Arrays.copyOfRange(strs, 1, strs.length));
            } catch (Exception ignored) { }
        }
    }

    public void getCommand(String cmd, String[] args) {
        String ret = null;

        switch(cmd) {
            case "ADD_VERTEX": execAddVertex(args); break;
            case "REMOVE_VERTEX": execRemoveVertex(args); break;
            case "ADD_EDGE": execAddEdge(args); break;
            case "REMOVE_EDGE": execRemoveEdge(args); break;
            case "HAS_EDGE": ret = execHasEdge(args); break;
            case "IS_ACYCLIC": ret = execIsAcyclic(args); break;
            case "TRANSPOSE": execTranspose(args); break;
        }

        if (ret != null) System.out.println(ret);
    }

    private void execAddVertex(String[] args) {
        String name = args[0];
        graph.addVertex(name);
    }

    private void execRemoveVertex(String[] args) {
        String name = args[0];

        Vertex<String> v = graph.findVertex(name);
        graph.removeVertex(v);
    }

    private void execAddEdge(String[] args) {
        String from = args[0];
        String to = args[1];
        Integer weight = Integer.parseInt(args[2]);

        Vertex<String> from_v = graph.findVertex(from);
        Vertex<String> to_v = graph.findVertex(to);
        graph.addEdge(from_v, to_v, weight);
    }

    private void execRemoveEdge(String[] args) {
        String from = args[0];
        String to = args[1];

        Edge<String, Integer> e = graph.findEdge(from, to);
        graph.removeEdge(e);
    }

    private String execHasEdge(String[] args) {
        String from = args[0];
        String to = args[1];

        Vertex<String> from_v = graph.findVertex(from);
        Vertex<String> to_v = graph.findVertex(to);

        return graph.hasEdge(from_v, to_v) ? "TRUE" : "FALSE";
    }

    private String execIsAcyclic(String[] args) {
        ArrayList<Edge<String, Integer>> path = graph.isAcyclic();
        if (path == null) {
            return "ACYCLIC";
        }
        else {
            Integer weight = 0;
            StringBuilder sb = new StringBuilder();
            for (Edge<String, Integer> e: path) {
                sb.append(e.from.name).append(" ");
                weight += e.weight;
            }

            return weight.toString().concat(" ".concat(sb.toString().trim()));
        }
    }

    private void execTranspose(String[] args) {
        graph.transpose();
    }
}
